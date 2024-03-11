#ifndef __messages_h__
#define __messages_h__

#include <stdint.h>

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
static const uint8_t COMFOAIR_GET_SENSOR_DATA_LENGTH = 0x11;
static const uint8_t COMFOAIR_GET_ANALOG_STATUS_REQUEST = 0x9d;
static const uint8_t COMFOAIR_GET_ANALOG_STATUS_RESPONSE = 0x9e;
static const uint8_t COMFOAIR_GET_ANALOG_STATUS_LENGTH = 0x0a;
static const uint8_t COMFOAIR_GET_DELAYS_REQUEST = 0xc9;
static const uint8_t COMFOAIR_GET_DELAYS_RESPONSE = 0xca;
static const uint8_t COMFOAIR_GET_DELAYS_LENGTH = 0x08;
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
static const uint8_t COMFOAIR_SET_RESET_REQUEST = 0xdb;
static const uint8_t COMFOAIR_SET_RESET_LENGTH = 0x04;
static const uint8_t COMFOAIR_SET_EWT_REHEATER_REQUEST = 0xed;
static const uint8_t COMFOAIR_SET_EWT_REHEATER_LENGTH = 0x05;

// Specials setters
static const uint8_t COMFOAIR_SET_TEST_MODE_START_REQUEST = 0x01;
static const uint8_t COMFOAIR_SET_TEST_MODE_END_REQUEST = 0x19;
static const uint8_t COMFOAIR_SET_OUTPUTS_REQUEST = 0x05;
static const uint8_t COMFOAIR_SET_ANALOG_OUTPUTS_REQUEST = 0x07;
static const uint8_t COMFOAIR_SET_VALVES_REQUEST = 0x09;

} //namespace comfoair
} //namespace esphome

#endif //__messages_h__