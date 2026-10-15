#!/usr/bin/env python3
"""Run HTTP integration checks against a local vmetajsonhttp executable.

Usage: python3 test_http.py ../project/unix/vmetajsonhttp ../../../dct/binary
Only Python's standard library is required. Each server uses an ephemeral port.
"""

import argparse
import concurrent.futures
import contextlib
import http.client
import json
import pathlib
import socket
import subprocess
import tempfile
import time
import urllib.parse


def request(base_url, path, payload=None, *, raw=None, status=200,
            content_type="application/json"):
    if raw is not None:
        data = raw
    elif payload is not None:
        data = json.dumps(payload, ensure_ascii=False).encode("utf-8")
    else:
        data = None
    url = urllib.parse.urlsplit(base_url)
    connection = http.client.HTTPConnection(url.hostname, url.port, timeout=10)
    try:
        headers = {"Content-Type": content_type} if data is not None and content_type is not None else {}
        connection.request("POST" if data is not None else "GET", path, body=data, headers=headers)
        response = connection.getresponse()
        body = response.read()
        expected_statuses = status if isinstance(status, tuple) else (status,)
        assert response.status in expected_statuses, (
            path, response.status, status, body.decode("utf-8", errors="replace")
        )
        assert response.headers.get_content_type() == "application/json"
    finally:
        connection.close()
    result = json.loads(body.decode("utf-8"))
    assert isinstance(result, dict), result
    if response.status >= 400:
        assert any(result.get(key) for key in ("error", "errors", "warnings", "failure")), result
    return result


@contextlib.contextmanager
def running_server(binary, dictionary_dir, *flags):
    with socket.socket() as sock:
        sock.bind(("127.0.0.1", 0))
        port = sock.getsockname()[1]
    base_url = "http://127.0.0.1:{}".format(port)
    with tempfile.TemporaryFile() as stdout, tempfile.TemporaryFile() as stderr:
        process = subprocess.Popen(
            [str(binary), "--host=127.0.0.1", "--port={}".format(port),
             "--path={}".format(dictionary_dir), *flags],
            stdin=subprocess.DEVNULL,
            stdout=stdout,
            stderr=stderr,
        )
        try:
            deadline = time.monotonic() + 15
            while time.monotonic() < deadline:
                if process.poll() is not None:
                    stderr.seek(0)
                    raise AssertionError("Server exited: " + stderr.read().decode("utf-8", errors="replace"))
                try:
                    if request(base_url, "/health").get("status") == "ok":
                        break
                except OSError:
                    time.sleep(0.05)
            else:
                raise AssertionError("Server did not become healthy within 15 seconds")
            yield base_url
        finally:
            process.terminate()
            try:
                process.wait(timeout=5)
            except subprocess.TimeoutExpired:
                process.kill()
                process.wait(timeout=5)
            stdout.seek(0)
            output = stdout.read()
            assert not output, "HTTP server wrote to stdout: {!r}".format(output)
            stderr.seek(0)
            diagnostics = stderr.read().decode("utf-8", errors="replace")
            assert process.returncode == 0, "HTTP server did not shut down cleanly: {}\n{}".format(
                process.returncode, diagnostics)


def analyses(result):
    tokens = result["annotations"]["tokens"]
    assert tokens, result
    found = [analysis for token in tokens for analysis in token["features"].get("mrf", [])]
    assert found, result
    return found


def check_default_server(base_url):
    assert request(base_url, "/health")["status"] == "ok"
    version = request(base_url, "/version")["version_vmetajson"]
    assert isinstance(version, str) and version

    payload = {"content": "Tere maailm!"}
    request(base_url, "/missing", status=404)
    request(base_url, "/process", payload, content_type="text/plain", status=415)
    request(base_url, "/process", payload, content_type=None, status=415)

    # A body exactly at the unchanged 8 MiB default is accepted.
    encoded = json.dumps(payload).encode("utf-8")
    default_sized_body = encoded + b" " * (8 * 1024 * 1024 - len(encoded))
    analyses(request(base_url, "/process", raw=default_sized_body))

    # Check rejection above the limit without transferring 9 MiB to the server.
    url = urllib.parse.urlsplit(base_url)
    connection = http.client.HTTPConnection(url.hostname, url.port, timeout=10)
    try:
        connection.request("POST", "/process", headers={
            "Content-Type": "application/json", "Content-Length": str(9 * 1024 * 1024),
        })
        response = connection.getresponse()
        assert response.status == 413
        assert response.headers.get_content_type() == "application/json"
        assert json.loads(response.read().decode("utf-8")).get("errors")
    finally:
        connection.close()

    baseline = request(base_url, "/process", payload)
    assert baseline["content"] == payload["content"]
    assert [token["features"]["token"] for token in baseline["annotations"]["tokens"]] == ["Tere", "maailm!"]
    assert all("lemma" in analysis and "stem" not in analysis for analysis in analyses(baseline))
    assert "version_vmetajson" not in baseline

    utf8 = "Õun öösel, äike ja üllatus."
    result = request(base_url, "/process", {"content": utf8})
    assert result["content"] == utf8
    assert result["annotations"]["tokens"][0]["features"]["token"] == "Õun"
    analyses(result)

    tab_result = request(base_url, "/process", {"tss": "Tere\tmaailm!"})
    assert [token["features"]["token"] for token in tab_result["annotations"]["tokens"]] == ["Tere", "maailm!"]
    analyses(tab_result)

    tokens = [{"features": {"token": "Tere"}}, {"features": {"token": "maailm!"}}]
    annotated = request(base_url, "/process", {"annotations": {"tokens": tokens}})
    analyses(annotated)
    sentence = request(base_url, "/process", {
        "params": {"vmetajson": ["--guess", "--guesspropnames"]},
        "annotations": {"tokens": tokens, "sentences": [{"features": {"start": 0, "end": 2}}]},
    })
    analyses(sentence)
    assert request(base_url, "/process", payload) == baseline

    with_version = request(base_url, "/process", {
        "content": payload["content"], "params": {"vmetajson": ["--version"]},
    })
    assert with_version["version_vmetajson"] == version
    assert request(base_url, "/process", payload) == baseline

    options = ["--stem", "--gt-category", "--result-string-as-joined", "--result-string-as-discrete"]
    with_options = request(base_url, "/process", {
        "content": payload["content"], "params": {"vmetajson": options},
    })
    for token in with_options["annotations"]["tokens"]:
        assert "result-string-as-joined" in token["features"]
    for analysis in analyses(with_options):
        assert "stem" in analysis and "lemma" not in analysis
        assert "gt-category" in analysis
        assert "result-string-as-discrete" in analysis
    assert request(base_url, "/process", payload) == baseline

    # A restricted analysis depth must not persist into the following request.
    depth_payload = {"content": "tühikuga eraldatud sõned morfimiseks"}
    depth_baseline = request(base_url, "/process", depth_payload)
    request(base_url, "/process", {
        **depth_payload, "params": {"vmetajson": ["--maxcomplexity=1"]},
    })
    assert request(base_url, "/process", depth_payload) == depth_baseline

    invalid_json = [
        b"{", b"", b"[]", b"null", b"123", b'"text"',
        b'{"content":"Tere"} {}',
        b'{"content":"Tere","content":"maailm"}',
        b'/* comment */ {"content":"Tere"}',
    ]
    for raw in invalid_json:
        request(base_url, "/process", raw=raw, status=400)
        assert request(base_url, "/process", payload) == baseline

    invalid_payloads = [
        {},
        {"content": 123},
        {"content": "Tere\u0000maailm"},
        {"content": "Tere", "params": []},
        {"content": "Tere", "params": {"vmetajson": "--stem"}},
        {"content": "Tere", "params": {"vmetajson": [123]}},
        {"content": "Tere", "params": {"vmetajson": ["--stem", "--unknown"]}},
        {"content": "Tere", "params": {"vmetajson": ["--path=/tmp"]}},
        {"content": "Tere", "params": {"vmetajson": ["--json={}"]}},
        {"content": "Tere", "params": {"vmetajson": ["--maxcomplexity=abc"]}},
        {"content": "Tere", "params": {"vmetajson": ["--maxcomplexity=-1"]}},
        {"content": "Tere", "params": {"vmetajson": ["--guesspropnames"]}},
        {"content": "Tere", "params": {"vmetajson": ["--guess", "--guesspropnames"]}},
        {"annotations": []},
        {"annotations": {"tokens": "Tere"}},
        {"annotations": {"tokens": ["Tere"]}},
        {"annotations": {"tokens": [{"features": {}}]}},
        {"annotations": {"tokens": [{"features": {"token": 123}}]}},
        # Validation must reject later bad tokens before entering the engine.
        {"annotations": {"tokens": [tokens[0], {"features": {"token": []}}]}},
    ]
    for features in ({"start": 2, "end": 1}, {"start": 0, "end": 3},
                     {"start": -1, "end": 2}, {"start": "0", "end": 2}):
        invalid_payloads.append({
            "params": {"vmetajson": ["--guess", "--guesspropnames"]},
            "annotations": {"tokens": tokens, "sentences": [{"features": features}]},
        })
    for invalid in invalid_payloads:
        request(base_url, "/process", invalid, status=400)
        assert request(base_url, "/process", payload) == baseline, invalid

    # Empty tokens may be accepted or rejected, but must never abort the server
    # or leave queued engine data to corrupt the following request.
    for empty in ({"content": ""}, {"content": "   "}, {"tss": "\tTere\t"},
                  {"tss": " \tTere"},
                  {"annotations": {"tokens": [{"features": {"token": ""}}]}},
                  {"annotations": {"tokens": [{"features": {"token": "   "}}]}},
                  {"params": {"vmetajson": ["--guess", "--guesspropnames"]},
                   "annotations": {"tokens": [{"features": {"token": ""}}],
                                   "sentences": [{"features": {"start": 0, "end": 1}}]}},
                  {"annotations": {"tokens": []}}):
        request(base_url, "/process", empty, status=(200, 400, 500))
        assert request(base_url, "/process", payload) == baseline

    # Reprocessing an annotated result must replace old analyses, not append duplicates.
    assert request(base_url, "/process", baseline) == baseline

    def parallel_request(index):
        if index % 2:
            result = request(base_url, "/process", {
                "content": payload["content"], "params": {"vmetajson": ["--stem"]},
            })
            assert all("stem" in item and "lemma" not in item for item in analyses(result))
        else:
            assert request(base_url, "/process", payload) == baseline

    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as pool:
        list(pool.map(parallel_request, range(32)))
    assert request(base_url, "/process", payload) == baseline
    assert request(base_url, "/health")["status"] == "ok"


def check_startup_defaults(base_url):
    payload = {"content": "Tere maailm!"}
    baseline = request(base_url, "/process", payload)
    assert baseline["version_vmetajson"] == request(base_url, "/version")["version_vmetajson"]
    assert all("stem" in item and "lemma" not in item for item in analyses(baseline))
    assert all("result-string-as-joined" in token["features"] for token in baseline["annotations"]["tokens"])

    # Explicit request flags replace command-line morphology defaults.
    override = request(base_url, "/process", {**payload, "params": {"vmetajson": []}})
    assert all("lemma" in item and "stem" not in item for item in analyses(override))
    assert "version_vmetajson" not in override
    assert all("result-string-as-joined" not in token["features"] for token in override["annotations"]["tokens"])
    assert request(base_url, "/process", payload) == baseline
    request(base_url, "/process", {
        **payload, "params": {"vmetajson": ["--unknown"]},
    }, status=400)
    assert request(base_url, "/process", payload) == baseline


def check_content_length_limits(binary, dictionary_dir):
    limit = 64
    json_text = json.dumps({"content": "Õun öösel"}, ensure_ascii=False)
    encoded = json_text.encode("utf-8")
    exact_body = encoded + b" " * (limit - len(encoded))
    with running_server(binary, dictionary_dir,
                        "--max_content_length={}".format(limit)) as base_url:
        # Whitespace padding changes the HTTP body size without changing the JSON.
        result = request(base_url, "/process", raw=exact_body)
        assert result["content"] == "Õun öösel"
        analyses(result)
        request(base_url, "/process", raw=exact_body + b" ", status=413)

        # The limit counts UTF-8 bytes, so a body of 64 characters is too large.
        character_sized = (json_text + " " * (limit - len(json_text))).encode("utf-8")
        assert len(character_sized) > limit
        request(base_url, "/process", raw=character_sized, status=413)
        assert request(base_url, "/process", raw=exact_body) == result
        assert request(base_url, "/health")["status"] == "ok"

    # A configured limit can also increase the default of 8 MiB. Trailing JSON
    # whitespace keeps the morphology work and response small for this request.
    raised_limit = 8 * 1024 * 1024 + 1
    encoded = b'{"content":"Tere"}'
    large_body = encoded + b" " * (raised_limit - len(encoded))
    with running_server(binary, dictionary_dir,
                        "--max_content_length={}".format(raised_limit)) as base_url:
        result = request(base_url, "/process", raw=large_body)
        assert result["content"] == "Tere"
        analyses(result)


def check_invalid_content_length_flags(binary):
    for value in ("", "0", "-1", "+128", "1.5", "1e3", "0x100", "abc",
                  " 128", "128 ", "9" * 100):
        # --help avoids dictionary loading if a bad value is accidentally accepted.
        result = subprocess.run(
            [str(binary), "--max_content_length=" + value, "--help"],
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=10,
        )
        assert result.returncode != 0, (value, result.stdout, result.stderr)
        assert b"max_content_length" in result.stderr, (value, result.stderr)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("binary", type=pathlib.Path)
    parser.add_argument("dictionary_dir", type=pathlib.Path)
    args = parser.parse_args()
    binary = args.binary.resolve()
    dictionary_dir = args.dictionary_dir.resolve()
    if not binary.is_file():
        parser.error("Executable does not exist: {}".format(binary))
    if not (dictionary_dir / "et.dct").is_file():
        parser.error("Dictionary directory does not contain et.dct: {}".format(dictionary_dir))
    with running_server(binary, dictionary_dir) as base_url:
        check_default_server(base_url)
    with running_server(binary, dictionary_dir, "--stem", "--version",
                        "--result-string-as-joined", "--maxcomplexity=1") as base_url:
        check_startup_defaults(base_url)
    check_content_length_limits(binary, dictionary_dir)
    check_invalid_content_length_flags(binary)
    print("HTTP integration checks passed (including content limits, request isolation and concurrent requests).")


if __name__ == "__main__":
    main()
