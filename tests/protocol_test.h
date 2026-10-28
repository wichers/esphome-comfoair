#pragma once

#include "esphome/components/comfoair/comfoair.h"
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <deque>
#include <vector>

// Keep checks active even when the firmware build defines NDEBUG.
#define CHECK(condition) do { if (!(condition)) { \
  std::fprintf(stderr, "Protocol check failed at line %d: %s\n", __LINE__, #condition); \
  std::abort(); \
} } while (0)

class TestUART : public esphome::uart::UARTComponent {
 public:
  std::vector<uint8_t> output;
  std::deque<uint8_t> input;
  void write_array(const uint8_t *data, size_t len) override {
    output.insert(output.end(), data, data + len);
  }
  bool peek_byte(uint8_t *data) override {
    if (input.empty()) return false;
    *data = input.front();
    return true;
  }
  bool read_array(uint8_t *data, size_t len) override {
    if (input.size() < len) return false;
    while (len--) { *data++ = input.front(); input.pop_front(); }
    return true;
  }
  size_t available() override { return input.size(); }
  esphome::uart::UARTFlushResult flush() override {
    return esphome::uart::UARTFlushResult::UART_FLUSH_RESULT_SUCCESS;
  }
 protected:
  void check_logger_conflict() override {}
};

class TestComfoAir : public esphome::comfoair::ComfoAirComponent {
 public:
  using ComfoAirComponent::calcChecksum_;
  using ComfoAirComponent::checkRx_;
  using ComfoAirComponent::write_command_;
  using ComfoAirComponent::parseRxMessage_;
  using ComfoAirComponent::bootloader_version_;
  using ComfoAirComponent::firmware_version_;
  using ComfoAirComponent::connector_board_version_;
  using ComfoAirComponent::BusOwner;
  using ComfoAirComponent::bus_owner_;
  using ComfoAirComponent::caRxIdx_u8;
  using ComfoAirComponent::last_command_sent_;
  using ComfoAirComponent::update_counter_;
};

// Display proxy: the unit is on uart, the ComfoSense display on display.
inline void run_proxy_tests() {
  using namespace esphome;
  TestUART unit, display;
  TestComfoAir component;
  component.set_uart_parent(&unit);
  component.set_proxy_uart_component(&display);
  const std::vector<uint8_t> poll_level = {7, 0xF0, 0, CMD_GET_VENTILATION_LEVEL, 0,
      uint8_t(0xAD + CMD_GET_VENTILATION_LEVEL), 7, 0x0F};

  // A poll that cannot be sent keeps its slot. A slot without entities still
  // advances, also after an earlier failed send.
  component.caRxIdx_u8 = 3;  // The unit is sending a frame.
  component.update_counter_ = 3;
  component.update();
  CHECK(unit.output.empty() && component.update_counter_ == 3 && !component.last_command_sent_);
  component.caRxIdx_u8 = 0;
  component.update_counter_ = 0;  // Fan status, but no fan entities.
  component.update();
  CHECK(unit.output.empty() && component.update_counter_ == 1);
  component.update_counter_ = 3;
  component.update();
  CHECK(unit.output == poll_level && component.update_counter_ == 4);
  CHECK(component.bus_owner_ == TestComfoAir::BusOwner::ESP);
}

// Synthetic fixtures based on registers.h; these are not captured unit traffic.
inline void run_decoder_tests() {
  using namespace esphome;
  TestUART uart;
  TestComfoAir component;
  component.set_uart_parent(&uart);
  auto decode = [&](uint8_t command, std::initializer_list<uint8_t> payload) {
    std::vector<uint8_t> message{0, command, uint8_t(payload.size())};
    message.insert(message.end(), payload.begin(), payload.end());
    component.parseRxMessage_(message.data());
  };
  decode(RES_GET_BOOTLOADER_VERSION, {3, 70, 32});
  decode(RES_GET_FIRMWARE_VERSION, {3, 71, 33});
  decode(RES_GET_CONNECTOR_BOARD_VERSION, {2, 12});
  CHECK(component.bootloader_version_[1] == 70);
  CHECK(component.firmware_version_[2] == 33);
  CHECK(component.connector_board_version_[1] == 12);

  sensor::Sensor intake, exhaust, intake_rpm, exhaust_rpm;
  component.intake_fan_speed = &intake;
  component.exhaust_fan_speed = &exhaust;
  component.intake_fan_speed_rpm = &intake_rpm;
  component.exhaust_fan_speed_rpm = &exhaust_rpm;
  decode(RES_GET_FAN_STATUS, {35, 45, 0x03, 0xE8, 0x07, 0xD0});
  CHECK(intake.state == 35 && exhaust.state == 45);
  CHECK(intake_rpm.state == 1875 && exhaust_rpm.state == 937);
  decode(RES_GET_FAN_STATUS, {0, 0, 0, 0, 0, 0});
  CHECK(intake_rpm.state == 0 && exhaust_rpm.state == 0);

  sensor::Sensor bypass, bypass_current, preheat_current;
  binary_sensor::BinarySensor bypass_open, preheat_open;
  component.bypass_valve = &bypass;
  component.bypass_valve_open = &bypass_open;
  component.preheating_state = &preheat_open;
  component.motor_current_bypass = &bypass_current;
  component.motor_current_preheating = &preheat_current;
  decode(RES_GET_VALVE_STATUS, {100, 1, 12, 34});
  CHECK(bypass.state == 100 && bypass_open.state && preheat_open.state);
  CHECK(bypass_current.state == 12 && preheat_current.state == 34);
  decode(RES_GET_VALVE_STATUS, {0, 2, 0, 0});
  CHECK(!bypass_open.state && preheat_open.state);  // Unknown must not become closed.

  sensor::Sensor factor, step, correction;
  binary_sensor::BinarySensor summer;
  component.bypass_factor = &factor;
  component.bypass_step = &step;
  component.bypass_correction = &correction;
  component.summer_mode = &summer;
  decode(RES_GET_BYPASS_CONTROL_STATUS, {0, 0, 10, 20, 30, 0, 1});
  CHECK(factor.state == 10 && step.state == 20 && correction.state == 30);
  CHECK(summer.state);

  sensor::Sensor outside, supply, returning, exiting, enthalpy, ewt, reheating, hood;
  component.outside_air_temperature = &outside;
  component.supply_air_temperature = &supply;
  component.return_air_temperature = &returning;
  component.exhaust_air_temperature = &exiting;
  component.enthalpy_temperature = &enthalpy;
  component.ewt_temperature = &ewt;
  component.reheating_temperature = &reheating;
  component.kitchen_hood_temperature = &hood;
  decode(RES_GET_TEMPERATURE_STATUS, {0, 40, 80, 81});
  CHECK(outside.state == -20 && supply.state == 0);
  CHECK(returning.state == 20 && exiting.state == 20.5f);
  decode(RES_GET_SENSOR_DATA, {90});
  CHECK(enthalpy.state == 25);
  decode(RES_GET_TEMPERATURES, {82, 42, 62, 84, 86, 0x7F, 88, 90, 92});
  CHECK(component.target_temperature == 21 && component.current_temperature == 22);
  CHECK(outside.state == 1 && supply.state == 11 && returning.state == 22);
  CHECK(exiting.state == 23 && ewt.state == 24 && reheating.state == 25 && hood.state == 26);
  decode(RES_GET_TEMPERATURES, {80, 0, 0, 0, 0, 0, 0, 0, 0});
  CHECK(outside.state == 1 && returning.state == 22);  // Presence mask gates updates.

  sensor::Sensor return_level, supply_level, ventilation;
  binary_sensor::BinarySensor supply_active;
  component.return_air_level = &return_level;
  component.supply_air_level = &supply_level;
  component.ventilation_level = &ventilation;
  component.supply_fan_active = &supply_active;
  comfoair::ComfoAirNumber fan_numbers[8];
  component.return_air_level_absent = &fan_numbers[0];
  component.return_air_level_low = &fan_numbers[1];
  component.return_air_level_medium = &fan_numbers[2];
  component.return_air_level_high = &fan_numbers[3];
  component.supply_air_level_absent = &fan_numbers[4];
  component.supply_air_level_low = &fan_numbers[5];
  component.supply_air_level_medium = &fan_numbers[6];
  component.supply_air_level_high = &fan_numbers[7];
  decode(RES_GET_VENTILATION_LEVEL, {15, 35, 50, 16, 36, 51, 40, 41, 2, 1, 70, 71});
  CHECK(return_level.state == 40 && supply_level.state == 41 && ventilation.state == 1);
  CHECK(supply_active.state && component.fan_mode == climate::CLIMATE_FAN_LOW);
  const int expected_numbers[] = {15, 35, 50, 70, 16, 36, 51, 71};
  for (int i = 0; i < 8; ++i) CHECK(fan_numbers[i].state == expected_numbers[i]);
  const climate::ClimateFanMode modes[] = {climate::CLIMATE_FAN_AUTO, climate::CLIMATE_FAN_OFF,
      climate::CLIMATE_FAN_LOW, climate::CLIMATE_FAN_MEDIUM, climate::CLIMATE_FAN_HIGH};
  for (uint8_t level = 0; level < 5; ++level) {
    decode(RES_GET_VENTILATION_LEVEL, {15, 35, 50, 15, 35, 50, 35, 35, level, 1, 70, 70});
    CHECK(ventilation.state == static_cast<int>(level) - 1);
    CHECK(component.fan_mode == modes[level]);
    CHECK(component.mode == (level == 1 ? climate::CLIMATE_MODE_OFF : climate::CLIMATE_MODE_FAN_ONLY));
    uart.output.clear();
    auto call = component.make_call();
    call.set_fan_mode(modes[level]);
    component.control(call);
    CHECK(uart.output == std::vector<uint8_t>({7, 0xF0, 0, CMD_SET_LEVEL, 1,
          level, uint8_t(0xAD + CMD_SET_LEVEL + 1 + level), 7, 0x0F}));
  }
  uart.output.clear();
  auto call = component.make_call();
  call.set_target_temperature(21.5f);
  component.control(call);
  CHECK(uart.output == std::vector<uint8_t>({7, 0xF0, 0, CMD_SET_COMFORT_TEMPERATURE, 1,
        83, uint8_t(0xAD + CMD_SET_COMFORT_TEMPERATURE + 1 + 83), 7, 0x0F}));
  uart.output.clear();
  component.set_level(-1);
  component.set_level(5);
  component.set_comfort_temperature(11.5f);
  component.set_comfort_temperature(29.5f);
  CHECK(uart.output.empty());

  text_sensor::TextSensor filter, type, size, valve, frost_level;
  binary_sensor::BinarySensor full, preheat_active, frost_active, preheat_present, bypass_present;
  binary_sensor::BinarySensor fireplace, kitchen, postheat, pwm, p10, p17, p18, p90, p97;
  component.filter_status = &filter;
  component.filter_status_full = &full;
  decode(RES_GET_FAULTS, {0, 0, 0, 0, 0, 0, 0, 0, 1});
  CHECK(filter.state == "Full" && full.state);
  decode(RES_GET_FAULTS, {0, 0, 0, 0, 0, 0, 0, 0, 2});
  CHECK(filter.state == "Unknown" && full.state);
  decode(RES_GET_FAULTS, {0, 0, 0, 0, 0, 0, 0, 0, 0});
  CHECK(filter.state == "Ok" && !full.state);

  // Fault bits: A1-A8 (byte 0), E1-E8 (byte 1), EA1-EA8 (byte 9), A9-A15 and A0 (byte 13).
  // The 0xFF filler after the payload shows reads past the declared length.
  auto decode_in_buffer = [&](uint8_t command, std::initializer_list<uint8_t> payload) {
    std::vector<uint8_t> message(comfoair::MAX_MESSAGE_SIZE, 0xFF);
    message[0] = 0;
    message[1] = command;
    message[2] = uint8_t(payload.size());
    std::copy(payload.begin(), payload.end(), message.begin() + 3);
    component.parseRxMessage_(message.data());
  };
  text_sensor::TextSensor errors;
  component.error_status = &errors;
  decode_in_buffer(RES_GET_FAULTS, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
  CHECK(errors.state == "None");
  decode_in_buffer(RES_GET_FAULTS, {0x81, 0x02, 0, 0, 0, 0, 0, 0, 0, 0x04, 0, 0, 0, 0xC1, 0, 0, 0});
  CHECK(errors.state == "A1, A8, E2, EA3, A9, A15, A0");
  decode_in_buffer(RES_GET_FAULTS, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01, 0, 0, 0});
  CHECK(errors.state == "A9");
  decode_in_buffer(RES_GET_FAULTS, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x40, 0, 0, 0});
  CHECK(errors.state == "A15");
  decode_in_buffer(RES_GET_FAULTS, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0, 0, 0});
  CHECK(errors.state == "A0");
  // Without the optional bytes, EA and high A are not read.
  decode_in_buffer(RES_GET_FAULTS, {0, 0x01, 0, 0, 0, 0, 0, 0, 0, 0x01});
  CHECK(errors.state == "E1, EA1");
  decode_in_buffer(RES_GET_FAULTS, {0x01, 0, 0, 0, 0, 0, 0, 0, 0});
  CHECK(errors.state == "A1" && filter.state == "Ok" && !full.state);

  // A short fault payload in a valid frame gets an ACK but changes no state.
  for (uint8_t length : {0, 8}) {
    const std::vector<uint8_t> payload(length, 0x01);
    uart.output.clear();
    component.write_command_(RES_GET_FAULTS, payload.data(), length);
    uart.input.assign(uart.output.begin(), uart.output.end());
    uart.output.clear();
    component.loop();
    CHECK(uart.output == std::vector<uint8_t>({7, 0xF3}));
    CHECK(errors.state == "A1" && filter.state == "Ok" && !full.state);
  }

  // The reset buttons send command 0xDB with different payloads.
  comfoair::ComfoAirFilterResetButton filter_reset;
  comfoair::ComfoAirErrorResetButton error_reset;
  component.set_filter_reset(&filter_reset);
  component.set_error_reset(&error_reset);
  uart.output.clear();
  filter_reset.press();
  CHECK(uart.output == std::vector<uint8_t>({7, 0xF0, 0, CMD_RESET_AND_SELF_TEST, 4,
        0, 0, 0, 1, uint8_t(0xAD + CMD_RESET_AND_SELF_TEST + 4 + 1), 7, 0x0F}));
  uart.output.clear();
  error_reset.press();
  CHECK(uart.output == std::vector<uint8_t>({7, 0xF0, 0, CMD_RESET_AND_SELF_TEST, 4,
        1, 0, 0, 0, uint8_t(0xAD + CMD_RESET_AND_SELF_TEST + 4 + 1), 7, 0x0F}));

  // Update slot 5 polls the faults when only error_status is configured.
  {
    TestUART poll_uart;
    TestComfoAir poller;
    poller.set_uart_parent(&poll_uart);
    poller.update_counter_ = 5;
    poller.update();
    CHECK(poll_uart.output.empty());
    text_sensor::TextSensor only_errors;
    poller.error_status = &only_errors;
    poller.update_counter_ = 5;
    poller.update();
    CHECK(poll_uart.output == std::vector<uint8_t>({7, 0xF0, 0, CMD_GET_FAULTS, 0,
          uint8_t(0xAD + CMD_GET_FAULTS), 7, 0x0F}));
  }
  component.type = &type;
  component.size = &size;
  component.preheating_present = &preheat_present;
  component.bypass_present = &bypass_present;
  component.fireplace_present = &fireplace;
  component.kitchen_hood_present = &kitchen;
  component.postheating_present = &postheat;
  component.postheating_pwm_mode_present = &pwm;
  component.p10_active = &p10;
  component.p17_active = &p17;
  component.p18_active = &p18;
  component.p90_active = &p90;
  component.p97_active = &p97;
  decode(RES_GET_STATUS, {1, 1, 1, 1, 0x47, 0, 0x81, 1, 0x81, 0, 0});
  CHECK(type.state == "Left");
  CHECK(preheat_present.state && bypass_present.state && fireplace.state);
  CHECK(kitchen.state && postheat.state && pwm.state);
  CHECK(p10.state && p17.state && p18.state && p90.state && p97.state);
  decode(RES_GET_STATUS, {0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0});
  CHECK(type.state == "Right" && !p10.state && !p97.state && !fireplace.state);

  sensor::Sensor hours[8];
  component.level0_hours = &hours[0]; component.level1_hours = &hours[1];
  component.level2_hours = &hours[2]; component.level3_hours = &hours[3];
  component.frost_protection_hours = &hours[4]; component.preheating_hours = &hours[5];
  component.bypass_open_hours = &hours[6]; component.filter_hours = &hours[7];
  decode(RES_GET_OPERATION_HOURS, {1, 2, 3, 0, 0, 4, 0, 1, 0, 1, 2, 2, 3, 3, 4, 4, 5, 2, 3, 4});
  const int expected_hours[] = {66051, 4, 256, 131844, 258, 515, 772, 1029};
  for (int i = 0; i < 8; ++i) CHECK(hours[i].state == expected_hours[i]);
  sensor::Sensor minutes;
  component.preheating_valve = &valve;
  component.preheating_active = &preheat_active;
  component.frost_protection_active = &frost_active;
  component.frost_protection_level = &frost_level;
  component.frost_protection_minutes = &minutes;
  decode(RES_GET_PREHEATING_STATUS, {1, 1, 1, 1, 2, 2});
  CHECK(valve.state == "Open" && preheat_active.state && frost_active.state);
  CHECK(minutes.state == 258 && frost_level.state == "NominalProtection");
  decode(RES_GET_PREHEATING_STATUS, {2, 0, 2, 0, 0, 99});
  CHECK(valve.state == "Unknown" && preheat_active.state && !frost_active.state);
  CHECK(frost_level.state == "Unknown");
  sensor::Sensor delays[8];
  component.bathroom_switch_on_delay_minutes = &delays[0];
  component.bathroom_switch_off_delay_minutes = &delays[1];
  component.l1_switch_off_delay_minutes = &delays[2];
  component.boost_ventilation_minutes = &delays[3];
  component.filter_warning_weeks = &delays[4];
  component.rf_high_time_short_minutes = &delays[5];
  component.rf_high_time_long_minutes = &delays[6];
  component.extractor_hood_switch_off_delay_minutes = &delays[7];
  decode(RES_GET_TIME_DELAY, {1, 2, 3, 4, 5, 6, 7, 8});
  for (int i = 0; i < 8; ++i) CHECK(delays[i].state == i + 1);
}

inline void run_protocol_tests() {
  using namespace esphome::comfoair;
  TestUART uart;
  TestComfoAir component;
  component.set_uart_parent(&uart);
  struct {
    uint8_t before{0xA5};
    uint8_t data[MAX_MESSAGE_SIZE]{};
    uint8_t after{0x5A};
  } guarded;
  uint8_t *buffer = guarded.data;
  uint8_t index = 0;
  auto feed = [&](const std::vector<uint8_t> &bytes) {
    rx_status status = RX_STATUS_DEFAULT;
    for (uint8_t byte : bytes) {
      status = component.checkRx_(buffer, &index, byte);
      CHECK(index < MAX_MESSAGE_SIZE);
      CHECK(guarded.before == 0xA5 && guarded.after == 0x5A);
    }
    return status;
  };

  uint8_t message[] = {0, 0x10, 1, 0x07};
  CHECK(component.calcChecksum_(message) == 0xC5);
  component.write_command_(0x10, message + 3, 1);
  const std::vector<uint8_t> frame = {7, 0xF0, 0, 0x10, 1, 7, 7, 0xC5, 7, 0x0F};
  CHECK(uart.output == frame);
  CHECK(feed(frame) == RX_STATUS_RECEIVED_MESSAGE);
  CHECK(buffer[3] == 7);

  auto corrupt = frame;
  corrupt[7] ^= 1;
  CHECK(feed(corrupt) == RX_STATUS_RECEIVED_INVALID_MESSAGE);
  CHECK(index == 0);
  CHECK(feed({7, 0xF0, 0, 0x10}) != RX_STATUS_RECEIVED_MESSAGE);
  CHECK(feed(frame) == RX_STATUS_RECEIVED_MESSAGE);
  auto wrong_length = frame;
  wrong_length[4] = 2;
  CHECK(feed(wrong_length) == RX_STATUS_RECEIVED_INVALID_MESSAGE);

  // Checksum 0x07 is accepted both escaped and raw.
  uart.output.clear();
  component.write_command_(0x5A, nullptr, 0);
  CHECK(uart.output == std::vector<uint8_t>({7, 0xF0, 0, 0x5A, 0, 7, 7, 7, 0x0F}));
  CHECK(feed(uart.output) == RX_STATUS_RECEIVED_MESSAGE);
  CHECK(feed({7, 0xF0, 0, 0x5A, 0, 7, 7, 0x0F}) == RX_STATUS_RECEIVED_MESSAGE);

  // Worst case: all payload bytes and the checksum need escaping.
  std::vector<uint8_t> payload(MAX_PROTOCOL_DATA_SIZE, 7);
  uart.output.clear();
  component.write_command_(0x6A, payload.data(), payload.size());
  CHECK(uart.output.size() == 69);
  CHECK(uart.output.size() <= MAX_MESSAGE_SIZE);
  CHECK(feed(uart.output) == RX_STATUS_RECEIVED_MESSAGE);
  CHECK(buffer[2] == MAX_PROTOCOL_DATA_SIZE);
  uart.output.clear();
  payload.push_back(7);
  component.write_command_(0x10, payload.data(), payload.size());
  CHECK(uart.output.empty());
  CHECK(feed({7, 0xF0, 0, 0x10, uint8_t(MAX_PROTOCOL_DATA_SIZE + 1)}) ==
        RX_STATUS_RECEIVED_INVALID_MESSAGE);
  CHECK(index == 0);
  // Repeated wraps must stay in bounds and allow the next frame to resynchronize.
  feed(std::vector<uint8_t>(MAX_MESSAGE_SIZE * 4, 0x55));
  CHECK(feed(frame) == RX_STATUS_RECEIVED_MESSAGE);
  CHECK(feed({7, 0xF3}) == RX_STATUS_RECEIVED_ACK);
  CHECK(feed({0x55, 7, 0xF3}) == RX_STATUS_RECEIVED_ACK);
  CHECK(index == 0);

  // Exercise the real loop: only valid messages get an ACK; ACKs are not echoed.
  for (const auto &bytes : {frame, corrupt, std::vector<uint8_t>{7, 0xF3},
                            std::vector<uint8_t>{7, 0xF0, 0, 0x10}}) {
    uart.output.clear();
    uart.input.assign(bytes.begin(), bytes.end());
    component.loop();
    if (bytes == frame) CHECK(uart.output == std::vector<uint8_t>({7, 0xF3}));
    else CHECK(uart.output.empty());
  }
  run_decoder_tests();
  run_proxy_tests();
  std::puts("ComfoAir protocol and decoder tests passed");
}

#undef CHECK
