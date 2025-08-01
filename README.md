# ComfoAir
Port of the ComfoAir protocol to ESPHome.io firmware, which is supported by external_components.

Add the following definition of `external_components` to your yaml configuration:

```
external_components:
  - source:
      type: git
      url: https://github.com/wichers/esphome-comfoair
    components: [comfoair]
```
and then use it:
```
uart:
  id: uart_bus
  rx_pin: 3
  tx_pin: 1
  baud_rate: 9600

comfoair:
  name: "ComfoAir 350"
  uart_id: uart_bus
  type:
    name: "type"
  size:
    name: "size"
  intake_fan_speed:
    name: "intake_fan_speed"
  exhaust_fan_speed:
    name: "exhaust_fan_speed"
  intake_fan_speed_rpm:
    name: "intake_fan_speed_rpm"
  exhaust_fan_speed_rpm:
    name: "exhaust_fan_speed_rpm"
  ventilation_level:
    name: "ventilation_level"
  bypass_present:
    name: "bypass_present"
  bypass_valve:
    name: "bypass_valve"
  bypass_valve_open:
    name: "bypass_valve_open"
  preheating_state:
    name: "preheating_state"
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
  filter_status:
    name: "filter_status"
  enthalpy_present:
    name: "enthalpy_present"
  ewt_present:
    name: "ewt_present"
  options_present:
    name: "options_present"
  fireplace_present:
    name: "fireplace_present"
  kitchen_hood_present:
    name: "kitchen_hood_present"
  postheating_present:
    name: "postheating_present"
  postheating_pwm_mode_present:
    name: "postheating_pwm_mode_present"
  preheating_present:
    name: "preheating_present"
  bypass_factor:
    name: "bypass_factor"
  bypass_step:
    name: "bypass_step"
  bypass_correction:
    name: "bypass_correction"
  bypass_open_hours:
    name: "bypass_open_hours"
  preheating_hours:
    name: "preheating_hours"
  level0_hours:
    name: "level0_hours"
  level1_hours:
    name: "level1_hours"
  level2_hours:
    name: "level2_hours"
  level3_hours:
    name: "level3_hours"
  preheating_valve:
    name: "preheating_valve"
  frost_protection_active:
    name: "frost_protection_active"
  frost_protection_hours:
    name: "frost_protection_hours"
  frost_protection_minutes:
    name: "frost_protection_minutes"
  frost_protection_level:
    name: "frost_protection_level"
  filter_hours:
    name: "filter_hours"
  motor_current_bypass:
    name: "motor_current_bypass"
  motor_current_preheating:
    name: "motor_current_preheating"
  summer_mode:
    name: "summer_mode"
  p10_active:
    name: "p10_active"
  p11_active:
    name: "p11_active"
  p12_active:
    name: "p12_active"
  p13_active:
    name: "p13_active"
  p14_active:
    name: "p14_active"
  p15_active:
    name: "p15_active"
  p16_active:
    name: "p16_active"
  p17_active:
    name: "p17_active"
  p18_active:
    name: "p18_active"
  p19_active:
    name: "p19_active"
  p90_active:
    name: "p90_active"
  p91_active:
    name: "p91_active"
  p92_active:
    name: "p92_active"
  p93_active:
    name: "p93_active"
  p94_active:
    name: "p94_active"
  p95_active:
    name: "p95_active"
  p96_active:
    name: "p96_active"
  p97_active:
    name: "p97_active"
  bathroom_switch_on_delay_minutes:
    name: "bathroom_switch_on_delay_minutes"
  bathroom_switch_off_delay_minutes:
    name: "bathroom_switch_off_delay_minutes"
  l1_switch_off_delay_minutes:
    name: "l1_switch_off_delay_minutes"
  boost_ventilation_minutes:
    name: "boost_ventilation_minutes"
  filter_warning_weeks:
    name: "filter_warning_weeks"
  rf_high_time_short_minutes:
    name: "rf_high_time_short_minutes"
  rf_high_time_long_minutes:
    name: "rf_high_time_long_minutes"
  extractor_hood_switch_off_delay_minutes:
    name: "extractor_hood_switch_off_delay_minutes"
```

The sensor defined here is a full list of sensors - if you remove a sensor from the yaml definition it will not be monitored.

For visualization:
Checkout https://github.com/wichers/lovelace-comfoair and follow the instructions.

## Compatibility

This is a list of models that are known to work, feel free to add yours if you tested it successfully:

* Zehnder WHR 930
* Zehnder WHR 950
* Zehnder ComfoAir 160
* Zehnder ComfoAir 200
* Zehnder Comfoair 550

## Support my work
Thank you for thinking about supporting my work.

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/wichers)
