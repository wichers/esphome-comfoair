#pragma once

#include "esphome/components/comfoair/comfoair.h"
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
};

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
  std::puts("ComfoAir protocol tests passed");
}

#undef CHECK
