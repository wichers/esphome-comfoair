# v1.0.0

First versioned release of the ComfoAir ESPHome external component. It includes
serial parser and proxy transaction fixes, device-confirmed controls, protected
fan calibration, corrected sensor metadata, communication health diagnostics,
and authenticated installation examples pinned to a release tag.

Before upgrading, read [the changelog](CHANGELOG.md) and
[control/availability behavior](doc/controls.md). Remove the unused fan-number
`initial_value` setting. Enable advanced configuration entities deliberately.
Review Home Assistant statistics for fan-level changes from m³ to % and for raw
motor-current values losing the A unit. Off still means the unit's Absent level.

The baseline test matrix covers ESPHome 2026.8.1 and 2026.10.0, ESP32-S3 and ESP32
with ESP-IDF, ESP32 with Arduino, and ESP8266. Software fixtures are synthetic;
the release dispatcher must complete and record the hardware checks described
in [the release procedure](doc/releases.md).

Install the source at `ref: v1.0.0`, then rebuild and upload using your own board,
UART pins and secrets. No universal firmware binary is provided.
