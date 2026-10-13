#!/usr/bin/env python3
"""Run HTTP integration checks against a local vmetyjsonhttp executable.

Usage: python3 test_http.py ../project/unix/vmetyjsonhttp ../../../dct/binary
Only Python's standard library is required. Each server uses an ephemeral port.
The analyzed sentence fixtures were checked with vmetajson and et3.dct; the
tests require neither the analyzer executable nor et.dct at runtime.
"""

import argparse
import concurrent.futures
import contextlib
import copy
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


def analysis(lemma, pos, category, *, ending="0", discrete=None):
    result = {"lemma": lemma, "ending": ending, "enclitic-gi": "",
              "pos": pos, "fs-category": category}
    if discrete is not None:
        result["result-string-as-discrete"] = discrete
    return result


def token(word, *analyses):
    return {"features": {"token": word, "mrf": list(analyses)}}


def sentence_payload(*tokens):
    return {"annotations": {"tokens": list(tokens),
                            "sentences": [{"features": {"start": 0, "end": len(tokens)}}]}}


def road_payload():
    """See tee on pikk. The noun tee must beat the verb tegema."""
    return sentence_payload(
        token("See", analysis("see", "P", "sg n", discrete="see+0 //_P_ sg n, //")),
        token("tee", analysis("tee", "S", "sg g", discrete="tee+0 //_S_ sg g, //"),
              analysis("tegema", "V", "o", discrete="tege+0 //_V_ o, //"),
              analysis("tee", "S", "sg n", discrete="tee+0 //_S_ sg n, //")),
        token("on", analysis("olema", "V", "b", discrete="ole+0 //_V_ b, //"),
              analysis("olema", "V", "vad", discrete="ole+0 //_V_ vad, //")),
        token("pikk", analysis("pikk", "A", "sg n", discrete="pikk+0 //_A_ sg n, //"),
              analysis("pikk", "S", "sg n", discrete="pikk+0 //_S_ sg n, //")),
        token(".", analysis(".", "Z", "", discrete=". //_Z_ //")),
    )


def prohibition_payload():
    """Ära tee seda. In this sentence tee must be a verb."""
    return sentence_payload(
        token("Ära", analysis("ära", "D", ""), analysis("ära", "V", "neg o")),
        token("tee", analysis("tee", "S", "sg g"), analysis("tegema", "V", "o"),
              analysis("tee", "S", "sg n")),
        token("seda", analysis("see", "P", "sg p", ending="da")),
        token(".", analysis(".", "Z", "")),
    )


def preferred(result, token_index):
    variants = result["annotations"]["tokens"][token_index]["features"]["mrf"]
    assert all(isinstance(item.get("preferred"), bool) for item in variants), variants
    return {(item.get("lemma", item.get("stem")), item["pos"], item["fs-category"])
            for item in variants if item["preferred"]}


def check_default_server(base_url):
    assert request(base_url, "/health")["status"] == "ok"
    version = request(base_url, "/version")["version_vmetyjson"]
    assert isinstance(version, str) and version

    payload = road_payload()
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
    assert preferred(baseline, 1) == {("tee", "S", "sg n")}
    # Both analyses of on have the retained VON tag. Matching an earlier
    # retained analysis must survive a later nonmatching retained analysis.
    assert preferred(baseline, 2) == {("olema", "V", "b"), ("olema", "V", "vad")}
    assert preferred(baseline, 3) == {("pikk", "A", "sg n")}
    assert "version_vmetyjson" not in baseline
    assert request(base_url, "/process", payload,
                   content_type="Application/JSON; charset=utf-8") == baseline
    for source, target in zip(payload["annotations"]["tokens"], baseline["annotations"]["tokens"]):
        assert source["features"]["token"] == target["features"]["token"]
        assert len(source["features"]["mrf"]) == len(target["features"]["mrf"])
        for before, after in zip(source["features"]["mrf"], target["features"]["mrf"]):
            sign = "+ " if after["preferred"] else "- "
            assert after["result-string-as-discrete"] == sign + before["result-string-as-discrete"]

    # Repeated requests, including already marked results, must remain identical.
    assert request(base_url, "/process", baseline) == baseline
    assert request(base_url, "/process", request(base_url, "/process", baseline)) == baseline
    stale = copy.deepcopy(payload)
    for item in stale["annotations"]["tokens"]:
        for variant in item["features"]["mrf"]:
            variant["preferred"] = {"stale": True}
    assert request(base_url, "/process", stale) == baseline

    contrasting = prohibition_payload()
    contrast_result = request(base_url, "/process", contrasting)
    assert preferred(contrast_result, 1) == {("tegema", "V", "o")}
    assert contrast_result["annotations"]["tokens"][0]["features"]["token"] == "Ära"
    assert preferred(contrast_result, 0) == {("ära", "V", "neg o")}
    assert request(base_url, "/process", payload) == baseline

    # The second sentence starts after the first; it must use its own context.
    # Its discrete result exists even though the first sentence has none.
    two_sentences = copy.deepcopy(contrasting)
    offset = len(two_sentences["annotations"]["tokens"])
    two_sentences["annotations"]["tokens"].extend(copy.deepcopy(payload["annotations"]["tokens"]))
    two_sentences["annotations"]["sentences"].append({
        "features": {"start": offset, "end": offset + len(payload["annotations"]["tokens"])}})
    combined = request(base_url, "/process", two_sentences)
    assert combined["annotations"]["tokens"][:offset] == contrast_result["annotations"]["tokens"]
    assert combined["annotations"]["tokens"][offset:] == baseline["annotations"]["tokens"]
    assert request(base_url, "/process", combined) == combined

    # Legacy input uses enclitic_gi, current vmetajson uses enclitic-gi.
    legacy = copy.deepcopy(payload)
    for item in legacy["annotations"]["tokens"]:
        for variant in item["features"]["mrf"]:
            variant["enclitic_gi"] = variant.pop("enclitic-gi")
    legacy_result = request(base_url, "/process", legacy)
    assert preferred(legacy_result, 1) == preferred(baseline, 1)
    assert preferred(legacy_result, 2) == preferred(baseline, 2)
    without_enclitics = copy.deepcopy(payload)
    for item in without_enclitics["annotations"]["tokens"]:
        for variant in item["features"]["mrf"]:
            del variant["enclitic-gi"]
    assert preferred(request(base_url, "/process", without_enclitics), 1) == preferred(baseline, 1)

    # Stem-based analyses are valid too; only the field name changes here.
    stemmed = copy.deepcopy(payload)
    for item in stemmed["annotations"]["tokens"]:
        for variant in item["features"]["mrf"]:
            variant["stem"] = variant.pop("lemma")
    assert preferred(request(base_url, "/process", stemmed), 1) == preferred(baseline, 1)

    enriched = copy.deepcopy(payload)
    enriched["request_id"] = "test-õun"
    enriched["annotations"]["custom"] = [1, 2]
    enriched["params"] = {"other_service": {"enabled": True}}
    enriched["annotations"]["tokens"][0]["features"]["custom"] = {"value": "jäätis"}
    result = request(base_url, "/process", enriched)
    assert result["request_id"] == enriched["request_id"]
    assert result["annotations"]["custom"] == [1, 2]
    assert result["params"] == enriched["params"]
    assert result["annotations"]["tokens"][0]["features"]["custom"] == {"value": "jäätis"}

    version_payload = {**payload, "params": {"vmetyjson": ["--version"]}}
    with_version = request(base_url, "/process", version_payload)
    assert with_version["version_vmetyjson"] == version
    assert request(base_url, "/process", payload) == baseline
    for flag in ("--formattedjson", "--utf8json"):
        flagged = {**payload, "params": {"vmetyjson": [flag]}}
        assert preferred(request(base_url, "/process", flagged), 1) == preferred(baseline, 1)
        assert request(base_url, "/process", payload) == baseline

    invalid_json = [
        b"{", b"", b"[]", b"null", b"123", b'"text"', b"false",
        b'{"annotations":{}} {}', b'{"annotations":{},"annotations":{}}',
        b'/* comment */ {"annotations":{}}', b'{"annotations":{},}',
    ]
    for raw in invalid_json:
        request(base_url, "/process", raw=raw, status=400)
        assert request(base_url, "/process", payload) == baseline

    invalid_payloads = [
        {}, {"content": "See tee on pikk ."}, {"annotations": []},
        {"annotations": {}}, {"annotations": {"tokens": []}},
        {"annotations": {"sentences": []}},
    ]
    for field in ("tokens", "sentences"):
        for value in (None, "invalid", {}, False, 123):
            bad = copy.deepcopy(payload)
            bad["annotations"][field] = value
            invalid_payloads.append(bad)
    for bad_params in ([], None, "invalid", {"vmetyjson": "--version"},
                       {"vmetyjson": [123]}, {"vmetyjson": [None]},
                       {"vmetyjson": ["--version\u0000"]}):
        invalid_payloads.append({**payload, "params": bad_params})
    for flag in ("--unknown", "--guess", "--stem", "--path=/tmp", "--json={}", "--host=localhost"):
        invalid_payloads.append({**payload, "params": {"vmetyjson": ["--version", flag]}})
    for bad_token in (None, "tee", [], {"features": []}, {"features": {}},
                      {"features": {"token": 123}}, {"features": {"token": "tee\u0000"}},
                      {"features": {"token": ""}}, {"features": {"token": " \t\n "}},
                      {"features": {"token": "tee", "mrf": "invalid"}},
                      {"features": {"token": "tee", "mrf": [None]}}):
        bad = copy.deepcopy(payload)
        # Reject later malformed tokens before any earlier valid sentence runs.
        bad["annotations"]["tokens"][-1] = bad_token
        invalid_payloads.append(bad)
    for features in ({"start": 2, "end": 1}, {"start": 0, "end": 6},
                     {"start": -1, "end": 5}, {"start": "0", "end": 5},
                     {"start": 0, "end": None}, {"start": False, "end": 5},
                     {"start": 0.5, "end": 5}, {"start": 0, "end": 2**40},
                     {"start": 0}, {"end": 5}):
        bad = copy.deepcopy(payload)
        bad["annotations"]["sentences"].append({"features": features})
        invalid_payloads.append(bad)
    for sentence in (None, "invalid", {}, {"features": []}):
        bad = copy.deepcopy(payload)
        bad["annotations"]["sentences"].append(sentence)
        invalid_payloads.append(bad)
    for field in ("lemma", "stem", "ending", "enclitic-gi", "enclitic_gi", "pos",
                  "fs-category", "result-string-as-discrete"):
        for value in (123, None, [], {}, False, "bad\u0000value"):
            bad = copy.deepcopy(payload)
            bad["annotations"]["tokens"][1]["features"]["mrf"][1][field] = value
            invalid_payloads.append(bad)
    for field in ("lemma", "ending", "pos", "fs-category"):
        bad = copy.deepcopy(payload)
        del bad["annotations"]["tokens"][1]["features"]["mrf"][1][field]
        invalid_payloads.append(bad)
    # Empty or unknown POS must not trigger core assertions in debug builds.
    for bad_pos in ("", " ", "SS", "?"):
        bad = copy.deepcopy(payload)
        bad["annotations"]["tokens"][1]["features"]["mrf"][1]["pos"] = bad_pos
        invalid_payloads.append(bad)
    for invalid in invalid_payloads:
        request(base_url, "/process", invalid, status=400)
        assert request(base_url, "/process", payload) == baseline, invalid

    # Unknown words have no analyses. Empty documents and sentences are valid.
    for unknown in (token("tundmatusõna"), {"features": {"token": "tundmatusõna"}}):
        result = request(base_url, "/process", sentence_payload(unknown))
        assert result["annotations"]["tokens"][0]["features"]["token"] == "tundmatusõna"
        assert not result["annotations"]["tokens"][0]["features"].get("mrf")
        assert request(base_url, "/process", payload) == baseline
    for empty in (sentence_payload(), {"annotations": {"tokens": [], "sentences": []}}):
        result = request(base_url, "/process", empty)
        assert not result["annotations"]["tokens"]
        assert request(base_url, "/process", payload) == baseline

    def parallel_request(index):
        if index % 2:
            assert request(base_url, "/process", version_payload) == with_version
        else:
            assert request(base_url, "/process", payload) == baseline

    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as pool:
        list(pool.map(parallel_request, range(32)))
    assert request(base_url, "/process", payload) == baseline
    assert request(base_url, "/health")["status"] == "ok"


def check_startup_defaults(base_url):
    payload = road_payload()
    baseline = request(base_url, "/process", payload)
    assert baseline["version_vmetyjson"] == request(base_url, "/version")["version_vmetyjson"]
    assert preferred(baseline, 1) == {("tee", "S", "sg n")}
    # Explicit request flags replace defaults; the following request restores them.
    override = request(base_url, "/process", {**payload, "params": {"vmetyjson": []}})
    assert "version_vmetyjson" not in override
    assert preferred(override, 1) == preferred(baseline, 1)
    assert request(base_url, "/process", payload) == baseline
    request(base_url, "/process", {
        **payload, "params": {"vmetyjson": ["--unknown"]},
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
    if not (dictionary_dir / "et3.dct").is_file():
        parser.error("Dictionary directory does not contain et3.dct: {}".format(dictionary_dir))
    with running_server(binary, dictionary_dir) as base_url:
        check_default_server(base_url)
    with running_server(binary, dictionary_dir, "--version", "--formattedjson", "--utf8json") as base_url:
        check_startup_defaults(base_url)
    print("HTTP integration checks passed (including disambiguation, request isolation and concurrent requests).")


if __name__ == "__main__":
    main()
