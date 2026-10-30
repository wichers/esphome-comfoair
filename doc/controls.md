# Controls and communication health

The climate entity displays values reported by the ventilation unit. Changing a
fan mode or comfort temperature sends a command and schedules a readback on the
next available two-second poll slot. Rejected, expired, or ignored commands do
not change the displayed device state. Filter/error resets are not retried
automatically.

Climate fan **Off** means the unit's **Absent** ventilation level. Fans may
continue running at the configured absent percentages. It is not a guarantee
that the fans or ventilation unit are electrically off.

## Installer settings

Fan-percentage numbers and the unit-size select belong to the configuration
category and are disabled by default in Home Assistant. Enable them only when
needed. Examples omit the unit-size control entirely.

Changing one fan percentage preserves the seven other values read from the
device, including values whose number entities are not configured. Writes are
blocked until a complete valid ventilation response arrives. Controls accept
integer percentages from 15 to 95; optional UI min/max limits must stay inside
that range. `initial_value` is deliberately unsupported: firmware must not replace
installer calibration with YAML defaults. The sync button resends the cached
settings; normal number changes already send their change.

Unit-size writes similarly require a status response. Both fan tuning and unit
size are shown as changed only after a device response confirms them. Queued
commands expire after five seconds and log a warning; the component then reads
the actual settings. Do not use these controls as part of routine automations.

## Communication status

Optional diagnostics under `comfoair`:

```yaml
connected:
  name: Ventilation communication
response_age:
  name: Ventilation response age
communication_timeout: 90s
```

Only a supported response with a valid frame and sufficient payload refreshes
communication health. Bytes, ACKs, unknown messages, and short responses do not.
Health is checked once per second. The default timeout allows several regular
poll cycles; allow additional time if a busy wall controller delays ESP polls.

On timeout the component raises a warning, reports `connected: false`, publishes
unknown (`NaN`) for numeric measurements and climate temperatures, and blocks
installer writes until their device baselines are received again. Binary/text
entities retain their last reported values: gate automations using them on
`connected`, particularly fault/filter/preheating automations. MQTT consumers
must treat non-finite numeric values as unavailable and also check connectivity.
ESP network availability alone does not establish ventilation-unit availability.

The two `motor_current_*` sensors are raw ADC counts, not amperes. Their entity
keys stay the same, but they have no current device class or physical unit.
No unverified ADC-to-current conversion is applied.
