""" Creates module ComfoAir """

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, sensor, text_sensor, uart
from esphome.const import (CONF_ID, CONF_UART_ID, DEVICE_CLASS_CURRENT,
                           DEVICE_CLASS_EMPTY, DEVICE_CLASS_SPEED,
                           DEVICE_CLASS_TEMPERATURE, DEVICE_CLASS_VOLUME,
                           STATE_CLASS_MEASUREMENT, UNIT_AMPERE, UNIT_CELSIUS,
                           UNIT_CUBIC_METER, UNIT_HOUR, UNIT_MINUTE,
                           UNIT_PERCENT, UNIT_REVOLUTIONS_PER_MINUTE)

comfoair_ns = cg.esphome_ns.namespace("comfoair")
ComfoAirComponent = comfoair_ns.class_("ComfoAirComponent", cg.Component)

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor", "climate", "binary_sensor", "text_sensor"]
REQUIRED_KEY_NAME = "name"
CONF_HUB_ID = "comfoair"

UNIT_WEEK = "weeks"

CONF_TYPE = "type"
CONF_SIZE = "size"
CONF_INTAKE_FAN_SPEED = "intake_fan_speed"
CONF_EXHAUST_FAN_SPEED = "exhaust_fan_speed"
CONF_INTAKE_FAN_SPEED_RPM = "intake_fan_speed_rpm"
CONF_EXHAUST_FAN_SPEED_RPM = "exhaust_fan_speed_rpm"
CONF_VENTILATION_LEVEL = "ventilation_level"
CONF_PREHEATING_STATE = "preheating_state"
CONF_OUTSIDE_AIR_TEMPERATURE = "outside_air_temperature"
CONF_SUPPLY_AIR_TEMPERATURE = "supply_air_temperature"
CONF_RETURN_AIR_TEMPERATURE = "return_air_temperature"
CONF_EXHAUST_AIR_TEMPERATURE = "exhaust_air_temperature"
CONF_ENTHALPY_TEMPERATURE = "enthalpy_temperature"
CONF_EWT_TEMPERATURE = "ewt_temperature"
CONF_REHEATING_TEMPERATURE = "reheating_temperature"
CONF_KITCHEN_HOOD_TEMPERATURE = "kitchen_hood_temperature"
CONF_RETURN_AIR_LEVEL = "return_air_level"
CONF_SUPPLY_AIR_LEVEL = "supply_air_level"
CONF_SUPPLY_FAN_ACTIVE = "supply_fan_active"
CONF_FILTER_STATUS = "filter_status"
CONF_BYPASS_PRESENT = "bypass_present"
CONF_ENTHALPY_PRESENT = "enthalpy_present"
CONF_EWT_PRESENT = "ewt_present"
CONF_OPTIONS_PRESENT = "options_present"
CONF_FIREPLACE_PRESENT = "fireplace_present"
CONF_KITCHEN_HOOD_PRESENT = "kitchen_hood_present"
CONF_POSTHEATING_PRESENT = "postheating_present"
CONF_POSTHEATING_PWM_MODE_PRESENT = "postheating_pwm_mode_present"
CONF_PREHEATING_PRESENT = "preheating_present"
CONF_BYPASS_VALVE = "bypass_valve"
CONF_BYPASS_VALVE_OPEN = "bypass_valve_open"
CONF_BYPASS_FACTOR = "bypass_factor"
CONF_BYPASS_STEP = "bypass_step"
CONF_BYPASS_CORRECTION = "bypass_correction"
CONF_BYPASS_OPEN_HOURS = "bypass_open_hours"
CONF_MOTOR_CURRENT_BYPASS = "motor_current_bypass"
CONF_MOTOR_CURRENT_PREHEATING = "motor_current_preheating"
CONF_PREHEATING_HOURS = "preheating_hours"
CONF_PREHEATING_VALVE = "preheating_valve"
CONF_LEVEL0_HOURS = "level0_hours"
CONF_LEVEL1_HOURS = "level1_hours"
CONF_LEVEL2_HOURS = "level2_hours"
CONF_LEVEL3_HOURS = "level3_hours"
CONF_FROST_PROTECTION_ACTIVE = "frost_protection_active"
CONF_FROST_PROTECTION_HOURS = "frost_protection_hours"
CONF_FROST_PROTECTION_MINUTES = "frost_protection_minutes"
CONF_FROST_PROTECTION_LEVEL = "frost_protection_level"
CONF_FILTER_HOURS = "filter_hours"
CONF_SUMMER_MODE = "summer_mode"

CONF_P10_ACTIVE = "p10_active"
CONF_P11_ACTIVE = "p11_active"
CONF_P12_ACTIVE = "p12_active"
CONF_P13_ACTIVE = "p13_active"
CONF_P14_ACTIVE = "p14_active"
CONF_P15_ACTIVE = "p15_active"
CONF_P16_ACTIVE = "p16_active"
CONF_P17_ACTIVE = "p17_active"
CONF_P18_ACTIVE = "p18_active"
CONF_P19_ACTIVE = "p19_active"
CONF_P90_ACTIVE = "p90_active"
CONF_P91_ACTIVE = "p91_active"
CONF_P92_ACTIVE = "p92_active"
CONF_P93_ACTIVE = "p93_active"
CONF_P94_ACTIVE = "p94_active"
CONF_P95_ACTIVE = "p95_active"
CONF_P96_ACTIVE = "p96_active"
CONF_P97_ACTIVE = "p97_active"

CONF_BATHROOM_SWITCH_ON_DELAY_MINUTES = "bathroom_switch_on_delay_minutes"
CONF_BATHROOM_SWITCH_OFF_DELAY_MINUTES = "bathroom_switch_off_delay_minutes"
CONF_L1_SWITCH_OFF_DELAY_MINUTES = "l1_switch_off_delay_minutes"
CONF_BOOST_VENTILATION_MINUTES = "boost_ventilation_minutes"
CONF_FILTER_WARNING_WEEKS = "filter_warning_weeks"
CONF_RF_HIGH_TIME_SHORT_MINUTES = "rf_high_time_short_minutes"
CONF_RF_HIGH_TIME_LONG_MINUTES = "rf_high_time_long_minutes"
CONF_EXTRACTOR_HOOD_SWITCH_OFF_DELAY_MINUTES = "extractor_hood_switch_off_delay_minutes"

helper_comfoair = {
    "sensor": [
        CONF_INTAKE_FAN_SPEED,
        CONF_EXHAUST_FAN_SPEED,
        CONF_INTAKE_FAN_SPEED_RPM,
        CONF_EXHAUST_FAN_SPEED_RPM,
        CONF_VENTILATION_LEVEL,
        CONF_OUTSIDE_AIR_TEMPERATURE,
        CONF_SUPPLY_AIR_TEMPERATURE,
        CONF_RETURN_AIR_TEMPERATURE,
        CONF_EXHAUST_AIR_TEMPERATURE,
        CONF_ENTHALPY_TEMPERATURE,
        CONF_EWT_TEMPERATURE,
        CONF_REHEATING_TEMPERATURE,
        CONF_KITCHEN_HOOD_TEMPERATURE,
        CONF_RETURN_AIR_LEVEL,
        CONF_SUPPLY_AIR_LEVEL,
        CONF_BYPASS_VALVE,
        CONF_BYPASS_FACTOR,
        CONF_BYPASS_STEP,
        CONF_BYPASS_CORRECTION,
        CONF_BYPASS_OPEN_HOURS,
        CONF_MOTOR_CURRENT_BYPASS,
        CONF_MOTOR_CURRENT_PREHEATING,
        CONF_PREHEATING_HOURS,
        CONF_LEVEL0_HOURS,
        CONF_LEVEL1_HOURS,
        CONF_LEVEL2_HOURS,
        CONF_LEVEL3_HOURS,
        CONF_FROST_PROTECTION_HOURS,
        CONF_FROST_PROTECTION_MINUTES,
        CONF_FILTER_HOURS,
        CONF_BATHROOM_SWITCH_ON_DELAY_MINUTES,
        CONF_BATHROOM_SWITCH_OFF_DELAY_MINUTES,
        CONF_L1_SWITCH_OFF_DELAY_MINUTES,
        CONF_BOOST_VENTILATION_MINUTES,
        CONF_FILTER_WARNING_WEEKS,
        CONF_RF_HIGH_TIME_SHORT_MINUTES,
        CONF_RF_HIGH_TIME_LONG_MINUTES,
        CONF_EXTRACTOR_HOOD_SWITCH_OFF_DELAY_MINUTES,
    ],
    "binary_sensor": [
        CONF_BYPASS_PRESENT,
        CONF_ENTHALPY_PRESENT,
        CONF_EWT_PRESENT,
        CONF_OPTIONS_PRESENT,
        CONF_FIREPLACE_PRESENT,
        CONF_KITCHEN_HOOD_PRESENT,
        CONF_POSTHEATING_PRESENT,
        CONF_POSTHEATING_PWM_MODE_PRESENT,
        CONF_PREHEATING_PRESENT,
        CONF_BYPASS_VALVE_OPEN,
        CONF_PREHEATING_STATE,
        CONF_SUMMER_MODE,
        CONF_SUPPLY_FAN_ACTIVE,
        CONF_FROST_PROTECTION_ACTIVE,

        CONF_P10_ACTIVE,
        CONF_P11_ACTIVE,
        CONF_P12_ACTIVE,
        CONF_P13_ACTIVE,
        CONF_P14_ACTIVE,
        CONF_P15_ACTIVE,
        CONF_P16_ACTIVE,
        CONF_P17_ACTIVE,
        CONF_P18_ACTIVE,
        CONF_P19_ACTIVE,
        CONF_P90_ACTIVE,
        CONF_P91_ACTIVE,
        CONF_P92_ACTIVE,
        CONF_P93_ACTIVE,
        CONF_P94_ACTIVE,
        CONF_P95_ACTIVE,
        CONF_P96_ACTIVE,
        CONF_P97_ACTIVE,
    ],
    "text_sensor": [
        CONF_TYPE,
        CONF_SIZE,
        CONF_FILTER_STATUS,
        CONF_FROST_PROTECTION_LEVEL,
        CONF_PREHEATING_VALVE,
    ],
}

comfoair_sensors_schemas = cv.Schema(
    {
        cv.Optional(CONF_TYPE): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_SIZE): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_FILTER_STATUS): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_FROST_PROTECTION_LEVEL): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_PREHEATING_VALVE): text_sensor.text_sensor_schema(),

        cv.Optional(CONF_INTAKE_FAN_SPEED): sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_EXHAUST_FAN_SPEED): sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_INTAKE_FAN_SPEED_RPM): sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_REVOLUTIONS_PER_MINUTE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_EXHAUST_FAN_SPEED_RPM): sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_REVOLUTIONS_PER_MINUTE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_VENTILATION_LEVEL): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_OUTSIDE_AIR_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_SUPPLY_AIR_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_RETURN_AIR_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_EXHAUST_AIR_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_ENTHALPY_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_EWT_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_REHEATING_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_KITCHEN_HOOD_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_RETURN_AIR_LEVEL): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            unit_of_measurement=UNIT_CUBIC_METER,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_SUPPLY_AIR_LEVEL): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            unit_of_measurement=UNIT_CUBIC_METER,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_BYPASS_FACTOR): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_BYPASS_STEP): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_BYPASS_CORRECTION): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_BYPASS_OPEN_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_MOTOR_CURRENT_BYPASS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_CURRENT,
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_MOTOR_CURRENT_PREHEATING): sensor.sensor_schema(
            device_class=DEVICE_CLASS_CURRENT,
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),

        cv.Optional(CONF_PREHEATING_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_LEVEL0_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_LEVEL1_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_LEVEL2_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_LEVEL3_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_FROST_PROTECTION_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_FROST_PROTECTION_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_MINUTE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_FILTER_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),

        cv.Optional(CONF_BYPASS_VALVE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),

        cv.Optional(CONF_BATHROOM_SWITCH_ON_DELAY_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_BATHROOM_SWITCH_OFF_DELAY_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_L1_SWITCH_OFF_DELAY_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_BOOST_VENTILATION_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_FILTER_WARNING_WEEKS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_WEEK,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_RF_HIGH_TIME_SHORT_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_RF_HIGH_TIME_LONG_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_EXTRACTOR_HOOD_SWITCH_OFF_DELAY_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),

        cv.Optional(CONF_FROST_PROTECTION_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_BYPASS_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_ENTHALPY_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_EWT_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_OPTIONS_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_FIREPLACE_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_KITCHEN_HOOD_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_POSTHEATING_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_POSTHEATING_PWM_MODE_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_PREHEATING_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_BYPASS_VALVE_OPEN): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_PREHEATING_STATE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_SUMMER_MODE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_SUPPLY_FAN_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),

        cv.Optional(CONF_P10_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P11_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P12_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P13_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P14_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P15_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P16_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P17_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P18_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P19_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P90_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P91_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P92_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P93_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P94_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P95_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P96_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P97_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
    }
)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(ComfoAirComponent),
            cv.Required(REQUIRED_KEY_NAME): cv.string,
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(comfoair_sensors_schemas)
    .extend(cv.COMPONENT_SCHEMA)
)


def to_code(config):
    """Generates code"""
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)
    cg.add(var.set_name(config[REQUIRED_KEY_NAME]))
    paren = yield cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart_component(paren))
    for k, values in helper_comfoair.items():
        for v in values:
            if not v in config:
                continue
            sens = None
            if k == "sensor":
                sens = yield sensor.new_sensor(config[v])
            elif k == "binary_sensor":
                sens = yield binary_sensor.new_binary_sensor(config[v])
            elif k == "text_sensor":
                sens = yield text_sensor.new_text_sensor(config[v])
            if sens is not None:
                func = getattr(var, "set_" + v)
                cg.add(func(sens))
    cg.add(cg.App.register_climate(var))
