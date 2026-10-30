# Tests

Run from the repository root with ESPHome installed:

```sh
python -m unittest discover -s tests -v
esphome config tests/esp32-s3.yaml
esphome compile tests/esp32-s3.yaml
esphome compile tests/protocol.yaml
tests/.esphome/build/comfoair-protocol/.pioenvs/comfoair-protocol/program
```

The Python tests invoke the real ESPHome CLI in temporary directories. They
cover issue #97 (no implicit size select), explicit name/ID-only selects,
code generation for all 87 optional component entities, fan-number defaults,
and rejection of each invalid UART setting or missing pin.

`protocol.yaml` builds a native host executable using the actual component and
ESPHome classes, not a reimplementation of the parser. A fake UART captures TX
and supplies RX. Checks stay enabled with `NDEBUG`. It exits successfully only
after the checks run; it does not need or contact a ventilation unit.

Protocol coverage:

- Independently specified checksum and wire bytes, including escaped `0x07`.
- Valid/invalid checksums and raw/escaped checksum `0x07`.
- Incomplete frames, length mismatch, oversized payloads, and resynchronization.
- Maximum escaped payload/checksum (69 wire bytes in the 70-byte buffer).
- Repeated buffer wraps, index bounds, and guard bytes around the RX buffer.
- Incoming ACKs, ACK transmission for valid messages, and no ACK for invalid
  or incomplete messages or incoming ACKs.

Decoder fixtures exercise every response dispatch branch: version data, fans
(including zero-period RPM), valves, bypass control, both temperature formats,
ventilation levels/numbers, faults, status bits, hours, preheating and delays.
They check presence masks and unknown binary/text values as well as climate
fan commands for all five modes and the encoded target temperature. These
are synthetic fixtures from `registers.h`, not captured device traffic.

Fault fixtures decode A, E, EA and high-A bits (including A9, A15 and A0) with
and without the optional bytes. A valid frame with a short fault payload goes
through the receive loop and must not change any state. The tests also check
the two reset-button payloads and that `error_status` alone polls the faults.

The host harness targets ESPHome 2026.10.0's UART C++ API. Configuration and
firmware CI separately target 2026.8.1 and 2026.10.0, covering ESP32-S3/ESP-IDF,
ESP32/ESP-IDF, ESP32/Arduino, ESP8266, and an entity-minimal configuration.
All fixtures use local component sources, so CI tests the PR rather than master.
The build job fails on a deprecated `check_uart_settings` warning and retains
build logs. Version pins are deliberate: update them when testing new releases.

These are build and software tests, not a physical UART/timing or Home Assistant
integration test. They do not prove every decoded sensor value or climate control
command against real hardware. A passive OTA smoke test must use the actual
installation YAML and secrets, not these fixtures. Before uploading, verify the
board, flash/PSRAM settings, UART pins, live Wi-Fi SSID and target address;
record entity identities and sensor readings, then compare after reboot without
operating reset/installer controls.
