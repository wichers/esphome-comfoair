# Hardware evidence

## Zehnder ComfoAir 350 — 30 October 2026

User-provided logs from an ESP32-S3 running ESPHome 2026.10.1 and component
commit `c41b4673466a1c2abe1a00157f278d8edf2e90ca` cover approximately 81 seconds,
14:31:13–14:32:34 local time. The user identified the unit as a ComfoAir 350;
its own firmware version is unknown. Initial 0.00 version messages were printed
before version requests and are not firmware identification.

Observed:

- Serial communication remained online after the first valid response.
- Once regular polling started, response age stayed approximately 0–1.5 seconds.
- All ten regular poll types returned data; no serial/parser timeout or error
  messages appeared in the supplied interval.
- Intake/exhaust fans reported 15% and approximately 615–622 RPM.
- Outside/supply/return/exhaust temperatures reported 16/18/20.5/17.5°C.
- Fault status was None and filter status was Ok.
- Device-reported fan calibration was 15/30/50/95% on both sides.
- A level-1 (Absent) command appeared at startup. The user confirmed that Home
  Assistant automation intentionally requested it and that this behavior is
  correct. This was not a wholly passive test.

The log exposed a decoder issue: enthalpy presence was OFF while its temperature
was published as -20°C. The follow-up fix gates the reading on confirmed sensor
presence and is covered by native regression tests. This first capture predates
that fix; the follow-up test below verifies its behavior on the same installation.

Not established by this test: a full 90-second capture, device firmware identity,
serial-disconnect recovery, installer/reset operations, long-term stability, or
wall-controller proxy operation. Do not use this evidence to claim those checks
were completed.

## Follow-up: enthalpy fix — 30 October 2026

The user installed the test YAML pinned to merged commit
`a1ba20362b8fe3a79d9575feff186d0318ce0eae`. Logs from the same ComfoAir 350,
ESP32-S3/ESP-IDF and ESPHome 2026.10.1 cover approximately 73 seconds,
14:47:58–14:49:11 local time, including two complete regular poll cycles.

- `enthalpy_present` was OFF and `enthalpy_temperature` consistently reported
  NaN/unavailable, confirming the fix on hardware.
- Communication stayed ON after initialization; response age was approximately
  0.5–1.9 seconds during regular polling.
- All ten regular poll types returned data. No serial errors, timeouts or unit
  faults appeared in the supplied interval; filter status was Ok.
- Intake/exhaust remained at 15%, approximately 615–623 RPM; recorded tuning
  remained 15/30/50/95% on both sides.
- Temperatures were outside 16.5°C, supply 18.5°C, return 20.5°C and exhaust 18°C.
- The expected Home Assistant automation requested Absent at startup.

The unit firmware remains unknown. This verifies the installed direct-UART
setup and the enthalpy fix, not a 90-second continuous observation,
disconnect recovery, installer/reset commands, display-proxy behavior or
long-term stability. Those limits must remain explicit in release notes.
