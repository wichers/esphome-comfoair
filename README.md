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
    name: "Fan supply (%)"
  fan_exhaust_air_percentage:
    name: "Fan exhaust (%)"
  fan_speed_supply:
    name: "Supply fan speed"
  fan_speed_exhaust:
    name: "Exhaust fan speed"
  is_bypass_valve_open:
    name: "Is bypas open?"
  is_preheating:
    name: "is preheating active?"
  outside_air_temperature:
    name: "Outside temperature"
  supply_air_temperature:
    name: "Supply temperature"
  return_air_temperature:
    name: "Return temperature"
  exhaust_air_temperature:
    name: "Exhaust temperature"
  enthalpy_temperature:
    name: "Enthalpy temperature"
  ewt_temperature:
    name: "EWT temperature "
  reheating_temperature:
    name: "Reheating temperature"
  kitchen_hood_temperature:
    name: "Kitchen hood temperature"
  return_air_level:
    name: "Return level"
  supply_air_level:
    name: "Supply level"
  is_supply_fan_active:
    name: "Is supply fan active?"
  is_filter_full:
    name: "Is filter full?"
  bypass_factor:
    name: "Bypass factor"
  bypass_step:
    name: "Bypass step"
  bypass_correction:
    name: "Bypass correction"
  is_summer_mode:
    name: "Is summer mode?"
```

The sensor defined here is a full list of sensor - if you remove sensor from yaml it will be not monitored.


For visualization: 

Have a look at simple_thermostat.yaml and https://github.com/nervetattoo/simple-thermostat

Or checkout https://github.com/wichers/lovelace-comfoair and follow the instructions.
