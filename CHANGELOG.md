# Changelog

Published versions use immutable Git tags and GitHub Releases. This file records
component changes; ESPHome itself has a separate version.

## [Unreleased]

## [1.1.0]

### Added

- Automatic releases on master when VERSION changes, gated on metadata validation
  and the existing CI/firmware builds. Release PRs record hardware evidence and limits.

### Fixed

- The climate entity no longer reports mode Off, which it did not advertise as
  supported. The mode is always Fan only, including before the first readback.
- Absent is now the Quiet fan mode instead of Off: the fans keep running at the
  absent percentages. Commands that still send fan mode Off select Absent.
- `filter_warning_weeks` uses Home Assistant's week unit `w` instead of `weeks`.
- The climate target temperature is unknown until the unit reports it. It was
  uninitialised memory (usually 0°C) in the first publication after boot.
- A comfort temperature is rounded to the nearest 0.5°C step before sending
  instead of being truncated (21.49 now sends 21.5, not 21.0).
- Unit versions are logged when they arrive. At boot the configuration dump ran
  before the replies and printed zeros.

### Changed

- The climate `name` uses ESPHome's standard entity-name validation and is no
  longer required. `name: None` now uses the device's friendly name instead of
  creating an entity called "None"; names containing `/` get ESPHome's warning.
  Existing names register the same entity, so Home Assistant entity IDs stay.
- Removed the unused `set_name()` overrides and `name` member.
- The unit-size select implements ESPHome's preferred index-based `control()`
  instead of the string overload, avoiding a string lookup per change.
- Entities have default icons, and HA device classes where one fits: `duration`
  for the hour and minute values and `response_age`; `opening` for
  `bypass_valve_open`; `problem` for `filter_status_full`; `running` for
  `supply_fan_active`; `cold` for `frost_protection_active`; `heat` for
  `preheating_state` and `preheating_active`. Icons, device classes and
  categories set in YAML still take precedence.
- Operating-hour counters use state class `total_increasing` instead of
  `measurement`, because they only count up.
- `frost_protection_minutes` and `filter_warning_weeks` are diagnostic entities.

### User actions

- Automations and dashboards that check or set climate fan mode `off` or HVAC
  mode `off` for Absent must use fan mode `quiet` and HVAC mode `fan_only`.
- After the `filter_warning_weeks` unit change, Home Assistant pauses that
  sensor's long-term statistics and raises a Repairs issue. Choose to update the
  unit of the historic values to keep the history (1 `weeks` = 1 `w`).
- Binary sensors with a new device class show HA's matching state names, for
  example Open/Closed for `bypass_valve_open` and Problem/OK for
  `filter_status_full`. Automations compare the unchanged `on`/`off` states.
- The operating-hour counters' long-term statistics are kept as running totals
  from the update on; earlier statistics keep their min/max/mean values.
- `frost_protection_minutes` and `filter_warning_weeks` move to the device's
  Diagnostic section in Home Assistant.

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
