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

## Follow-up work (not mixed into the fixes)

| Priority | Improvement | Risk / verification needed |
| --- | --- | --- |
| P1 | Run the new GitHub CI matrix before merging | Local success does not establish Linux runner/toolchain compatibility. Version pins need maintenance. |
| P2 | Supplement synthetic decoder fixtures with captured traffic | Every dispatch branch is exercised, but synthetic samples do not prove every hardware/firmware variant. |
| P2 | Longer-term runtime and Home Assistant UI observation | The real OTA smoke test uses the native API and passive polling; it does not operate physical controls or establish long-term uptime. |

The standard UART validator already identifies the component, required setting
and referenced UART. Custom validation/error wrappers would duplicate ESPHome
behavior and are not needed.

## Open pull requests checked before implementation

- #96: optional second UART/ComfoSense proxy; intentionally excluded. It modifies
  the same Python and C++ files, so rebasing and rerunning protocol tests is
  necessary if it merges.
- #95: fault bits and reset buttons; separate features, overlapping files.
- #94: Auto ventilation level and zero fan-period handling; separate C++ fix.

No remote branches or pull requests are created by these local commits, and no
open PR is merged or modified.
