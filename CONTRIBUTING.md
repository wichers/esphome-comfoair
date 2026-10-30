# Contributing

Keep pull requests focused on one behavior or maintenance task. Start from the
latest `master`; use a local external-component source so tests exercise your
change. Include the problem, resulting behavior, and checks performed in the PR.

Install the pinned development tools in an isolated Python 3.12 environment:

```sh
python -m venv .venv
.venv/bin/python -m pip install -r requirements-dev.txt
.venv/bin/python -m unittest discover -s tests -v
.venv/bin/python -m pylint components tests .github/scripts
.venv/bin/python -m ruff check components tests .github/scripts
```

Run the repository tests once after implementation is complete. Add a focused
regression for changed behavior, especially parser escapes, payload lengths,
optional entities, control confirmation and proxy timing. Format only changed
C/C++ files at the end; use `.clang-format`. CI performs firmware builds across
supported ESPHome versions and frameworks; a local full build is optional.

Never commit installation secrets or generated build files. Example tests use
temporary dummy credentials and do not contact devices. Test the actual shipped
examples as well as minimal fixtures when changing configuration or examples.

For new model evidence, attach a small redacted serial capture with direction,
relative timing, model and firmware version, and expected decoded values. Keep
the original wire bytes, including escaping/checksum/ACKs. Add them as regression
fixtures in the native harness; do not describe synthetic frames as captures.

Before reporting a bug, include the component tag/commit, ESPHome version, board,
unit model/firmware, proxy use, redacted YAML, and the relevant DEBUG log. Never
attach Wi-Fi/API/OTA/MQTT credentials or unredacted secrets.yaml.

Maintainers: require passing ESPHome checks before merging. Contributors without
repository administration access cannot configure branch rules. See
[release instructions](doc/releases.md) for versioning and publication.
