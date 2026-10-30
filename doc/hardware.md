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
presence and is covered by native regression tests. The supplied capture predates
that fix and does not verify it on hardware.

Not established by this test: a full 90-second capture, device firmware identity,
serial-disconnect recovery, installer/reset operations, long-term stability, or
wall-controller proxy operation. Do not use this evidence to claim those checks
were completed or to attest the release workflow's display-proxy hardware gate.
