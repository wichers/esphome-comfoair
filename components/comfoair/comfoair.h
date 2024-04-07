#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/climate/climate_mode.h"
#include "esphome/components/climate/climate_traits.h"
#include "esphome/components/sensor/sensor.h"
#include "registers.h"

namespace esphome {
namespace comfoair {

static const char *TAG = "comfoair";

class ComfoAirComponent : public climate::Climate, public PollingComponent, public uart::UARTDevice {
public:

  // Poll every 600ms
  ComfoAirComponent() :
  Climate(),
  PollingComponent(600),
  UARTDevice() { }

  /// Return the traits of this controller.
  climate::ClimateTraits traits() override {
    auto traits = climate::ClimateTraits();
    traits.set_supports_current_temperature(true);
    traits.set_supported_modes({
      climate::CLIMATE_MODE_FAN_ONLY
    });
    traits.set_supports_two_point_target_temperature(false);
    traits.set_supported_presets({
        climate::CLIMATE_PRESET_HOME,
    });
    traits.set_supports_action(false);
    traits.set_visual_min_temperature(12);
    traits.set_visual_max_temperature(29);
    traits.set_visual_temperature_step(1);
    traits.set_supported_fan_modes({
      climate::CLIMATE_FAN_AUTO,
      climate::CLIMATE_FAN_LOW,
      climate::CLIMATE_FAN_MEDIUM,
      climate::CLIMATE_FAN_HIGH,
      climate::CLIMATE_FAN_OFF,
    });
    return traits;
  }

  /// Override control to change settings of the climate device.
  void control(const climate::ClimateCall &call) override {
    if (call.get_fan_mode().has_value()) {
      int level;

      fan_mode = *call.get_fan_mode();
      switch (fan_mode.value()) {
        case climate::CLIMATE_FAN_HIGH:
          level = 0x04;
          break;
        case climate::CLIMATE_FAN_MEDIUM:
          level = 0x03;
          break;
        case climate::CLIMATE_FAN_LOW:
          level = 0x02;
          break;
        case climate::CLIMATE_FAN_OFF:
          level = 0x01;
          break;
        case climate::CLIMATE_FAN_AUTO:
          level = 0x00;
          break;
        case climate::CLIMATE_FAN_ON:
        case climate::CLIMATE_FAN_MIDDLE:
        case climate::CLIMATE_FAN_DIFFUSE:
        default:
          level = -1;
          break;
      }

      if (level >= 0) {
        set_level_(level);
      }

    }
    if (call.get_target_temperature().has_value()) {
      target_temperature = *call.get_target_temperature();
      set_comfort_temperature_(target_temperature);
    }

    publish_state();
  }

  void dump_config() override {
    uint8_t *p;
    ESP_LOGCONFIG(TAG, "ComfoAir:");
    //LOG_UPDATE_INTERVAL(this);
    p = bootloader_version_;
    ESP_LOGCONFIG(TAG, "  Bootloader %.10s v%0d.%02d b%2d", p + 3, *p, *(p + 1), *(p + 2));
    p = firmware_version_;
    ESP_LOGCONFIG(TAG, "  Firmware %.10s v%0d.%02d b%2d", p + 3, *p, *(p + 1), *(p + 2));
    p = connector_board_version_;
    ESP_LOGCONFIG(TAG, "  Connector Board %.10s v%0d.%02d", p + 2, *p, *(p + 1));

    if (*(p + 12) != 0) {
      ESP_LOGCONFIG(TAG, "  CC-Ease v%0d.%02d", *(p + 12) >> 4, *(p + 12) & 0x0f);
    }
    if (*(p + 13) != 0) {
      ESP_LOGCONFIG(TAG, "  CC-Luxe v%0d.%02d", *(p + 13) >> 4, *(p + 13) & 0x0f);
    }
    check_uart_settings(9600);
  }

  void update() override {
    switch(update_counter_) {
      case -4:
        write_command_(CMD_GET_BOOTLOADER_VERSION, nullptr, 0);
        break;
      case -3:
        write_command_(CMD_GET_FIRMWARE_VERSION, nullptr, 0);
        break;
      case -2:
        write_command_(CMD_GET_CONNECTOR_BOARD_VERSION, nullptr, 0);
        break;
      case -1:
        write_command_(CMD_GET_STATUS, nullptr, 0);
        break;
      case 0:
        get_fan_status_();
        break;
      case 1:
        get_valve_status_();
        break;
      case 2:
        get_sensor_data_();
        break;
      case 3:
        get_ventilation_level_();
        break;
      case 4:
        get_temperatures_();
        break;
      case 5:
        get_error_status_();
        break;
      case 6:
        get_bypass_control_status_();
        break;
      case 7:
        get_operation_hours_();
        break;
      case 8:
        get_preheating_status_();
        break;
      case 9:
        get_time_delay_();
        break;
    }

    update_counter_++;
    if (update_counter_ > num_update_counter_elements_)
      update_counter_ = 0;
  }

  void loop() override {
    while (available() != 0) {
      read_byte(&data_[data_index_]);
      auto check = check_byte_();
      if (!check.has_value()) {

        // finished
        if (data_[COMMAND_ID_ACK] != COMMAND_ACK) {
          parse_data_();
        }
        data_index_ = 0;
      } else if (!*check) {
        // wrong data
        ESP_LOGV(TAG, "Byte %i of received data frame is invalid.", data_index_);
        data_index_ = 0;
      } else {
        // next byte
        data_index_++;
      }
    }
  }

  float get_setup_priority() const override { return setup_priority::DATA; }

  void error_reset(void) {
    uint8_t reset_cmd[4] = {1, 0, 0, 0};
    write_command_(CMD_RESET_AND_SELF_TEST, reset_cmd, sizeof(reset_cmd));
	}

  void filter_reset(void) {
    uint8_t reset_cmd[4] = {0, 0, 0, 1};
    write_command_(CMD_RESET_AND_SELF_TEST, reset_cmd, sizeof(reset_cmd));
	}

  void set_name(const char* value) {name = value;}
  void set_uart_component(uart::UARTComponent *parent) {set_uart_parent(parent);}

protected:

  void set_level_(int level) {
    if (level < 0 || level > 5) {
      ESP_LOGI(TAG, "Ignoring invalid level request: %i", level);
      return;
    }

    ESP_LOGI(TAG, "Setting level to: %i", level);
    {
      uint8_t command[1] = {(uint8_t) level};
      write_command_(CMD_SET_LEVEL, command, sizeof(command));
    }
  }

  void set_comfort_temperature_(float temperature) {
    if (temperature < 12.0f || temperature > 29.0f) {
      ESP_LOGI(TAG, "Ignoring invalid temperature request: %i", temperature);
      return;
    }

    ESP_LOGI(TAG, "Setting temperature to: %i", temperature);
    {
      uint8_t command[1] = {(uint8_t) ((temperature + 20.0f) * 2.0f)};
      write_command_(CMD_SET_COMFORT_TEMPERATURE, command, sizeof(command));
    }
  }

  void write_command_(const uint8_t command, const uint8_t *command_data, uint8_t command_data_length) {
    write_byte(COMMAND_PREFIX);
    write_byte(COMMAND_HEAD);
    write_byte(0x00);
    write_byte(command);
    write_byte(command_data_length);
    if (command_data_length > 0) {
      write_array(command_data, command_data_length);
      write_byte((command + command_data_length + comfoair_checksum_(command_data, command_data_length)) & 0xff);
    } else {
      write_byte(comfoair_checksum_(&command, 1));
    }
    write_byte(COMMAND_PREFIX);
    write_byte(COMMAND_TAIL);
    flush();
}

  uint8_t comfoair_checksum_(const uint8_t *command_data, uint8_t length) const {
    uint8_t sum = 0;
    for (uint8_t i = 0; i < length; i++) {
      sum += command_data[i];
    }
    return sum + 0xad;
  }

  optional<bool> check_byte_() const {
    uint8_t index = data_index_;
    uint8_t byte = data_[index];

    if (index == 0) {
      return byte == COMMAND_PREFIX;
    }

    if (index == 1) {
      if (byte == COMMAND_ACK) {
        return {};
      } else {
        return byte == COMMAND_HEAD;
      }
    }

    if (index == 2) {
      return byte == 0x00;
    }

    if (index < COMMAND_LEN_HEAD) {
      return true;
    }

    uint8_t data_length = data_[COMMAND_IDX_DATA];

    if ((COMMAND_LEN_HEAD + data_length + COMMAND_LEN_TAIL) > sizeof(data_)) {
      ESP_LOGW(TAG, "ComfoAir message too large");
      return false;
    }

    if (index < COMMAND_LEN_HEAD + data_length) {
      return true;
    }

    if (index == COMMAND_LEN_HEAD + data_length) {
      // checksum is without checksum bytes
      uint8_t checksum = comfoair_checksum_(data_ + 2, COMMAND_LEN_HEAD + data_length - 2);
      if (checksum != byte) {
        //ESP_LOGW(TAG, "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", data_[0], data_[1], data_[2], data_[3], data_[4], data_[5], data_[6], data_[7], data_[8], data_[9], data_[10]);
        ESP_LOGW(TAG, "ComfoAir Checksum doesn't match: 0x%02X!=0x%02X", byte, checksum);
        return false;
      }
      return true;
    }

    if (index == COMMAND_LEN_HEAD + data_length + 1) {
      return byte == COMMAND_PREFIX;
    }

    if (index == COMMAND_LEN_HEAD + data_length + 2) {
      if (byte != COMMAND_TAIL) {
        return false;
      }
    }

    return {};
  }

  void parse_data_() {
    status_clear_warning();
    uint8_t *msg = &data_[COMMAND_LEN_HEAD];

    switch (data_[COMMAND_IDX_MSG_ID]) {
      case RES_GET_BOOTLOADER_VERSION:
        memcpy(bootloader_version_, msg, data_[COMMAND_IDX_DATA]);
        break;
      case RES_GET_FIRMWARE_VERSION:
        memcpy(firmware_version_, msg, data_[COMMAND_IDX_DATA]);
        break;
      case RES_GET_CONNECTOR_BOARD_VERSION:
        memcpy(connector_board_version_, msg, data_[COMMAND_IDX_DATA]);
        break;
      case RES_GET_FAN_STATUS: {
          if (intake_fan_speed != nullptr) {
            intake_fan_speed->publish_state(msg[0]);
          }
          if (exhaust_fan_speed != nullptr) {
            exhaust_fan_speed->publish_state(msg[1]);
          }
          if (intake_fan_speed_rpm != nullptr) {
            intake_fan_speed_rpm->publish_state(1875000.0f / get_uint16_(2));
          }
          if (exhaust_fan_speed_rpm != nullptr) {
            exhaust_fan_speed_rpm->publish_state(1875000.0f / get_uint16_(4));
          }
          break;
        }
      case RES_GET_VALVE_STATUS: {
        if (bypass_valve != nullptr) {
          bypass_valve->publish_state(msg[0]);
        }
        if (bypass_valve_open != nullptr) {
          bypass_valve_open->publish_state(msg[0] != 0);
        }
        if (preheating_state != nullptr) {
          preheating_state->publish_state(msg[1] != 0);
        }
        if (motor_current_bypass != nullptr) {
          motor_current_bypass->publish_state(msg[2]);
        }
        if (motor_current_preheating != nullptr) {
          motor_current_preheating->publish_state(msg[3]);
        }
        break;
      }
      case RES_GET_BYPASS_CONTROL_STATUS: {
        if (bypass_factor != nullptr) {
          bypass_factor->publish_state(msg[2]);
        }
        if (bypass_step != nullptr) {
          bypass_step->publish_state(msg[3]);
        }
        if (bypass_correction != nullptr) {
          bypass_correction->publish_state(msg[4]);
        }
        if (summer_mode != nullptr) {
          summer_mode->publish_state(msg[6] != 0);
        }
        break;
      }
      case RES_GET_TEMPERATURE_STATUS: {

        // T1 / outside air
        if (outside_air_temperature != nullptr) {
          outside_air_temperature->publish_state((float) msg[0] / 2.0f - 20.0f);
        }
        // T2 / supply air
        if (supply_air_temperature != nullptr) {
          supply_air_temperature->publish_state((float) msg[1] / 2.0f - 20.0f);
        }
        // T3 / return air
        if (return_air_temperature != nullptr) {
          return_air_temperature->publish_state((float) msg[2] / 2.0f - 20.0f);
        }
        // T4 / exhaust air
        if (exhaust_air_temperature != nullptr) {
          exhaust_air_temperature->publish_state((float) msg[3] / 2.0f - 20.0f);
        }
        break;
      }
      case RES_GET_SENSOR_DATA: {

        if (enthalpy_temperature != nullptr) {
          enthalpy_temperature->publish_state((float) msg[0] / 2.0f - 20.0f);
        }

        break;
      }
      case RES_GET_VENTILATION_LEVEL: {

        ESP_LOGD(TAG, "Level %02x", msg[8]);

        if (return_air_level != nullptr) {
          return_air_level->publish_state(msg[6]);
        }
        if (supply_air_level != nullptr) {
          supply_air_level->publish_state(msg[7]);
        }

        if (ventilation_level != nullptr) {
          ventilation_level->publish_state(msg[8] - 1);
        }

        // Fan Speed
        switch(msg[8]) {
          case 0x00:
            fan_mode = climate::CLIMATE_FAN_AUTO;
            mode = climate::CLIMATE_MODE_AUTO;
            break;
          case 0x01:
            fan_mode = climate::CLIMATE_FAN_OFF;
            mode = climate::CLIMATE_MODE_OFF;
            break;
          case 0x02:
            fan_mode = climate::CLIMATE_FAN_LOW;
            mode = climate::CLIMATE_MODE_FAN_ONLY;
            break;
          case 0x03:
            fan_mode = climate::CLIMATE_FAN_MEDIUM;
            mode = climate::CLIMATE_MODE_FAN_ONLY;
          break;
          case 0x04:
            fan_mode = climate::CLIMATE_FAN_HIGH;
            mode = climate::CLIMATE_MODE_FAN_ONLY;
            break;
        }

        publish_state();

        // Supply air fan active (1 = active / 0 = inactive)
        if (supply_fan_active != nullptr) {
          supply_fan_active->publish_state(msg[9] == 1);
        }
        break;
      }
      case RES_GET_FAULTS: {
        if (filter_status != nullptr) {
          uint8_t status = msg[8];
          filter_status->publish_state(status == 0 ? "Ok" : (status == 1 ? "Full" : "Unknown"));
        }
        break;
      }
      case RES_GET_TEMPERATURES: {

        // comfort temperature
        target_temperature = (float) msg[0] / 2.0f - 20.0f;
        publish_state();

        // T1 / outside air
        if (outside_air_temperature != nullptr && msg[5] & 0x01) {
          outside_air_temperature->publish_state((float) msg[1] / 2.0f - 20.0f);
        }
        // T2 / supply air
        if (supply_air_temperature != nullptr && msg[5] & 0x02) {
          supply_air_temperature->publish_state((float) msg[2] / 2.0f - 20.0f);
        }
        // T3 / exhaust air
        if (return_air_temperature != nullptr && msg[5] & 0x04) {
          return_air_temperature->publish_state((float) msg[3] / 2.0f - 20.0f);
          current_temperature = (float) msg[3] / 2.0f - 20.0f;
        }
        // T4 / continued air
        if (exhaust_air_temperature != nullptr && msg[5] & 0x08) {
          exhaust_air_temperature->publish_state((float) msg[4] / 2.0f - 20.0f);
        }
        // EWT
        if (ewt_temperature != nullptr && msg[5] & 0x10) {
          ewt_temperature->publish_state((float) msg[6] / 2.0f - 20.0f);
        }
        // reheating
        if (reheating_temperature != nullptr && msg[5] & 0x20) {
          reheating_temperature->publish_state((float) msg[7] / 2.0f - 20.0f);
        }
        // kitchen hood
        if (kitchen_hood_temperature != nullptr && msg[5] & 0x40) {
          kitchen_hood_temperature->publish_state((float) msg[8] / 2.0f - 20.0f);
        }

        break;
      }
      case RES_GET_STATUS: {
        if (preheating_present != nullptr) {
          preheating_present->publish_state(msg[0]);
        }

        if (bypass_present != nullptr) {
          bypass_present->publish_state(msg[1]);
        }

        if (type != nullptr) {
          type->publish_state(msg[2] == 1 ? "Left" : (msg[2] == 2 ? "Right" : "Unknown"));
        }

        if (size != nullptr) {
          size->publish_state(msg[3] == 1 ? "Large" : (msg[3] == 2 ? "Small" : "Unknown"));
        }

        if (options_present != nullptr) {
          options_present->publish_state(msg[4]);
        }

        if (p10_active != nullptr) {
          p10_active->publish_state(msg[6] & 0x01);
        }

        if (p11_active != nullptr) {
          p11_active->publish_state(msg[6] & 0x02);
        }

        if (p12_active != nullptr) {
          p12_active->publish_state(msg[6] & 0x04);
        }

        if (p13_active != nullptr) {
          p13_active->publish_state(msg[6] & 0x08);
        }

        if (p14_active != nullptr) {
          p14_active->publish_state(msg[6] & 0x10);
        }

        if (p15_active != nullptr) {
          p15_active->publish_state(msg[6] & 0x20);
        }

        if (p16_active != nullptr) {
          p16_active->publish_state(msg[6] & 0x40);
        }

        if (p17_active != nullptr) {
          p17_active->publish_state(msg[6] & 0x80);
        }

        if (p18_active != nullptr) {
          p18_active->publish_state(msg[7] & 0x01);
        }

        if (p19_active != nullptr) {
          p19_active->publish_state(msg[7] & 0x02);
        }

        if (p90_active != nullptr) {
          p90_active->publish_state(msg[8] & 0x01);
        }

        if (p91_active != nullptr) {
          p91_active->publish_state(msg[8] & 0x02);
        }

        if (p92_active != nullptr) {
          p92_active->publish_state(msg[8] & 0x04);
        }

        if (p93_active != nullptr) {
          p93_active->publish_state(msg[8] & 0x08);
        }

        if (p94_active != nullptr) {
          p94_active->publish_state(msg[8] & 0x10);
        }

        if (p95_active != nullptr) {
          p95_active->publish_state(msg[8] & 0x20);
        }

        if (p96_active != nullptr) {
          p96_active->publish_state(msg[8] & 0x40);
        }

        if (p97_active != nullptr) {
          p97_active->publish_state(msg[8] & 0x80);
        }

        if (enthalpy_present != nullptr) {
          enthalpy_present->publish_state(msg[9]);
        }

        if (ewt_present != nullptr) {
          ewt_present->publish_state(msg[10]);
        }
        break;
      }
      case RES_GET_OPERATION_HOURS: {
        if (level0_hours != nullptr) {
          level0_hours->publish_state(msg[0] + msg[1] + msg[2]);
        }

        if (level1_hours != nullptr) {
          level1_hours->publish_state(msg[3] + msg[4] + msg[5]);
        }

        if (level2_hours != nullptr) {
          level2_hours->publish_state(msg[6] + msg[7] + msg[8]);
        }

        if (level3_hours != nullptr) {
          level3_hours->publish_state(msg[17] + msg[18] + msg[19]);
        }

        if (frost_protection_hours != nullptr) {
          frost_protection_hours->publish_state(msg[9] + msg[10]);
        }

        if (bypass_open_hours != nullptr) {
          bypass_open_hours->publish_state(msg[13] + msg[14]);
        }

        if (preheating_hours != nullptr) {
          preheating_hours->publish_state(msg[11] + msg[12]);
        }

        if (filter_hours != nullptr) {
          filter_hours->publish_state(msg[15] + msg[16]);
        }
        break;
      }

      case RES_GET_PREHEATING_STATUS: {
        if (preheating_valve != nullptr) {
          std::string name_preheating_valve;
          switch (msg[0]) {
            case 0:
              name_preheating_valve = "Closed";
              break;

            case 1:
              name_preheating_valve = "Open";
              break;

            default:
              name_preheating_valve = "Unknown";
              break;
          }
          preheating_valve->publish_state(name_preheating_valve);
        }

        if (frost_protection_active != nullptr) {
          frost_protection_active->publish_state(msg[1] != 0);
        }

        if (preheating_state != nullptr) {
          preheating_state->publish_state(msg[2] != 0);
        }

        if (frost_protection_minutes != nullptr) {
          frost_protection_minutes->publish_state(msg[3] + msg[4]);
        }

        if (frost_protection_level != nullptr) {
          std::string name_frost_protection_level;
          switch (msg[5]) {
            case 0:
              name_frost_protection_level = "GuaranteedProtection";
              break;

            case 1:
              name_frost_protection_level = "HighProtection";
              break;

            case 2:
              name_frost_protection_level = "NominalProtection";
              break;

            case 3:
              name_frost_protection_level = "Economy";
              break;

            default:
              name_frost_protection_level = "Unknown";
              break;
          }
          frost_protection_level->publish_state(name_frost_protection_level);
        }
        break;
      }
      case RES_GET_TIME_DELAY: {
        if (bathroom_switch_on_delay_minutes != nullptr) {
          bathroom_switch_on_delay_minutes->publish_state(msg[0]);
        }

        if (bathroom_switch_off_delay_minutes != nullptr) {
          bathroom_switch_off_delay_minutes->publish_state(msg[1]);
        }

        if (l1_switch_off_delay_minutes != nullptr) {
          l1_switch_off_delay_minutes->publish_state(msg[2]);
        }

        if (boost_ventilation_minutes != nullptr) {
          boost_ventilation_minutes->publish_state(msg[3]);
        }

        if (filter_warning_weeks != nullptr) {
          filter_warning_weeks->publish_state(msg[4]);
        }

        if (rf_high_time_short_minutes != nullptr) {
          rf_high_time_short_minutes->publish_state(msg[5]);
        }

        if (rf_high_time_long_minutes != nullptr) {
          rf_high_time_long_minutes->publish_state(msg[6]);
        }

        if (extractor_hood_switch_off_delay_minutes != nullptr) {
          extractor_hood_switch_off_delay_minutes->publish_state(msg[7]);
        }

        break;
      }
    }
  }

  void get_fan_status_() {
    if (intake_fan_speed != nullptr ||
        exhaust_fan_speed != nullptr ||
        intake_fan_speed_rpm != nullptr ||
        exhaust_fan_speed_rpm != nullptr) {
      ESP_LOGD(TAG, "getting fan status");
      write_command_(CMD_GET_FAN_STATUS, nullptr, 0);
    }
  }

  void get_valve_status_() {
    if (bypass_valve != nullptr ||
      bypass_valve_open != nullptr ||
      preheating_state != nullptr) {
      ESP_LOGD(TAG, "getting valve status");
      write_command_(CMD_GET_VALVE_STATUS, nullptr, 0);
    }
  }

  void get_error_status_() {
    if (filter_status != nullptr) {
      ESP_LOGD(TAG, "getting error status");
      write_command_(CMD_GET_FAULTS, nullptr, 0);
    }
  }

  void get_bypass_control_status_() {
    if (bypass_factor != nullptr ||
      bypass_step != nullptr ||
      bypass_correction != nullptr ||
      summer_mode != nullptr) {
      ESP_LOGD(TAG, "getting bypass control");
      write_command_(CMD_GET_BYPASS_CONTROL_STATUS, nullptr, 0);
    }
  }

  void get_temperature_() {
    if (outside_air_temperature != nullptr ||
      supply_air_temperature != nullptr ||
      return_air_temperature != nullptr ||
      outside_air_temperature != nullptr) {
      ESP_LOGD(TAG, "getting temperature");
      write_command_(CMD_GET_TEMPERATURE_STATUS, nullptr, 0);
    }
  }

  void get_sensor_data_() {
    if (enthalpy_temperature != nullptr) {
      ESP_LOGD(TAG, "getting sensor data");
      write_command_(CMD_GET_SENSOR_DATA, nullptr, 0);
    }
  }

  void get_ventilation_level_() {
    ESP_LOGD(TAG, "getting ventilation level");
    write_command_(CMD_GET_VENTILATION_LEVEL, nullptr, 0);
  }

  void get_temperatures_() {
    ESP_LOGD(TAG, "getting temperatures");
    write_command_(CMD_GET_TEMPERATURES, nullptr, 0);
  }

  void get_operation_hours_() {
    ESP_LOGD(TAG, "getting operation hours");
    write_command_(CMD_GET_OPERATION_HOURS, nullptr, 0);
  }

  void get_preheating_status_() {
    ESP_LOGD(TAG, "getting preheating status");
    write_command_(CMD_GET_PREHEATING_STATUS, nullptr, 0);
  }

  void get_time_delay_() {
    ESP_LOGD(TAG, "getting time delay");
    write_command_(CMD_GET_TIME_DELAY, nullptr, 0);
  }

  uint8_t get_uint8_t_(uint8_t start_index) const {
    return data_[COMMAND_LEN_HEAD + start_index];
  }

  uint16_t get_uint16_(uint8_t start_index) const {
    return (uint16_t(data_[COMMAND_LEN_HEAD + start_index + 1] | data_[COMMAND_LEN_HEAD + start_index] << 8));
  }

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

  void set_type(text_sensor::TextSensor *type) { this->type = type; };
  void set_size(text_sensor::TextSensor *size) { this->size = size; };
  void set_intake_fan_speed(sensor::Sensor *intake_fan_speed) { this->intake_fan_speed = intake_fan_speed; };
  void set_exhaust_fan_speed(sensor::Sensor *exhaust_fan_speed) { this->exhaust_fan_speed = exhaust_fan_speed; };
  void set_intake_fan_speed_rpm(sensor::Sensor *intake_fan_speed_rpm) { this->intake_fan_speed_rpm = intake_fan_speed_rpm; };
  void set_exhaust_fan_speed_rpm(sensor::Sensor *exhaust_fan_speed_rpm) { this->exhaust_fan_speed_rpm = exhaust_fan_speed_rpm; };
  void set_ventilation_level(sensor::Sensor *ventilation_level) { this->ventilation_level = ventilation_level; };
  void set_bypass_valve(sensor::Sensor *bypass_valve) { this->bypass_valve = bypass_valve; };
  void set_bypass_present(binary_sensor::BinarySensor *bypass_present) { this->bypass_present = bypass_present; };
  void set_enthalpy_present(binary_sensor::BinarySensor *enthalpy_present) { this->enthalpy_present = enthalpy_present; };
  void set_ewt_present(binary_sensor::BinarySensor *ewt_present) { this->ewt_present = ewt_present; };
  void set_preheating_present(binary_sensor::BinarySensor *preheating_present) { this->preheating_present = preheating_present; };
  void set_options_present(binary_sensor::BinarySensor *options_present) { this->options_present = options_present; };
  void set_bypass_valve_open(binary_sensor::BinarySensor *bypass_valve_open) { this->bypass_valve_open = bypass_valve_open; };
  void set_preheating_state(binary_sensor::BinarySensor *preheating_state) { this->preheating_state = preheating_state; };
  void set_outside_air_temperature(sensor::Sensor *outside_air_temperature) { this->outside_air_temperature = outside_air_temperature; };
  void set_supply_air_temperature(sensor::Sensor *supply_air_temperature) { this->supply_air_temperature = supply_air_temperature; };
  void set_return_air_temperature(sensor::Sensor *return_air_temperature) { this->return_air_temperature = return_air_temperature; };
  void set_exhaust_air_temperature(sensor::Sensor *exhaust_air_temperature) { this->exhaust_air_temperature = exhaust_air_temperature; };
  void set_enthalpy_temperature(sensor::Sensor *enthalpy_temperature) { this->enthalpy_temperature = enthalpy_temperature; };
  void set_ewt_temperature(sensor::Sensor *ewt_temperature) { this->ewt_temperature = ewt_temperature; };
  void set_reheating_temperature(sensor::Sensor *reheating_temperature) { this->reheating_temperature = reheating_temperature; };
  void set_kitchen_hood_temperature(sensor::Sensor *kitchen_hood_temperature) { this->kitchen_hood_temperature = kitchen_hood_temperature; };
  void set_return_air_level(sensor::Sensor *return_air_level) { this->return_air_level = return_air_level; };
  void set_supply_air_level(sensor::Sensor *supply_air_level) { this->supply_air_level = supply_air_level; };
  void set_supply_fan_active(binary_sensor::BinarySensor *supply_fan_active) { this->supply_fan_active = supply_fan_active; };
  void set_filter_status(text_sensor::TextSensor *filter_status) { this->filter_status = filter_status; };
  void set_bypass_factor(sensor::Sensor *bypass_factor) { this->bypass_factor = bypass_factor; };
  void set_bypass_step(sensor::Sensor *bypass_step) { this->bypass_step = bypass_step; };
  void set_bypass_correction(sensor::Sensor *bypass_correction) { this->bypass_correction = bypass_correction; };
  void set_bypass_open_hours(sensor::Sensor *bypass_open_hours) { this->bypass_open_hours = bypass_open_hours; };
  void set_motor_current_bypass(sensor::Sensor *motor_current_bypass) { this->motor_current_bypass = motor_current_bypass; };
  void set_motor_current_preheating(sensor::Sensor *motor_current_preheating) { this->motor_current_preheating = motor_current_preheating; };
  void set_preheating_hours(sensor::Sensor *preheating_hours) { this->preheating_hours = preheating_hours; };
  void set_preheating_valve(text_sensor::TextSensor *preheating_valve) { this->preheating_valve = preheating_valve; };
  void set_level0_hours(sensor::Sensor *level0_hours) { this->level0_hours = level0_hours; };
  void set_level1_hours(sensor::Sensor *level1_hours) { this->level1_hours = level1_hours; };
  void set_level2_hours(sensor::Sensor *level2_hours) { this->level2_hours = level2_hours; };
  void set_level3_hours(sensor::Sensor *level3_hours) { this->level3_hours = level3_hours; };
  void set_frost_protection_active(binary_sensor::BinarySensor *frost_protection_active) { this->frost_protection_active = frost_protection_active; };
  void set_frost_protection_hours(sensor::Sensor *frost_protection_hours) { this->frost_protection_hours = frost_protection_hours; };
  void set_frost_protection_minutes(sensor::Sensor *frost_protection_minutes) { this->frost_protection_minutes = frost_protection_minutes; };
  void set_frost_protection_level(text_sensor::TextSensor *frost_protection_level) { this->frost_protection_level = frost_protection_level; };
  void set_filter_hours(sensor::Sensor *filter_hours) { this->filter_hours = filter_hours; };
  void set_summer_mode(binary_sensor::BinarySensor *summer_mode) { this->summer_mode = summer_mode; };
  void set_p10_active(binary_sensor::BinarySensor *p10_active) { this->p10_active = p10_active; };
  void set_p11_active(binary_sensor::BinarySensor *p11_active) { this->p11_active = p11_active; };
  void set_p12_active(binary_sensor::BinarySensor *p12_active) { this->p12_active = p12_active; };
  void set_p13_active(binary_sensor::BinarySensor *p13_active) { this->p13_active = p13_active; };
  void set_p14_active(binary_sensor::BinarySensor *p14_active) { this->p14_active = p14_active; };
  void set_p15_active(binary_sensor::BinarySensor *p15_active) { this->p15_active = p15_active; };
  void set_p16_active(binary_sensor::BinarySensor *p16_active) { this->p16_active = p16_active; };
  void set_p17_active(binary_sensor::BinarySensor *p17_active) { this->p17_active = p17_active; };
  void set_p18_active(binary_sensor::BinarySensor *p18_active) { this->p18_active = p18_active; };
  void set_p19_active(binary_sensor::BinarySensor *p19_active) { this->p19_active = p19_active; };
  void set_p90_active(binary_sensor::BinarySensor *p90_active) { this->p90_active = p90_active; };
  void set_p91_active(binary_sensor::BinarySensor *p91_active) { this->p91_active = p91_active; };
  void set_p92_active(binary_sensor::BinarySensor *p92_active) { this->p92_active = p92_active; };
  void set_p93_active(binary_sensor::BinarySensor *p93_active) { this->p93_active = p93_active; };
  void set_p94_active(binary_sensor::BinarySensor *p94_active) { this->p94_active = p94_active; };
  void set_p95_active(binary_sensor::BinarySensor *p95_active) { this->p95_active = p95_active; };
  void set_p96_active(binary_sensor::BinarySensor *p96_active) { this->p96_active = p96_active; };
  void set_p97_active(binary_sensor::BinarySensor *p97_active) { this->p97_active = p97_active; };
  void set_bathroom_switch_on_delay_minutes(sensor::Sensor *bathroom_switch_on_delay_minutes) { this->bathroom_switch_on_delay_minutes = bathroom_switch_on_delay_minutes; };
  void set_bathroom_switch_off_delay_minutes(sensor::Sensor *bathroom_switch_off_delay_minutes) { this->bathroom_switch_off_delay_minutes = bathroom_switch_off_delay_minutes; };
  void set_l1_switch_off_delay_minutes(sensor::Sensor *l1_switch_off_delay_minutes) { this->l1_switch_off_delay_minutes = l1_switch_off_delay_minutes; };
  void set_boost_ventilation_minutes(sensor::Sensor *boost_ventilation_minutes) { this->boost_ventilation_minutes = boost_ventilation_minutes; };
  void set_filter_warning_weeks(sensor::Sensor *filter_warning_weeks) { this->filter_warning_weeks = filter_warning_weeks; };
  void set_rf_high_time_short_minutes(sensor::Sensor *rf_high_time_short_minutes) { this->rf_high_time_short_minutes = rf_high_time_short_minutes; };
  void set_rf_high_time_long_minutes(sensor::Sensor *rf_high_time_long_minutes) { this->rf_high_time_long_minutes = rf_high_time_long_minutes; };
  void set_extractor_hood_switch_off_delay_minutes(sensor::Sensor *extractor_hood_switch_off_delay_minutes) { this->extractor_hood_switch_off_delay_minutes = extractor_hood_switch_off_delay_minutes; };
};

}  // namespace comfoair
}  // namespace esphome
