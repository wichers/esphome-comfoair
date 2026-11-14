# v1.1.0

Climate fixes, Home Assistant entity metadata improvements, and automatic
version-gated releases for the ComfoAir ESPHome external component.

## Changes and migration

- The climate HVAC mode is always **Fan only**; **Quiet** selects Absent.
  Fans continue at their configured absent percentages. Update automations and
  dashboards that use HVAC mode `off` or fan mode `off` to `fan_only` and `quiet`.
- Target temperature stays unknown until readback; comfort temperature commands
  round to the nearest 0.5°C. Unit versions are logged when replies arrive.
- Entity names use standard ESPHome validation. Entities gain appropriate icons,
  device classes and diagnostic categories; YAML overrides still take precedence.
- Operating-hour counters use `total_increasing`; prior statistics remain intact.
- `filter_warning_weeks` uses `w`. Resolve Home Assistant's Repairs prompt by
  updating the historic unit (1 `weeks` = 1 `w`).
- A change to `VERSION` on master now builds and publishes a release after CI.

Read [the changelog](https://github.com/wichers/esphome-comfoair/blob/v1.1.0/CHANGELOG.md)
and [control behavior](https://github.com/wichers/esphome-comfoair/blob/v1.1.0/doc/controls.md)
for details and migration guidance.

## Validation and limits

The release workflow requires the baseline ESPHome 2026.8.1/2026.10.0 checks,
including firmware compilation for ESP32-S3/ESP-IDF, ESP32/ESP-IDF,
ESP32/Arduino and ESP8266, plus the native protocol harness. These are software
checks, not physical-device verification.

No new hardware validation is claimed for 1.1.0. Historical 1.0.0 smoke tests on
ComfoAir 350 with ESP32-S3/ESP-IDF and ESPHome 2026.10.1 confirmed polling,
communication, Absent-mode control and missing enthalpy-sensor handling. The
unit firmware was unknown; captures lasted approximately 81 and 73 seconds.
See [recorded evidence](https://github.com/wichers/esphome-comfoair/blob/v1.1.0/doc/hardware.md).

**Hardware-unverified for 1.1.0:** changes in this release, wall-controller proxy,
serial-disconnect recovery, installer/reset controls and long-term operation.

Install at `ref: v1.1.0`, then rebuild and upload using your own board, UART pins
and secrets. No universal firmware binary is provided.
