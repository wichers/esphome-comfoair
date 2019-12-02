#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace comfoair {

static const char *TAG = "comfoair";

static const uint8_t COMFOAIR_MSG_HEAD_LENGTH = 5;
static const uint8_t COMFOAIR_MSG_TAIL_LENGTH = 3;
static const uint8_t COMFOAIR_MSG_PREFIX = 0x07;
static const uint8_t COMFOAIR_MSG_HEAD = 0xf0;
static const uint8_t COMFOAIR_MSG_TAIL = 0x0f;
static const uint8_t COMFOAIR_MSG_ACK = 0xf3;
static const uint8_t COMFOAIR_MSG_IDENTIFIER_IDX = 3;
static const uint8_t COMFOAIR_MSG_DATA_LENGTH_IDX = 4;
static const uint8_t COMFOAIR_MSG_ACK_IDX = 1;

static const uint8_t COMFOAIR_GET_BOOTLOADER_VERSION_REQUEST = 0x67;
static const uint8_t COMFOAIR_GET_BOOTLOADER_VERSION_RESPONSE = 0x68;
static const uint8_t COMFOAIR_GET_BOOTLOADER_VERSION_LENGTH = 13;
static const uint8_t COMFOAIR_GET_FIRMWARE_VERSION_REQUEST = 0x69;
static const uint8_t COMFOAIR_GET_FIRMWARE_VERSION_RESPONSE = 0x6a;
static const uint8_t COMFOAIR_GET_FIRMWARE_VERSION_LENGTH = 13;
static const uint8_t COMFOAIR_GET_BOARD_VERSION_REQUEST = 0xa1;
static const uint8_t COMFOAIR_GET_BOARD_VERSION_RESPONSE = 0xa2;
static const uint8_t COMFOAIR_GET_BOARD_VERSION_LENGTH = 14;

static const uint8_t COMFOAIR_SET_RS232_MODE_REQUEST = 0x9b;
static const uint8_t COMFOAIR_SET_RS232_MODE_RESPONSE = 0x9c;
static const uint8_t COMFOAIR_SET_RS232_MODE_LENGTH = 0x01;

static const uint8_t COMFOAIR_GET_INPUTS_REQUEST = 0x03;
static const uint8_t COMFOAIR_GET_INPUTS_RESPONSE = 0x04;
static const uint8_t COMFOAIR_GET_INPUTS_LENGTH = 0x02;
static const uint8_t COMFOAIR_GET_FAN_STATUS_REQUEST = 0x0b;
static const uint8_t COMFOAIR_GET_FAN_STATUS_RESPONSE = 0x0c;
static const uint8_t COMFOAIR_GET_FAN_STATUS_LENGTH = 0x06;
static const uint8_t COMFOAIR_GET_VALVE_STATUS_REQUEST = 0x0d;
static const uint8_t COMFOAIR_GET_VALVE_STATUS_RESPONSE = 0x0e;
static const uint8_t COMFOAIR_GET_VALVE_STATUS_LENGTH = 0x04;
static const uint8_t COMFOAIR_GET_TEMPERATURE_REQUEST = 0x0f;
static const uint8_t COMFOAIR_GET_TEMPERATURE_RESPONSE = 0x10;
static const uint8_t COMFOAIR_GET_TEMPERATURE_LENGTH = 0x04;
static const uint8_t COMFOAIR_GET_BUTTON_STATUS_REQUEST = 0x11;
static const uint8_t COMFOAIR_GET_BUTTON_STATUS_RESPONSE = 0x12;
static const uint8_t COMFOAIR_GET_BUTTON_STATUS_LENGTH = 0x01;
static const uint8_t COMFOAIR_GET_ANALOG_INPUTS_REQUEST = 0x13;
static const uint8_t COMFOAIR_GET_ANALOG_INPUTS_RESPONSE = 0x14;
static const uint8_t COMFOAIR_GET_ANALOG_INPUTS_LENGTH = 0x04;
static const uint8_t COMFOAIR_GET_SENSOR_DATA_REQUEST = 0x97;
static const uint8_t COMFOAIR_GET_SENSOR_DATA_RESPONSE = 0x98;
static const uint8_t COMFOAIR_GET_SENSOR_DATA_RESPONSE = 0x11;
static const uint8_t COMFOAIR_GET_ANALOG_STATUS_REQUEST = 0x9d;
static const uint8_t COMFOAIR_GET_ANALOG_STATUS_RESPONSE = 0x9e;
static const uint8_t COMFOAIR_GET_ANALOG_STATUS_LENGTH = 0x0a;
static const uint8_t COMFOAIR_GET_DELAYS_REQUEST = 0xc9;
static const uint8_t COMFOAIR_GET_DELAYS_RESPONSE = 0xca;
static const uint8_t COMFOAIR_GET_DELAYS_RESPONSE = 0x08;
static const uint8_t COMFOAIR_GET_VENTILATION_LEVEL_REQUEST = 0xcd;
static const uint8_t COMFOAIR_GET_VENTILATION_LEVEL_RESPONSE = 0xce;
static const uint8_t COMFOAIR_GET_VENTILATION_LEVEL_LENGTH = 0x0e;
static const uint8_t COMFOAIR_GET_TEMPERATURES_REQUEST = 0xd1;
static const uint8_t COMFOAIR_GET_TEMPERATURES_RESPONSE = 0xd2;
static const uint8_t COMFOAIR_GET_TEMPERATURES_LENGTH = 0x09;
static const uint8_t COMFOAIR_GET_MAIN_STATUS_REQUEST = 0xd5;
static const uint8_t COMFOAIR_GET_MAIN_STATUS_RESPONSE = 0xd6;
static const uint8_t COMFOAIR_GET_MAIN_STATUS_LENGTH = 0x0b;
static const uint8_t COMFOAIR_GET_ERROR_STATE_REQUEST = 0xd9;
static const uint8_t COMFOAIR_GET_ERROR_STATE_RESPONSE = 0xda;
static const uint8_t COMFOAIR_GET_ERROR_STATE_LENGTH = 0x11;
static const uint8_t COMFOAIR_GET_OPERATING_HOURS_REQUEST = 0xdd;
static const uint8_t COMFOAIR_GET_OPERATING_HOURS_RESPONSE = 0xde;
static const uint8_t COMFOAIR_GET_OPERATING_HOURS_LENGTH = 0x14;
static const uint8_t COMFOAIR_GET_BYPASS_CONTROL_REQUEST = 0xdf;
static const uint8_t COMFOAIR_GET_BYPASS_CONTROL_RESPONSE = 0xe0;
static const uint8_t COMFOAIR_GET_BYPASS_CONTROL_LENGTH = 0x07;
static const uint8_t COMFOAIR_GET_PREHEATER_STATUS_REQUEST = 0xe1;
static const uint8_t COMFOAIR_GET_PREHEATER_STATUS_RESPONSE = 0xe2;
static const uint8_t COMFOAIR_GET_PREHEATER_STATUS_LENGTH = 0x06;
static const uint8_t COMFOAIR_GET_RF_STATUS_REQUEST = 0xe5;
static const uint8_t COMFOAIR_GET_RF_STATUS_RESPONSE = 0xe6;
static const uint8_t COMFOAIR_GET_RF_STATUS_LENGTH = 0x07;
static const uint8_t COMFOAIR_GET_LAST_PREHEATING_REQUEST = 0xe9;
static const uint8_t COMFOAIR_GET_LAST_PREHEATING_RESPONSE = 0xea;
static const uint8_t COMFOAIR_GET_LAST_PREHEATING_LENGTH = 0x08;
static const uint8_t COMFOAIR_GET_EWT_REHEATER_REQUEST = 0xeb;
static const uint8_t COMFOAIR_GET_EWT_REHEATER_RESPONSE = 0xec;
static const uint8_t COMFOAIR_GET_EWT_REHEATER_LENGTH = 0x07;

// requests with ACK response
static const uint8_t COMFOAIR_SET_LEVEL_REQUEST = 0x99;
static const uint8_t COMFOAIR_SET_LEVEL_LENGTH = 0x01;
static const uint8_t COMFOAIR_SET_ANALOG_VALUES_REQUEST = 0x9f;
static const uint8_t COMFOAIR_SET_ANALOG_VALUES_LENGTH = 0x13;
static const uint8_t COMFOAIR_SET_DELAYS_REQUEST = 0xcb;
static const uint8_t COMFOAIR_SET_DELAYS_LENGTH = 0x08;
static const uint8_t COMFOAIR_SET_VENTILATION_LEVEL_REQUEST = 0xcf;
static const uint8_t COMFOAIR_SET_VENTILATION_LEVEL_LENGTH = 0x09;
static const uint8_t COMFOAIR_SET_COMFORT_TEMPERATURE_REQUEST = 0xd3;
static const uint8_t COMFOAIR_SET_COMFORT_TEMPERATURE_LENGTH = 0x01;
static const uint8_t COMFOAIR_SET_MAIN_STATUS_REQUEST = 0xd7;
static const uint8_t COMFOAIR_SET_MAIN_STATUS_LENGTH = 0x08;
static const uint8_t COMFOAIR_SET_RESET_SELFTEST_REQUEST = 0xdb;
static const uint8_t COMFOAIR_SET_RESET_SELFTEST_LENGTH = 0x04;
static const uint8_t COMFOAIR_SET_EWT_REHEATER_REQUEST = 0xed;
static const uint8_t COMFOAIR_SET_EWT_REHEATER_LENGTH = 0x05;

// Specials setters
static const uint8_t COMFOAIR_SET_TEST_MODE_START_REQUEST = 0x01;
static const uint8_t COMFOAIR_SET_TEST_MODE_END_REQUEST = 0x19;
static const uint8_t COMFOAIR_SET_OUTPUTS_REQUEST = 0x05;
static const uint8_t COMFOAIR_SET_ANALOG_OUTPUTS_REQUEST = 0x07;
static const uint8_t COMFOAIR_SET_VALVES_REQUEST = 0x09;

class ComfoAirComponent : public PollingComponent, Climate, uart::UARTDevice {
 public:

  // For now poll every 2 seconds
  ComfoAirComponent() : PollingComponent(2000) { }

  void setup() override {
    this->write_command_(COMFOAIR_GET_BOOTLOADER_VERSION_REQUEST, nullptr, 0);
    this->write_command_(COMFOAIR_GET_FIRMWARE_VERSION_REQUEST, nullptr, 0);
    this->write_command_(COMFOAIR_GET_BOARD_VERSION_REQUEST, nullptr, 0);
  }

  climate::ClimateTraits traits() {
    auto traits = climate::ClimateTraits();
    traits.set_supports_current_temperature(true);
    traits.set_supports_auto_mode(false);
    traits.set_supports_cool_mode(false);
    traits.set_supports_heat_mode(false);
    traits.set_supports_dry_mode(false);
    traits.set_supports_fan_only_mode(true);
    traits.set_supports_two_point_target_temperature(false);
    traits.set_supports_away(false);
    traits.set_visual_min_temperature(0);
    traits.set_visual_max_temperature(30);
    traits.set_visual_temperature_step(1);
    traits.set_supports_fan_mode_auto(true);
    traits.set_supports_fan_mode_high(true);
    traits.set_supports_fan_mode_low(true);
    traits.set_supports_fan_mode_medium(true);
    traits.set_supports_fan_mode_off(true);
    traits.set_supports_swing_mode_off(false);
    traits.set_supports_swing_mode_both(false);
    traits.set_supports_swing_mode_vertical(false);
    traits.set_supports_swing_mode_horizontal(false);
    return traits;
  }

  void control(const climate::ClimateCall &call) {
    if (call.get_fan_mode().has_value()) {
      int level = -1;

      this->fan_mode = *call.get_fan_mode();
      switch (this->fan_mode) {
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
      }

      if (level >= 0) {
        set_level_(level);
      }

    }
    if (call.get_target_temperature().has_value()) {
      this->target_temperature = *call.get_target_temperature();
      set_comfort_temperature_(this->target_temperature);
    }

    this->publish_state();
  }

  void dump_config() override {
    uint8_t *p;
    ESP_LOGCONFIG(TAG, "ComfoAir:");
    LOG_UPDATE_INTERVAL(this);
    p = bootloader_version_;
    ESP_LOGCONFIG(TAG, "  Bootloader %.10s v%0d.%02d b%2d", p + 3, *p, *(p + 1), *(p + 2));
    p = firmware_version_;
    ESP_LOGCONFIG(TAG, "  Firmware %.10s v%0d.%02d b%2d", p + 3, *p, *(p + 1), *(p + 2));
    p = board_connector_version;
    ESP_LOGCONFIG(TAG, "  Connector Board %.10s v%0d.%02d", p + 2, *p, *(p + 1));
    if (*(p + 12) != 0) {
      ESP_LOGCONFIG(TAG, "  CC-Ease v%0d.%02d", *(p + 12) >> 4, *(p + 12) & 0x0f);
    }
    if (*(p + 13) != 0) {
      ESP_LOGCONFIG(TAG, "  CC-Luxe v%0d.%02d", *(p + 13) >> 4, *(p + 13) & 0x0f);
    }
    this->check_uart_settings(9600);
  }

  void update() {
    switch(update_counter_) {
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
    }

    update_counter_++;
    if (update_counter_ > 4)
      update_counter_ = 0;
  }

  void loop() override {
    while (this->available() != 0) {
      this->read_byte(&this->data_[this->data_index_]);
      auto check = this->check_byte_();
      if (!check.has_value()) {
        // finished
        if (this->data_[COMFOAIR_MSG_ACK_IDX] != COMFOAIR_MSG_ACK) {
          this->parse_data_();
        }
        this->data_index_ = 0;
      } else if (!*check) {
        // wrong data
        ESP_LOGV(TAG, "Byte %i of received data frame is invalid.", this->data_index_);
        this->data_index_ = 0;
      } else {
        // next byte
        this->data_index_++;
      }
    }
  }

  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:

  void set_level_(int level) {
    if (level < 0 || level > 4) {
      ESP_LOGI(TAG, "Ignoring invalid level request: %i", level);
      return;
    }

    ESP_LOGI(TAG, "Setting level to: %i", level);
    {
      uint8_t command[COMFOAIR_SET_LEVEL_LENGTH] = {level & 0xff};
      this->write_command_(COMFOAIR_SET_LEVEL_REQUEST, command, sizeof(command));
    }
  }

  void set_comfort_temperature_(int temperature) {
    if (temperature < 0 || temperature > 30) {
      ESP_LOGI(TAG, "Ignoring invalid temperature request: %i", temperature);
      return;
      }

    ESP_LOGI(TAG, "Setting temperature to: %i", temperature);
    {
      uint8_t command[COMFOAIR_SET_COMFORT_TEMPERATURE_LENGTH] = {((temperature & 0xff ) + 20) * 2};
      this->write_command_(COMFOAIR_SET_COMFORT_TEMPERATURE_REQUEST, command, sizeof(command));
    }
  }

  void write_command_(const uint8_t command, const uint8_t *command_data, uint8_t command_data_length) {
    this->write_byte(COMFOAIR_MSG_PREFIX);
    this->write_byte(COMFOAIR_MSG_HEAD);
    this->write_byte(0x00);
    this->write_byte(command);
    this->write_byte(command_data_length);
    if (command_data_length) {
      this->write_array(command_data, command_data_length);
      this->write_byte(command + comfoair_checksum_(command_data, command_data_length));
    } else {
      this->write_byte(comfoair_checksum_(command, 1));
    }
    this->write_byte(COMFOAIR_MSG_PREFIX);
    this->write_byte(COMFOAIR_MSG_TAIL);
  }

  uint8_t comfoair_checksum_(const uint8_t *command_data, uint8_t length) const {
    uint8_t sum = 0;
    for (uint8_t i = 0; i < length; i++) {
      sum += command_data[i];
    }
    return sum + 0xad;
  }

  optional<bool> check_byte_() const {
    uint8_t index = this->data_index_;
    uint8_t byte = this->data_[index];

    if (index == 0) {
      return byte == COMFOAIR_MSG_PREFIX;
    }

    if (index == 1) {
      if (byte == COMFOAIR_MSG_ACK) {
        return {};
      } else {
        return byte == COMFOAIR_MSG_HEAD;
      }
    }

    if (index == 2) {
      return byte == 0x00;
    }

    if (index < COMFOAIR_MSG_HEAD_LENGTH) {
      return true;
    }

    uint8_t data_length = this->data_[COMFOAIR_MSG_DATA_LENGTH_IDX];

    if ((COMFOAIR_MSG_HEAD_LENGTH + data_length + COMFOAIR_MSG_TAIL_LENGTH) > sizeof(this->data) {
      ESP_LOGW(TAG, "ComfoAir message too large");
      return false;
    }

    if (index == COMFOAIR_MSG_HEAD_LENGTH + data_length) {
      // checksum is without checksum bytes
      uint8_t checksum = comfoair_checksum_(this->data_ + 2, COMFOAIR_MSG_HEAD_LENGTH + data_length - 2);
      if (checksum != byte) {
        ESP_LOGW(TAG, "ComfoAir Checksum doesn't match: 0x%02X!=0x%02X", byte, checksum);
        return false;
      }
      return true;
    }

    if (index == COMFOAIR_MSG_HEAD_LENGTH + data_length + 1) {
      return byte == COMFOAIR_MSG_PREFIX;
    }

    if (index == COMFOAIR_MSG_HEAD_LENGTH + data_length + 2) {
      if (byte != COMFOAIR_MSG_TAIL) {
        return false;
      }
    }

    return {};
  }

  void parse_data_() {
    this->status_clear_warning();
    uint8_t *msg = this->data_[COMFOAIR_MSG_HEAD_LENGTH];

    switch (this->data_[COMFOAIR_MSG_IDENTIFIER_IDX]) {
      case COMFOAIR_GET_BOOTLOADER_VERSION_RESPONSE:
        memcpy(bootloader_version_, msg, this->data_[COMFOAIR_MSG_DATA_LENGTH_IDX]);
        break;
      case COMFOAIR_GET_FIRMWARE_VERSION_RESPONSE:
        memcpy(firmware_version_, msg, this->data_[COMFOAIR_MSG_DATA_LENGTH_IDX]);
        break;
      case COMFOAIR_GET_BOARD_VERSION_RESPONSE:
        memcpy(connector_board_version_, msg, this->data_[COMFOAIR_MSG_DATA_LENGTH_IDX]);
        break;
      case COMFOAIR_GET_FAN_STATUS_RESPONSE: {
          if (this->fan_supply_air_percentage_ != nullptr) {
            this->fan_supply_air_percentage_->set_value(msg[0]);
          }
          if (this->fan_exhaust_air_percentage_ != nullptr) {
            this->fan_exhaust_air_percentage_->set_value(msg[1]);
          }
          if (this->fan_speed_air_ != nullptr) {
            this->fan_speed_air_->publish_state(1875000.0f / this->get_uint16_(2));
          }
          if (this->fan_speed_exhaust_ != nullptr) {
            this->fan_speed_exhaust_->publish_state(1875000.0f / this->get_uint16_(4));
          }
          break;
        }
      case COMFOAIR_GET_VALVE_STATUS_RESPONSE: {
        if (this->bypass_valve_percentage_ != nullptr) {
          this->bypass_valve_percentage_->publish_state(msg[0]);
        }
        if (this->preheating_ != nullptr) {
          if (msg[1] == 0x01) {
            this->preheating_->publish_state(true);
          } else if (msg[1] == 0x00) {
            this->preheating_->publish_state(false);
          }
        }
        break;
      }
      case COMFOAIR_GET_TEMPERATURE_RESPONSE: {
          // T1 / outside air
          if (this->temperature_outside_air_sensor_ != nullptr) {
            this->temperature_outside_air_sensor_->publish_state((msg[0] + 20) * 2);
          }
          // T2 / supply air
          if (this->temperature_supply_air_sensor_ != nullptr) {
            this->temperature_supply_air_sensor_->publish_state((msg[1] + 20) * 2);
          }
          // T3 / exhaust air
          if (this->temperature_exhaust_air_sensor_ != nullptr) {
            this->temperature_exhaust_air_sensor_->publish_state((msg[2] + 20) * 2);
          }
          // T4 / exhaust air
          if (this->temperature_continued_air_ != nullptr) {
            this->temperature_continued_air_->publish_state((msg[3] + 20) * 2);
          }
        }
        break;
      }
      case COMFOAIR_GET_SENSOR_DATA_RESPONSE: {

        if (this->temperature_enthalpy_sensor_ != nullptr) {
          this->temperature_enthalpy_sensor_->publish_state((msg[0] + 20) * 2);
        }

        break;
      }
      case COMFOAIR_GET_VENTILATION_LEVEL_RESPONSE: {
        // Fan Speed
        switch(msg[8]) {
          case 0x00:
            this->fan_mode_(climate::CLIMATE_FAN_AUTO);
            break;
          case 0x01:
            this->fan_mode_(climate::CLIMATE_FAN_OFF);
            break;
          case 0x02:
            this->fan_mode_(climate::CLIMATE_FAN_LOW);
            break;
          case 0x03:
            this->fan_mode_(climate::CLIMATE_FAN_MEDIUM);
            break;
          case 0x04:
            this->fan_mode_(climate::CLIMATE_FAN_HIGH);
            break;
        }

        // Supply air fan active (1 = active / 0 = inactive)
        if (this->supply_air_fan_active_ != nullptr) {
          this->supply_air_fan_active_->publish_state(msg[9] == 1);
        }
        break;
      }
      case COMFOAIR_GET_TEMPERATURES_RESPONSE: {
        {
          // comfort temperature
          this->set_target_temperature(msg[0]);

          // T1 / outside air
          if (this->temperature_outside_air_sensor_ != nullptr && msg[5] & 0x01) {
            this->temperature_outside_air_sensor_->publish_state((msg[1] + 20) * 2);
          }
          // T2 / supply air
          if (this->temperature_supply_air_sensor_ != nullptr && msg[5] & 0x02) {
            this->temperature_supply_air_sensor_->publish_state((msg[2] + 20) * 2);
          }
          // T3 / exhaust air
          if (this->temperature_exhaust_air_sensor_ != nullptr && msg[5] & 0x04) {
            this->temperature_exhaust_air_sensor_->publish_state((msg[3] + 20) * 2);
          }
          // T4 / continued air
          if (this->temperature_continued_air_ != nullptr && msg[5] & 0x08) {
            this->temperature_continued_air_->publish_state((msg[4] + 20) * 2);
          }
          // EWT
          if (this->temperature_ewt_sensor_ != nullptr && msg[5] & 0x10) {
            this->temperature_ewt_sensor_->publish_state((msg[6] + 20) * 2);
          }
          // reheating
          if (this->temperature_reheating_sensor_ != nullptr && msg[5] & 0x20) {
            this->temperature_reheating_sensor_->publish_state((msg[7] + 20) * 2);
          }
          // kitchen hood
          if (this->temperature_kitchen_hood_sensor_ != nullptr && msg[5] & 0x40) {
            this->temperature_kitchen_hood_sensor_->publish_state((msg[8] + 20) * 2);
          }

          break;
      }
    }
  }

  void get_fan_status_() {
    if (this->fan_supply_air_percentage_ != nullptr ||
        this->fan_exhaust_air_percentage_ != nullptr ||
        this->fan_speed_air_ != nullptr ||
        this->fan_speed_exhaust_ != nullptr) {
      ESP_LOGD(TAG, "ComfoAir getting fan status");
      this->comfoair_write_command_(COMFOAIR_GET_FAN_STATUS_REQUEST, nullptr, 0);
    }
  }

  void get_valve_status_() {
    if (this->bypass_valve_percentage_ != nullptr ||
        this->preheating_ != nullptr) {
      ESP_LOGD(TAG, "ComfoAir getting valve status");
      this->comfoair_write_command_(COMFOAIR_GET_VALVE_STATUS_REQUEST, nullptr, 0);
    }
  }

  void get_temperature_() {
    if (this->temperature_outside_air_sensor_ != nullptr ||
       this->temperature_supply_air_sensor_ != nullptr ||
       this->temperature_exhaust_air_sensor_ != nullptr ||
       this->temperature_outside_air_sensor_ != nullptr) {
      ESP_LOGD(TAG, "ComfoAir getting temperature");
      this->comfoair_write_command_(COMFOAIR_GET_TEMPERATURE_REQUEST, nullptr, 0);
    }
  }

  void get_sensor_data_() {
    if (this->temperature_enthalpy_sensor_ != nullptr) {
      ESP_LOGD(TAG, "ComfoAir getting sensor data");
      this->comfoair_write_command_(COMFOAIR_GET_SENSOR_DATA_REQUEST, nullptr, 0);
    }
  }

  void get_ventilation_level_() {
    // Always execute as ventilation level is always present
    ESP_LOGD(TAG, "ComfoAir getting ventilation level");
    this->comfoair_write_command_(COMFOAIR_GET_VENTILATION_LEVEL_REQUEST, nullptr, 0);
  }

  void get_temperatures_() {
    // Always execute as comfort temperature is always present
    ESP_LOGD(TAG, "ComfoAir getting temperatures");
    this->comfoair_write_command_(COMFOAIR_GET_TEMPERATURES_REQUEST, nullptr, 0);
  }

  uint8_t get_uint8_t_(uint8_t start_index) const {
    return this->data_[COMFOAIR_MSG_HEAD_LENGTH + start_index];
  }

  uint16_t get_uint16_(uint8_t start_index) const {
    return (uint16_t(this->data_[COMFOAIR_MSG_HEAD_LENGTH + start_index + 1]) << 8) | uint16_t(this->data_[COMFOAIR_MSG_HEAD_LENGTH + start_index]);
  }

  sensor::Sensor *fan_supply_air_percentage_;
  sensor::Sensor *fan_exhaust_air_percentage_;
  sensor::Sensor *fan_speed_air_;
  sensor::Sensor *fan_speed_exhaust_;
  sensor::Sensor *bypass_valve_percentage_;
  sensor::BinarySensor *preheating_;
  sensor::Sensor *temperature_outside_air_sensor_;
  sensor::Sensor *temperature_supply_air_sensor_;
  sensor::Sensor *temperature_exhaust_air_sensor_;
  sensor::Sensor *temperature_continued_air_;
  sensor::Sensor *temperature_enthalpy_sensor_;
  sensor::Sensor *temperature_ewt_sensor_;
  sensor::Sensor *temperature_reheating_sensor;
  sensor::Sensor *temperature_kitchen_hood_sensor_;
  sensor::BinarySensor *supply_air_fan_active_;

  uint8_t data_[30];
  uint8_t data_index_{0};
  uint8_t update_counter_{0};

  uint8_t bootloader_version_[13]{0};
  uint8_t firmware_version_[13]{0};
  uint8_t connector_board_version_[14]{0};
};

}  // namespace comfoair
}  // namespace esphome
