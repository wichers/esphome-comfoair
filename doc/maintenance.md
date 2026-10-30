# Maintenance priorities

## Implemented fixes

- UART final validation replaces the deprecated C++ runtime check. The
  installed ESPHome 2026.10.0 API accepts all specified keywords. Its
  `uart.h` marks the old function deprecated since 2026.9.0, with removal in
  2027.3.0. Incorrect UART YAML now intentionally fails before compilation.
- Issue #97: removing `size_select`'s implicit empty default prevents creation
  of an entity without a name or ID. Existing codegen skips absent entities,
  and C++ initializes the select pointer to null and checks it before use.
- Configuration/codegen regression tests for all 87 optional entities, native
  tests of the real C++ serial implementation and every decoder dispatch branch,
  climate command tests, and a version/hardware firmware CI matrix.
- `to_code` uses `async`/`await`. Shared measurement and fan-number schema
  builders preserve entity metadata and defaults. Before/after full-entity
  schema and generated C++ SHA-256 hashes are identical.
- Invalid receive frames are no longer indiscriminately logged as checksum
  failures; the message also identifies length and payload-limit failures.
- A real ESP32-S3 installation was updated by OTA using its existing YAML,
  unchanged Wi-Fi/UART/PSRAM/BLE/API settings and local component sources.
  A passive 90-second post-update test found the same 94 entity identities,
  all ten regular poll types, live sensor updates and no ComfoAir warnings.
  Climate mode/target and ventilation level matched the pre-update baseline.
  No reset, fan-level or installer-size commands were sent.

API references:

- [ESPHome final validation](https://developers.esphome.io/architecture/components/)
- [UART component architecture](https://developers.esphome.io/architecture/components/uart/)
- [UART Python API source](https://github.com/esphome/esphome/blob/2026.10.0/esphome/components/uart/__init__.py)

## Current priorities

- Run all PR checks before merging and keep the supported ESPHome pins current.
- Validate v1.0.0 on an actual unit and a separate wall-controller installation.
  Record model/firmware, serial timing, entity identities and passive readings.
- Supplement synthetic fixtures with annotated captured traffic; no captured
  fixtures have been supplied yet. See CONTRIBUTING.md for the evidence format.
- Observe longer-term operation and Home Assistant/MQTT availability behavior.
- Ask the reporters of issues #40 and #54 to verify tagged fan/proxy behavior
  before closing those reports; do not assume every hardware variant is fixed.

## Historical implementation notes

The following PRs were separate from the original UART-validation work and have
since merged: #94 (Auto ventilation and zero fan-period handling), #95 (fault bits
and reset buttons), #96 (second-UART display proxy), and #100 (sensor metadata).
Their earlier exclusion is historical, not the current support status.

See controls.md for confirmed controls, tuning safeguards and communication
health, and releases.md for the v1.0.0 release gate and ongoing versioning.
