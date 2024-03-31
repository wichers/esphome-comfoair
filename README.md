# ComfoAir
Port of ComfoAir protocol to ESPHome.io firmware supported by external_components.

Add to your yaml configuration the definition of `external_components`:
```
external_components:
  - source:
      type: git
      url: https://github.com/wichers/esphome-comfoair
    components: [comfoair]
```
and than use it:
```
uart:
  id: uart_bus
  rx_pin: 3
  tx_pin: 1
  baud_rate: 9600

comfoair:
  name: "ComfoAir 350"
  uart_id: uart_bus
  fan_supply_air_percentage:
    name: "fan_supply_air_percentage"
  fan_exhaust_air_percentage:
    name: "fan_exhaust_air_percentage"
  fan_speed_supply:
    name: "fan_speed_supply"
  fan_speed_exhaust:
    name: "fan_speed_exhaust"
  bypass_valve_open:
    name: "bypass_valve_open"
  preheating:
    name: "preheating"
  outside_air_temperature:
    name: "outside_air_temperature"
  supply_air_temperature:
    name: "supply_air_temperature"
  return_air_temperature:
    name: "return_air_temperature"
  exhaust_air_temperature:
    name: "exhaust_air_temperature"
  enthalpy_temperature:
    name: "enthalpy_temperature"
  ewt_temperature:
    name: "ewt_temperature"
  reheating_temperature:
    name: "reheating_temperature"
  kitchen_hood_temperature:
    name: "kitchen_hood_temperature"
  return_air_level:
    name: "return_air_level"
  supply_air_level:
    name: "supply_air_level"
  supply_fan_active:
    name: "supply_fan_active"
  filter_full:
    name: "filter_full"
  bypass_factor:
    name: "bypass_factor"
  bypass_step:
    name: "bypass_step"
  bypass_correction:
    name: "bypass_correction"
  summer_mode:
    name: "summer_mode"
```

The sensor defined here is a full list of sensor - if you remove sensor from yaml it will be not monitored.


For visualization:

Have a look at simple_thermostat.yaml and https://github.com/nervetattoo/simple-thermostat

Or checkout https://github.com/wichers/lovelace-comfoair and follow the instructions.
