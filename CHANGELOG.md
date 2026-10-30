# Changelog

Published versions use immutable Git tags and GitHub Releases. This file records
component changes; ESPHome itself has a separate version.

## [1.0.0]

First tagged release, prepared from the existing component and its review fixes.
It is not published until the release workflow succeeds on merged `master`.

### Fixed

- Escaped payloads containing marker bytes and command-specific short responses.
- Proxy ownership across an intermediate ACK and its matching read response.
- Climate current temperature without an optional temperature sensor.
- Preservation of device fan calibration when only one tuning number is exposed.
- Installer writes before device settings arrive or after communication is lost.
- Displayed controls now use device-confirmed values with prompt readback.
- Polling when only motor-current sensors are configured.
- Fan/bypass units and device classes, including the previously merged PR #100.
- Motor-current values are identified as raw ADC counts, not amperes.
- An absent enthalpy temperature sensor reports unavailable instead of -20°C.
  Presence value 2 (exchanger without sensor) is also handled.

### Added

- Optional serial `connected` and `response_age` diagnostics, a 90-second default
  communication timeout, and invalidation of numeric readings on timeout.
- Regression coverage for escaped markers, every short decoder payload,
  ACK/reply timing, confirmed controls, partial tuning configurations and timeout
  recovery. The native CI harness runs with address/undefined-behavior sanitizers.
- Local-source validation of public examples, a scheduled latest-stable ESPHome
  check, dependency/build caches, and a manual release workflow gated on CI.
- Authenticated examples using secrets, pinned component sources, contribution
  and issue templates, and a documented hardware smoke-test/release procedure.

### User actions

Development tooling: Pylint is pinned to 4.0.9, which fixes the stats-cache pickle
deserialization advisory reported by Dependabot. Reinstall `requirements-dev.txt`
when linting locally; this dependency is not included in device firmware.

- Move installation sources to `ref: v1.0.0` after publication; use `refresh: never`
  for immutable tags. Updating source does not flash a device automatically.
- Replace placeholders in `secrets.example.yaml` with unique credentials. Web
  examples now require authentication; native API examples require encryption.
- Omit obsolete fan-number `initial_value`: settings come from the device.
  Exposed tuning numbers and the installer-size select are disabled by default
  in Home Assistant. Enable them deliberately when needed.
- Fan levels formerly labelled m³ now use %. Home Assistant may require updating
  or deleting their old long-term statistics. Raw motor-current sensors also
  lose the A unit; review their statistics and any automations using that unit.
- Climate Off means Absent, whose fans can still run. Gate binary/text-based
  automations on serial `connected`; those entities retain their last values
  during a communication outage.

### Compatibility evidence

Baseline CI targets ESPHome 2026.8.1 and 2026.10.0 on ESP32-S3/ESP-IDF,
ESP32/ESP-IDF, ESP32/Arduino and ESP8266. The native host harness targets
2026.10.0. Builds and synthetic fixtures do not establish physical timing or
model-specific behavior; hardware evidence must be recorded before publication.
