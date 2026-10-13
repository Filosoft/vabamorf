#!/usr/bin/env python3
"""Run HTTP integration checks against a local vmetsjsonhttp executable.

Usage: python3 test_http.py ../project/unix/vmetsjsonhttp ../../../dct/binary
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


def token_payload(*features):
    return {"annotations": {"tokens": [{"features": item} for item in features]}}


def forms(result):
    tokens = result["annotations"]["tokens"]
    assert tokens, result
    found = [form for token in tokens for form in token["features"].get("mrf", [])]
    assert found, result
    return found


def surface(form):
    return form["stem"] + ("" if form["ending"] == "0" else form["ending"]) + form["kigi"]


def check_default_server(base_url):
    assert request(base_url, "/health")["status"] == "ok"
    version = request(base_url, "/version")["version_vmetsjson"]
    assert isinstance(version, str) and version

    features = {"token": "palk", "hint": "palga", "pos": "S", "fs": "sg g"}
    payload = token_payload(features)
    request(base_url, "/missing", status=404)
    request(base_url, "/process", payload, content_type="text/plain", status=415)
    request(base_url, "/process", payload, content_type=None, status=415)

    # Check the body limit without transferring 9 MiB to the local server.
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
    assert {surface(form) for form in forms(baseline)} == {"palga"}
    assert all(form["fs"] == "sg g" and form["pos"] == "S" for form in forms(baseline))
    for key, value in features.items():
        assert baseline["annotations"]["tokens"][0]["features"][key] == value
    assert "version" not in baseline
    assert "classic" not in baseline["annotations"]["tokens"][0]["features"]
    assert all("gt" not in form for form in forms(baseline))
    assert request(base_url, "/process", payload,
                   content_type="Application/JSON; charset=utf-8") == baseline

    # The hint must distinguish the two meanings and inflections of palk.
    palgi = request(base_url, "/process", token_payload({**features, "hint": "palgi"}))
    assert {surface(form) for form in forms(palgi)} == {"palgi"}
    plural = request(base_url, "/process", token_payload({
        "token": "president", "pos": "S", "fs": "pl p",
    }))
    assert {surface(form) for form in forms(plural)} == {"presidente", "presidentisid"}
    assert all(form["fs"] == "pl p" for form in forms(plural))
    utf8 = request(base_url, "/process", token_payload({
        "token": "õun", "pos": "S", "fs": "sg g", "kigi": "gi",
    }))
    assert {surface(form) for form in forms(utf8)} == {"õunagi"}

    content = request(base_url, "/process", {"content": "õun president"})
    assert content["content"] == "õun president"
    assert [token["features"]["token"] for token in content["annotations"]["tokens"]] == ["õun", "president"]
    assert all(token["features"].get("mrf") for token in content["annotations"]["tokens"])
    unknown = request(base_url, "/process", token_payload({
        "token": "qzxvbn", "pos": "S", "fs": "sg g",
    }))
    assert not unknown["annotations"]["tokens"][0]["features"].get("mrf"), unknown

    # Extra application data must survive synthesis unchanged.
    enriched = token_payload({**features, "custom": {"value": "jäätis"}})
    enriched["request_id"] = "test-õun"
    enriched["annotations"]["custom"] = [1, 2]
    enriched["params"] = {"other_service": {"enabled": True}}
    result = request(base_url, "/process", enriched)
    assert result["request_id"] == enriched["request_id"]
    assert result["annotations"]["custom"] == [1, 2]
    assert result["params"] == enriched["params"]
    assert result["annotations"]["tokens"][0]["features"]["custom"] == {"value": "jäätis"}
    assert forms(result) == forms(baseline)

    options = ["--classic", "--gt", "--version"]
    option_payload = {**payload, "params": {"vmetsjson": options}}
    with_options = request(base_url, "/process", option_payload)
    assert with_options["version"] == version
    assert with_options["annotations"]["tokens"][0]["features"]["classic"]
    assert all(form.get("gt") for form in forms(with_options))
    assert {surface(form) for form in forms(with_options)} == {"palga"}
    assert request(base_url, "/process", with_options) == with_options
    assert request(base_url, "/process", payload) == baseline
    for flag in ("--guess", "--addphonetics", "--formattedjson", "--utf8json"):
        forms(request(base_url, "/process", {**payload, "params": {"vmetsjson": [flag]}}))
        assert request(base_url, "/process", payload) == baseline

    invalid_json = [
        b"{", b"", b"[]", b"null", b"123", b'"text"', b"false",
        b'{"content":"palk"} {}',
        b'{"content":"palk","content":"president"}',
        b'/* comment */ {"content":"palk"}',
        b'{"content":"palk",}',
    ]
    for raw in invalid_json:
        request(base_url, "/process", raw=raw, status=400)
        assert request(base_url, "/process", payload) == baseline

    invalid_payloads = [
        {},
        {"content": 123},
        {"content": "palk\u0000president"},
        {"content": "palk", "params": []},
        {"content": "palk", "params": {"vmetsjson": "--classic"}},
        {"content": "palk", "params": {"vmetsjson": [123]}},
        {"content": "palk", "params": {"vmetsjson": ["--classic\u0000"]}},
        {"annotations": []},
        {"annotations": {}},
        {"annotations": {"tokens": "palk"}},
        {"annotations": {"tokens": None}},
        {"annotations": {"tokens": ["palk"]}},
        {"annotations": {"tokens": [{"features": []}]}},
        {"annotations": {"tokens": [{"features": {}}]}},
        token_payload({"token": 123}),
        token_payload({"token": ""}),
        token_payload({"token": " \t\n "}),
        token_payload({"token": "palk\u0000"}),
        # Later malformed tokens must be rejected before invoking the engine.
        token_payload(features, {"token": []}),
    ]
    for flag in ("--unknown", "--stem", "--path=/tmp", "--json={}", "--host=localhost"):
        invalid_payloads.append({**payload, "params": {"vmetsjson": ["--classic", flag]}})
    for field in ("hint", "pos", "fs", "kigi"):
        for value in (123, None, [], {}, False, "bad\u0000value"):
            invalid_payloads.append(token_payload({**features, field: value}))
    for invalid in invalid_payloads:
        request(base_url, "/process", invalid, status=400)
        assert request(base_url, "/process", payload) == baseline, invalid

    for empty in ({"content": ""}, {"content": "   \t"}, token_payload()):
        result = request(base_url, "/process", empty)
        assert not result["annotations"]["tokens"], result
        assert request(base_url, "/process", payload) == baseline

    # The engine splits multiword lemmas internally. Whitespace between words
    # must never reach its analysis queue as an empty token, even in debug builds.
    for token in ("suur  palk", "suur\tpalk", " \tpalk\n "):
        request(base_url, "/process", token_payload({**features, "token": token}),
                status=(200, 400))
        assert request(base_url, "/process", payload) == baseline
    for field in ("hint", "pos", "fs", "kigi"):
        request(base_url, "/process", token_payload({**features, field: ""}))
        assert request(base_url, "/process", payload) == baseline

    # Reprocessing an annotated result must replace forms, not append duplicates.
    assert request(base_url, "/process", baseline) == baseline

    def parallel_request(index):
        if index % 2:
            assert request(base_url, "/process", option_payload) == with_options
        else:
            assert request(base_url, "/process", payload) == baseline

    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as pool:
        list(pool.map(parallel_request, range(32)))
    assert request(base_url, "/process", payload) == baseline
    assert request(base_url, "/health")["status"] == "ok"


def check_startup_defaults(base_url):
    payload = token_payload({"token": "palk", "hint": "palga", "pos": "S", "fs": "sg g"})
    baseline = request(base_url, "/process", payload)
    assert baseline["version"] == request(base_url, "/version")["version_vmetsjson"]
    assert baseline["annotations"]["tokens"][0]["features"]["classic"]
    assert all(form.get("gt") for form in forms(baseline))

    # Explicit request flags replace command-line synthesis defaults.
    override = request(base_url, "/process", {**payload, "params": {"vmetsjson": []}})
    assert {surface(form) for form in forms(override)} == {"palga"}
    assert "version" not in override
    assert "classic" not in override["annotations"]["tokens"][0]["features"]
    assert all("gt" not in form for form in forms(override))
    assert request(base_url, "/process", payload) == baseline
    request(base_url, "/process", {
        **payload, "params": {"vmetsjson": ["--unknown"]},
    }, status=400)
    assert request(base_url, "/process", payload) == baseline


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
    with running_server(binary, dictionary_dir, "--version", "--classic", "--gt") as base_url:
        check_startup_defaults(base_url)
    print("HTTP integration checks passed (including request isolation and concurrent requests).")


if __name__ == "__main__":
    main()
