# v1.0.0

First versioned release of the ComfoAir ESPHome external component. It includes
serial parser and proxy transaction fixes, device-confirmed controls, protected
fan calibration, corrected sensor metadata, communication health diagnostics,
and authenticated installation examples pinned to a release tag.

Before upgrading, read [the changelog](https://github.com/wichers/esphome-comfoair/blob/v1.0.0/CHANGELOG.md) and
[control/availability behavior](https://github.com/wichers/esphome-comfoair/blob/v1.0.0/doc/controls.md). Remove the unused fan-number
`initial_value` setting. Enable advanced configuration entities deliberately.
Review Home Assistant statistics for fan-level changes from m³ to % and for raw
motor-current values losing the A unit. Off still means the unit's Absent level.

The baseline test matrix covers ESPHome 2026.8.1 and 2026.10.0, ESP32-S3 and ESP32
with ESP-IDF, ESP32 with Arduino, and ESP8266. Software fixtures are synthetic.

Live testing on a **Zehnder ComfoAir 350**, ESP32-S3/ESP-IDF and ESPHome 2026.10.1
confirmed regular serial polling, healthy communication, expected Home Assistant
Absent-mode control and unavailable enthalpy temperature when its sensor is
absent. The ventilation unit's own firmware version is unknown. Two supplied
captures lasted approximately 81 and 73 seconds; these are short smoke tests,
not evidence of long-term stability.

**Hardware-unverified:** wall-controller proxy operation, serial-disconnect
recovery, installer/reset controls and long-term operation. Proxy and recovery
have software regression coverage; they have not been confirmed on physical
hardware for this release. See
[hardware evidence](https://github.com/wichers/esphome-comfoair/blob/v1.0.0/doc/hardware.md) for the observation details.

Install the source at `ref: v1.0.0`, then rebuild and upload using your own board,
UART pins and secrets. No universal firmware binary is provided.
