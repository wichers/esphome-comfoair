#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/climate/climate_mode.h"
#include "esphome/components/climate/climate_traits.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace comfoair {

static const char *TAG = "comfoair";

class ComfoAirComponent : public climate::Climate, public PollingComponent, public uart::UARTDevice {
public:

  ComfoAirComponent();

  /// Return the traits of this controller.
  climate::ClimateTraits traits() override;

  /// Override control to change settings of the climate device.
  void control(const climate::ClimateCall &call) override;

  void dump_config() override;
  void update() override;
  void loop() override;

  float get_setup_priority() const override;

  void error_reset(void);
  void filter_reset(void);

  void set_name(const char* value);
  void set_uart_component(uart::UARTComponent *parent);

protected:

  void set_level_(int level);
  void set_comfort_temperature_(float temperature);
  void write_command_(const uint8_t command, const uint8_t *command_data, uint8_t command_data_length);
  uint8_t comfoair_checksum_(const uint8_t *command_data, uint8_t length) const;
  optional<bool> check_byte_() const;
  void parse_data_();

  void get_fan_status_();
  void get_valve_status_();
  void get_error_status_();
  void get_bypass_control_status_();
  void get_temperature_();
  void get_sensor_data_();
  void get_ventilation_level_();
  void get_temperatures_();
  void get_operation_hours_();
  void get_preheating_status_();
  void get_time_delay_();

  uint8_t get_uint8_t_(uint8_t start_index) const;
  uint16_t get_uint16_(uint8_t start_index) const;

  uint8_t data_[30];
  uint8_t data_index_{0};
  int8_t update_counter_{-4};
  const int8_t num_update_counter_elements_{9};

  uint8_t bootloader_version_[13]{0};
  uint8_t firmware_version_[13]{0};
  uint8_t connector_board_version_[14]{0};
  const char* name{0};

public:
  text_sensor::TextSensor *type{nullptr};
  text_sensor::TextSensor *size{nullptr};
  text_sensor::TextSensor *filter_status{nullptr};
  text_sensor::TextSensor *frost_protection_level{nullptr};
  text_sensor::TextSensor *preheating_valve{nullptr};
  sensor::Sensor *intake_fan_speed{nullptr};
  sensor::Sensor *exhaust_fan_speed{nullptr};
  sensor::Sensor *intake_fan_speed_rpm{nullptr};
  sensor::Sensor *exhaust_fan_speed_rpm{nullptr};
  sensor::Sensor *ventilation_level{nullptr};
  sensor::Sensor *outside_air_temperature{nullptr};
  sensor::Sensor *supply_air_temperature{nullptr};
  sensor::Sensor *return_air_temperature{nullptr};
  sensor::Sensor *exhaust_air_temperature{nullptr};
  sensor::Sensor *enthalpy_temperature{nullptr};
  sensor::Sensor *ewt_temperature{nullptr};
  sensor::Sensor *reheating_temperature{nullptr};
  sensor::Sensor *kitchen_hood_temperature{nullptr};
  sensor::Sensor *return_air_level{nullptr};
  sensor::Sensor *supply_air_level{nullptr};
  sensor::Sensor *bypass_factor{nullptr};
  sensor::Sensor *bypass_step{nullptr};
  sensor::Sensor *bypass_correction{nullptr};
  sensor::Sensor *bypass_open_hours{nullptr};
  sensor::Sensor *motor_current_bypass{nullptr};
  sensor::Sensor *motor_current_preheating{nullptr};
  sensor::Sensor *preheating_hours{nullptr};
  sensor::Sensor *level0_hours{nullptr};
  sensor::Sensor *level1_hours{nullptr};
  sensor::Sensor *level2_hours{nullptr};
  sensor::Sensor *level3_hours{nullptr};
  binary_sensor::BinarySensor *frost_protection_active{nullptr};
  sensor::Sensor *frost_protection_hours{nullptr};
  sensor::Sensor *frost_protection_minutes{nullptr};
  sensor::Sensor *filter_hours{nullptr};
  sensor::Sensor *bypass_valve{nullptr};
  binary_sensor::BinarySensor *bypass_present{nullptr};
  binary_sensor::BinarySensor *enthalpy_present{nullptr};
  binary_sensor::BinarySensor *ewt_present{nullptr};
  binary_sensor::BinarySensor *preheating_present{nullptr};
  binary_sensor::BinarySensor *options_present{nullptr};
  binary_sensor::BinarySensor *fireplace_present{nullptr};
  binary_sensor::BinarySensor *kitchen_hood_present{nullptr};
  binary_sensor::BinarySensor *postheating_present{nullptr};
  binary_sensor::BinarySensor *postheating_pwm_mode_present{nullptr};
  binary_sensor::BinarySensor *bypass_valve_open{nullptr};
  binary_sensor::BinarySensor *preheating_state{nullptr};
  binary_sensor::BinarySensor *summer_mode{nullptr};
  binary_sensor::BinarySensor *supply_fan_active{nullptr};
  binary_sensor::BinarySensor *p10_active{nullptr};
  binary_sensor::BinarySensor *p11_active{nullptr};
  binary_sensor::BinarySensor *p12_active{nullptr};
  binary_sensor::BinarySensor *p13_active{nullptr};
  binary_sensor::BinarySensor *p14_active{nullptr};
  binary_sensor::BinarySensor *p15_active{nullptr};
  binary_sensor::BinarySensor *p16_active{nullptr};
  binary_sensor::BinarySensor *p17_active{nullptr};
  binary_sensor::BinarySensor *p18_active{nullptr};
  binary_sensor::BinarySensor *p19_active{nullptr};
  binary_sensor::BinarySensor *p90_active{nullptr};
  binary_sensor::BinarySensor *p91_active{nullptr};
  binary_sensor::BinarySensor *p92_active{nullptr};
  binary_sensor::BinarySensor *p93_active{nullptr};
  binary_sensor::BinarySensor *p94_active{nullptr};
  binary_sensor::BinarySensor *p95_active{nullptr};
  binary_sensor::BinarySensor *p96_active{nullptr};
  binary_sensor::BinarySensor *p97_active{nullptr};
  sensor::Sensor *bathroom_switch_on_delay_minutes{nullptr};
  sensor::Sensor *bathroom_switch_off_delay_minutes{nullptr};
  sensor::Sensor *l1_switch_off_delay_minutes{nullptr};
  sensor::Sensor *boost_ventilation_minutes{nullptr};
  sensor::Sensor *filter_warning_weeks{nullptr};
  sensor::Sensor *rf_high_time_short_minutes{nullptr};
  sensor::Sensor *rf_high_time_long_minutes{nullptr};
  sensor::Sensor *extractor_hood_switch_off_delay_minutes{nullptr};

  void set_type(text_sensor::TextSensor *type);
  void set_size(text_sensor::TextSensor *size);
  void set_intake_fan_speed(sensor::Sensor *intake_fan_speed);
  void set_exhaust_fan_speed(sensor::Sensor *exhaust_fan_speed);
  void set_intake_fan_speed_rpm(sensor::Sensor *intake_fan_speed_rpm);
  void set_exhaust_fan_speed_rpm(sensor::Sensor *exhaust_fan_speed_rpm);
  void set_ventilation_level(sensor::Sensor *ventilation_level);
  void set_bypass_valve(sensor::Sensor *bypass_valve);
  void set_bypass_present(binary_sensor::BinarySensor *bypass_present);
  void set_enthalpy_present(binary_sensor::BinarySensor *enthalpy_present);
  void set_ewt_present(binary_sensor::BinarySensor *ewt_present);
  void set_preheating_present(binary_sensor::BinarySensor *preheating_present);
  void set_options_present(binary_sensor::BinarySensor *options_present);
  void set_fireplace_present(binary_sensor::BinarySensor *fireplace_present);
  void set_kitchen_hood_present(binary_sensor::BinarySensor *kitchen_hood_present);
  void set_postheating_present(binary_sensor::BinarySensor *postheating_present);
  void set_postheating_pwm_mode_present(binary_sensor::BinarySensor *postheating_pwm_mode_present);
  void set_bypass_valve_open(binary_sensor::BinarySensor *bypass_valve_open);
  void set_preheating_state(binary_sensor::BinarySensor *preheating_state);
  void set_outside_air_temperature(sensor::Sensor *outside_air_temperature);
  void set_supply_air_temperature(sensor::Sensor *supply_air_temperature);
  void set_return_air_temperature(sensor::Sensor *return_air_temperature);
  void set_exhaust_air_temperature(sensor::Sensor *exhaust_air_temperature);
  void set_enthalpy_temperature(sensor::Sensor *enthalpy_temperature);
  void set_ewt_temperature(sensor::Sensor *ewt_temperature);
  void set_reheating_temperature(sensor::Sensor *reheating_temperature);
  void set_kitchen_hood_temperature(sensor::Sensor *kitchen_hood_temperature);
  void set_return_air_level(sensor::Sensor *return_air_level);
  void set_supply_air_level(sensor::Sensor *supply_air_level);
  void set_supply_fan_active(binary_sensor::BinarySensor *supply_fan_active);
  void set_filter_status(text_sensor::TextSensor *filter_status);
  void set_bypass_factor(sensor::Sensor *bypass_factor);
  void set_bypass_step(sensor::Sensor *bypass_step);
  void set_bypass_correction(sensor::Sensor *bypass_correction);
  void set_bypass_open_hours(sensor::Sensor *bypass_open_hours);
  void set_motor_current_bypass(sensor::Sensor *motor_current_bypass);
  void set_motor_current_preheating(sensor::Sensor *motor_current_preheating);
  void set_preheating_hours(sensor::Sensor *preheating_hours);
  void set_preheating_valve(text_sensor::TextSensor *preheating_valve);
  void set_level0_hours(sensor::Sensor *level0_hours);
  void set_level1_hours(sensor::Sensor *level1_hours);
  void set_level2_hours(sensor::Sensor *level2_hours);
  void set_level3_hours(sensor::Sensor *level3_hours);
  void set_frost_protection_active(binary_sensor::BinarySensor *frost_protection_active);
  void set_frost_protection_hours(sensor::Sensor *frost_protection_hours);
  void set_frost_protection_minutes(sensor::Sensor *frost_protection_minutes);
  void set_frost_protection_level(text_sensor::TextSensor *frost_protection_level);
  void set_filter_hours(sensor::Sensor *filter_hours);
  void set_summer_mode(binary_sensor::BinarySensor *summer_mode);
  void set_p10_active(binary_sensor::BinarySensor *p10_active);
  void set_p11_active(binary_sensor::BinarySensor *p11_active);
  void set_p12_active(binary_sensor::BinarySensor *p12_active);
  void set_p13_active(binary_sensor::BinarySensor *p13_active);
  void set_p14_active(binary_sensor::BinarySensor *p14_active);
  void set_p15_active(binary_sensor::BinarySensor *p15_active);
  void set_p16_active(binary_sensor::BinarySensor *p16_active);
  void set_p17_active(binary_sensor::BinarySensor *p17_active);
  void set_p18_active(binary_sensor::BinarySensor *p18_active);
  void set_p19_active(binary_sensor::BinarySensor *p19_active);
  void set_p90_active(binary_sensor::BinarySensor *p90_active);
  void set_p91_active(binary_sensor::BinarySensor *p91_active);
  void set_p92_active(binary_sensor::BinarySensor *p92_active);
  void set_p93_active(binary_sensor::BinarySensor *p93_active);
  void set_p94_active(binary_sensor::BinarySensor *p94_active);
  void set_p95_active(binary_sensor::BinarySensor *p95_active);
  void set_p96_active(binary_sensor::BinarySensor *p96_active);
  void set_p97_active(binary_sensor::BinarySensor *p97_active);
  void set_bathroom_switch_on_delay_minutes(sensor::Sensor *bathroom_switch_on_delay_minutes);
  void set_bathroom_switch_off_delay_minutes(sensor::Sensor *bathroom_switch_off_delay_minutes);
  void set_l1_switch_off_delay_minutes(sensor::Sensor *l1_switch_off_delay_minutes);
  void set_boost_ventilation_minutes(sensor::Sensor *boost_ventilation_minutes);
  void set_filter_warning_weeks(sensor::Sensor *filter_warning_weeks);
  void set_rf_high_time_short_minutes(sensor::Sensor *rf_high_time_short_minutes);
  void set_rf_high_time_long_minutes(sensor::Sensor *rf_high_time_long_minutes);
  void set_extractor_hood_switch_off_delay_minutes(sensor::Sensor *extractor_hood_switch_off_delay_minutes);
};

}  // namespace comfoair
}  // namespace esphome
