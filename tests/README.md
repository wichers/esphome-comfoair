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
code generation, and rejection of each invalid UART setting or missing pin.

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

The host harness targets ESPHome 2026.10.0's UART C++ API. Configuration and
firmware CI separately target 2026.8.1 and 2026.10.0, covering ESP32-S3/ESP-IDF,
ESP32/ESP-IDF, ESP32/Arduino, ESP8266, and an entity-minimal configuration.
All fixtures use local component sources, so CI tests the PR rather than master.
The build job fails on a deprecated `check_uart_settings` warning and retains
build logs. Version pins are deliberate: update them when testing new releases.

These are build and software tests, not a physical UART/timing or Home Assistant
integration test. They do not prove every decoded sensor value or climate control
command against real hardware.
