#pragma once

// =================================================
// ===================== CORE ======================
// =================================================

#define COMMAND_PREFIX 0x07
#define COMMAND_HEAD 0xF0
#define COMMAND_LEN_HEAD 5
#define COMMAND_TAIL 0x0F
#define COMMAND_LEN_TAIL 3
#define COMMAND_ACK 0xF3
#define COMMAND_ID_ACK 1
#define COMMAND_IDX_DATA 4
#define COMMAND_IDX_MSG_ID 3

// =================================================
// ========== Bootloader (PC an ComfoAir) ==========
// =================================================

/*
Data: -

Response: -
Confirmation of bootloader mode
*/
#define CMD_SET_BOOTLOADER_MODE 0x65
#define RES_SET_BOOTLOADER_MODE 0x66

/*
Data: 67 bytes
Byte[0] = Data byte 1
Byte[1..62] = ...
Byte[63] = Data byte 64
Byte[64] = Start address (high byte)
Byte[65] = Start address
Byte[66] = Start address (low byte)

Response: 1 byte
Byte[0] = status
    0x01 = success
    0x02 = Error
    0x04 = Address out of range
    0x08 = data block incomplete
*/
#define CMD_WRITE_FLASH_BLOCK 0x6F
#define RES_WRITE_FLASH_BLOCK 0x70

/*
Data: 3 bytes
Byte[0] = Start address (high byte)
Byte[1] = Start address
Byte[2] = Start address (low byte)

Response: 64 bytes
Byte[0] = Data byte 1
Byte[1..62] = ...
Byte[63] = Data byte 64
*/
#define CMD_READ_FLASH_BLOCK 0x71
#define RES_READ_FLASH_BLOCK 0x72

/*
Data: -

Response: -
Confirmation of end of bootloader mode
*/
#define CMD_EXIT_BOOTLOADER_MODE 0x73
#define RES_EXIT_BOOTLOADER_MODE 0x74

// =============================================================
// ========== Command list (PC to ComfoAir) / General ==========
// =============================================================

/*
Data: -

Response: 13 bytes
Byte[0] = Version Major
Byte[1] = Version Minor
Byte[2] = Beta
Byte[3-12] = Devicename (ASCII String)
*/
#define CMD_GET_BOOTLOADER_VERSION 0x67
#define RES_GET_BOOTLOADER_VERSION 0x68

/*
Data: -

Response: 13 bytes
Byte[0] = Version Major
Byte[1] = Version Minor
Byte[2] = Beta
Byte[3-12] = Devicename (ASCII String)
*/
#define CMD_GET_FIRMWARE_VERSION 0x69
#define RES_GET_FIRMWARE_VERSION 0x6A

/*
Data: -

Response: 14 bytes
Byte[0] = Version Major
Byte[1] = Version Minor
Byte[2-11] = Devicename (ASCII String)
Byte[12] = Version CC-Ease
    Bit[7..4] Version Major
    Bit[3..0] = Version Minor
Byte[13] = Version CC-Luxe
    Bit[7..4] Version Major
    Bit[3..0] = Version Minor
*/
#define CMD_GET_CONNECTOR_BOARD_VERSION 0xA1
#define RES_GET_CONNECTOR_BOARD_VERSION 0xA2

/*
Data: 1 byte
Byte[0] = 0x00 = end
          0x01 = PC only
          0x03 = PC master
          0x04 = PC log mode

Response: 1 byte
Byte[0] = 0x00 = Without connection
          0x01 = PC only
          0x02 = CC Ease only
          0x03 = PC master
          0x04 = PC log mode
*/
#define CMD_SET_RS232_MODE 0x9B
#define RES_SET_RS232_MODE 0x9C

// ======================================================================
// ========== Command list (PC to ComfoAir) / reading commands ==========
// ======================================================================

/*
Data: -

Response: 2 bytes
Byte[0] = Step switch: (1 = active / 0 = inactive)
    0x01 = L1
    0x02 = L2
Byte[1] = Switching inputs: (1 = active / 0 = inactive)
    0x01 = bathroom switch
    0x02 = kitchen hood switch
    0x04 = External filter
    0x08 = heat recovery (WTW)
    0x10 = bathroom switch 2 (luxe)
*/
#define CMD_GET_INPUTS 0x03
#define RES_GET_INPUTS 0x04

/*
Data: -

Response: 6 bytes
Byte[0] = Supply air (%)
Byte[1] = Exhaust air (%)
Byte[2..3] = Supply air fan speed (rpm**)
Byte[4..5] = Exhaust air fan speed (rpm**)
*/
#define CMD_GET_FAN_STATUS 0x0B
#define RES_GET_FAN_STATUS 0x0C

/*
Data: -

Response: 4 bytes
Byte[0] = Bypass (%) (0xFF = undefined)
Byte[1] = Preheating (1 = Open / 0 = Closed / 2 = Unknown)
Byte[2] = Bypass motor current (ADC raw data)
Byte[3] = Preheating motor current (ADC raw data)
*/
#define CMD_GET_VALVE_STATUS 0x0D
#define RES_GET_VALVE_STATUS 0x0E

/*
Data: -

Response: 4 bytes
Byte[0] = T1 / outside air (°C*)
Byte[1] = T2 / supply air (°C*)
Byte[2] = T3 / exhaust air (in) (°C*)
Byte[3] = T4 / exhaust air (out) (°C*)
*/
#define CMD_GET_TEMPERATURE_STATUS 0x0F
#define RES_GET_TEMPERATURE_STATUS 0x10

/*
Data: -

Response: 1 byte
Byte[0] = 0x00 = Nothing pressed
          0xFF = Error
*/
#define CMD_GET_BUTTON_STATUS 0x11
#define RES_GET_BUTTON_STATUS 0x12

/*
Data: -

Response: 4 bytes
Byte[0] = Analog 1 (0..255 = 0..10V)
Byte[1] = Analog 2 (0..255 = 0..10V)
Byte[2] = Analog 3 (0..255 = 0..10V)
Byte[3] = Analog 4 (0..255 = 0..10V)
*/
#define CMD_GET_ANALOG_INPUTS 0x13
#define RES_GET_ANALOG_INPUTS 0x14

/*
Data: -

Response: 17 bytes
Byte[0] = Enthalpy sensor temperature (°C*)
Byte[1] = Enthalpy sensor humidity (%)
Byte[2] = Analog 1 (%)
Byte[3] = Analog 2 (%)
Byte[4] = Enthalpy coefficient (%)
Byte[5] = Enthalpy Timer (0..240 = 0..2880 Min)
Byte[6] = 0x00
Byte[7] = Analog 1 to desired (%)
Byte[8] = Analog 1 from desired (%)
Byte[9] = Analog 2 to desired (%)
Byte[10] = Analog 2 from desired (%)
Byte[11] = Analog 3 (%)
Byte[12] = Analog 4 (%)
Byte[13] = Analog 3 to desired (%)
Byte[14] = Analog 3 from desired (%)
Byte[15] = Analog 4 to desired (%)
Byte[16] = Analog 4 from desired (%)
*/
#define CMD_GET_SENSOR_DATA 0x97
#define RES_GET_SENSOR_DATA 0x98

/*
Data: -

Response: 19 bytes
Byte[0] = Analogously present: (1 = present / 0 = absent)
    0x01 = Analog 1
    0x02 = Analog 2
    0x04 = Analog 3
    0x08 = Analog 4
    0x10 = RF
Byte[1] = Regulate / control analogously: (1 = regulate / 0 = control)
    0x01 = Analog 1
    0x02 = Analog 2
    0x04 = Analog 3
    0x08 = Analog 4
    0x10 = RF
Byte[2] = Analogue positive / negative: (1 = negative / 0 = positive)
    0x01 = Analog 1
    0x02 = Analog 2
    0x04 = Analog 3
    0x08 = Analog 4
    0x10 = RF
Byte[3] = Analog 1 Min. Setting (%)
Byte[4] = Analog 1 Max. Setting (%)
Byte[5] = Analog 1 setpoint (%)
Byte[6] = Analogue 2 min. setting (%)
Byte[7] = Analog 2 Max. Setting (%)
Byte[8] = Analog 2 setpoint (%)
Byte[9] = Analogue 3 min. setting (%)
Byte[10] = Analog 3 Max. Setting (%)
Byte[11] = Analog 3 setpoint (%)
Byte[12] = Analogue 4 min. setting (%)
Byte[13] = Analog 4 Max Setting (%)
Byte[14] = Analog 4 setpoint (%)
Byte[15] = Analog RF Min. Setting (%)
Byte[16] = Analog RF Max Setting (%)
Byte[17] = Analog RF setpoint (%)
Byte[18] = Priority control (0 = analog inputs / 1 = weekly program)
*/
#define CMD_GET_ANALOG_VALUES 0x9D
#define RES_GET_ANALOG_VALUES 0x9E

/*
Data: -

Response: 8 bytes
Byte[0] = Bathroom switch switch-on delay (min)
Byte[1] = Bathroom switch switch-off delay (min)
Byte[2] = L1 switch-off delay (min)
Byte[3] = Shock ventilation (min)
Byte[4] = Filter counter (weeks)
Byte[5] = RF high time short (min)
Byte[6] = RF high time long (min)
Byte[7] = Kitchen hood switch-off delay (min)
*/
#define CMD_GET_TIME_DELAY 0xC9
#define RES_GET_TIME_DELAY 0xCA

/*
Data: -

Response: 14 bytes
Byte[0] = Exhaust air absent (%)
Byte[1] = Exhaust air low / level 1 (%)
Byte[2] = Exhaust air medium / level 2 (%)
Byte[3] = Supply air level absent (%)
Byte[4] = Supply air low / level 1 (%)
Byte[5] = Supply air medium / level 2 (%)
Byte[6] = Current exhaust air (%)
Byte[7] = Current supply air (%)
Byte[8] = Current level (see command 0x00 0x99)
Byte[9] = Supply air fan active (1 = active / 0 = inactive)
Byte[10] = Exhaust air high / level 3 (%)
Byte[11] = Supply air high / level 3 (%)
Byte[12] = ..
Byte[13] = ..
*/
#define CMD_GET_VENTILATION_LEVEL 0xCD
#define RES_GET_VENTILATION_LEVEL 0xCE

/*
Data: -

Response: 9 bytes
Byte[0] = Comfort temperature (°C*)
Byte[1] = T1 / outside air (°C*)
Byte[2] = T2 / supply air (°C*)
Byte[3] = T3 / exhaust air (in) (°C*)
Byte[4] = T4 / exhaust air (out) (°C*)
Byte[5] = Sensor present: (1 = present / 0 = absent)
    0x01 = T1 / outside air
    0x02 = T2 / supply air
    0x04 = T3 / exhaust air (in)
    0x08 = T4 / exhaust air (out)
    0x10 = EWT
    0x20 = post-heating
    0x40 = kitchen hood
Byte[6] = Temperature EWT (°C*)
Byte[7] = Temperature afterheating (°C*)
Byte[8] = Temperature kitchen hood (°C*)
*/
#define CMD_GET_TEMPERATURES 0xD1
#define RES_GET_TEMPERATURES 0xD2

/*
Data: -

Response: 11 bytes
Byte[0] = Preheater present (1 = present / 0 = absent)
Byte[1] = Bypass present (1 = present / 0 = absent)
Byte[2] = Type (1 = left / 2 = right)
Byte[3] = Size (1 = large / 2 = small)
Byte[4] = Options: (1 = present / 0 = absent)
    0x01 = fireplace
    0x02 = kitchen hood
    0x04 = post-heating
    0x40 = Afterheating PWM mode
    0x80 =
Byte[5] = 0x00
Byte[6] = Active status 1:
    0x01 = P10
    :
    0x80 = P17
Byte[7] = Active status 2:
    0x01 = P18
    0x02 = P19
Byte[8] = Active status 3:
    0x01 = P90
    :
    0x40 = P96
Byte[9] = Enthalpy present (1 = present / 0 = absent / 2 = without Sensor)
Byte[10] = EWT present (1 = regulated / 0 = absent / 2 = unregulated)

*/
#define CMD_GET_STATUS 0xD5
#define RES_GET_STATUS 0xD6

/*
Data: -

Response: 17 bytes
Byte[0] = Current error A:
    0x01 = A1
    :
    0x80 = A8
Byte[1] = Current error E:
    0x01 = E1
    :
    0x80 = E8
Byte[2] = Last mistake A
    0x01 = A1
    :
    0x80 = A8
Byte[3] = Last mistake E
    0x01 = E1
    :
    0x80 = E8
Byte[4] = Penultimate mistake A
    0x01 = A1
    :
    0x80 = A8
Byte[5] = Penultimate error E
    0x01 = E1
    :
    0x80 = E8
Byte[6] = Penultimate error A
    0x01 = A1
    :
    0x80 = A8
Byte[7] = Penultimate error E
    0x01 = E1
    :
    0x80 = E8
Byte[8] = 0x00 = Filter OK
          0x01 = Filter full
Byte[9] = Current error EA:
    0x01 = EA1
    :
    0x80 = EA8
Byte[10] = Last error EA:
    0x01 = EA1
    :
    0x80 = EA8
Byte[11] = Second to last error EA:
    0x01 = EA1
    :
    0x80 = EA8
Byte[12] = Last but one error EA:
    0x01 = EA1
    :
    0x80 = EA8
Byte[13] = Current error A (high):
    0x01 = A9
    :
    0x40 = A15
    0x80 = A0
Byte[14] = Last error A (high):
    0x01 = A9
    :
    0x40 = A15
    0x80 = A0
Byte[15] = Second to last error A (high):
    0x01 = A9
    :
    0x40 = A15
    0x80 = A0
Byte[16] = Last but one error A (high):
    0x01 = A9
    :
    0x40 = A15
    0x80 = A0
*/
#define CMD_GET_FAULTS 0xD9
#define RES_GET_FAULTS 0xDA

/*
Data: -

Response: 20 bytes
Bytes[0-2] = Operating hours absent (h) (Byte[2] = Low Byte)
Bytes[3-5] = Operating hours low / level 1 (h) (Byte[5] = Low Byte)
Bytes[6-8] = Operating hours medium / level 2 (h) (Byte[8] = Low Byte)
Bytes[9-10] = Operating hours of frost protection (h) (Byte[10] = Low Byte)
Bytes[11-12] = Operating hours preheating (h) (Byte[12] = Low Byte)
Bytes[13-14] = Operating hours bypass open (h) (Byte[14] = Low Byte)
Bytes[15-16] = Filter operating hours (h) (Byte[16] = Low Byte)
Bytes[17-19] = Operating hours high / level 3 (h) (Byte[19] = Low Byte)
*/
#define CMD_GET_OPERATION_HOURS 0xDD
#define RES_GET_OPERATION_HOURS 0xDE

/*
Data: -

Response: 7 bytes
Byte[0] = 0x00
Byte[1] = 0x00
Byte[2] = Bypass factor
Byte[3] = Bypass stage
Byte[4] = Bypass correction
Byte[5] = 0x00
Byte[6] = Summer mode (1 = yes / 0 = no (winter))
*/
#define CMD_GET_BYPASS_CONTROL_STATUS 0xDF
#define RES_GET_BYPASS_CONTROL_STATUS 0xE0

/*
Data: -

Response: 6 bytes
Byte[0] = Status valve (1 = open / 0 = closed / 2 = unknown)
Byte[1] = Frost protection (1 = active / 0 = inactive)
Byte[2] = Preheating (1 = active / 0 = inactive)
Byte[3..4] = Frost minutes (min)
Byte[5] = Frost protection (1 = extra safe / 4 = safe)
*/
#define CMD_GET_PREHEATING_STATUS 0xE1
#define RES_GET_PREHEATING_STATUS 0xE2

/*
Data: -

Response: 7 bytes
Byte[0] = RF address 4 (low byte)
Byte[1] = RF address 3
Byte[2] = RF address 2
Byte[3] = RF address 1 (high byte)
Byte[4] = RF ID
Byte[5] = Module present
Byte[6] = Self-learning mode active
*/
#define CMD_GET_RF_STATUS 0xE5
#define RES_GET_RF_STATUS 0xE6

/*
Data: -

Response: 8 bytes
Byte[0] = Oldest value (°C)
Byte[1] =
Byte[2] =
Byte[3] =
Byte[4] =
Byte[5] =
Byte[6] =
Byte[7] = Latest value (°C)
*/
#define CMD_GET_LAST_8_TIMES_PREHEATING 0xE9
#define RES_GET_LAST_8_TIMES_PREHEATING 0xEA

/*
Data: -

Response: 7 byets
Byte[0] = EWT low (°C)
Byte[1] = EWT high (°C)
Byte[2] = EWT speed up (%)
Byte[3] = Kitchen hood speed up (%)
Byte[4] = Post-heating performance
Byte[5] = Post-heating power I parameter
Byte[6] = Reheating T desired (°C)
*/
#define CMD_EWT_POST_HEATING 0xEB
#define RES_EWT_POST_HEATING 0xEC

// ====================================================================
// ========== Command list (PC to ComfoAir) / write commands ==========
// ====================================================================

/*
Data: 1 byte
Byte[0] = 0x00 = Auto
          0x01 = Absent
          0x02 = low / level 1
          0x03 = medium / level 2
          0x04 = high / level 3

Response: ACK
*/
#define CMD_SET_LEVEL 0x99
#define RES_SET_LEVEL COMMAND_ACK

/*
Data: 19 bytes
Byte[0] = Analogously present: (1 = present / 0 = absent)
    0x01 = Analog 1
    0x02 = Analog 2
    0x04 = Analog 3
    0x08 = Analog 4
    0x10 = RF
Byte[1] = Regulate / control analogously: (1 = regulate / 0 = control)
    0x01 = Analog 1
    0x02 = Analog 2
    0x04 = Analog 3
    0x08 = Analog 4
    0x10 = RF
Byte[2] = Analogue positive / negative: (1 = negative / 0 = positive)
    0x01 = Analog 1
    0x02 = Analog 2
    0x04 = Analog 3
    0x08 = Analog 4
    0x10 = RF
Byte[3] = Analog 1 Min. Setting (%)
Byte[4] = Analog 1 Max. Setting (%)
Byte[5] = Analog 1 setpoint (%)
Byte[6] = Analogue 2 min. setting (%)
Byte[7] = Analog 2 Max. Setting (%)
Byte[8] = Analog 2 setpoint (%)
Byte[9] = Analogue 3 min. setting (%)
Byte[10] = Analog 3 Max. Setting (%)
Byte[11] = Analog 3 setpoint (%)
Byte[12] = Analogue 4 min. setting (%)
Byte[13] = Analog 4 Max Setting (%)
Byte[14] = Analog 4 setpoint (%)
Byte[15] = Analog RF Min. Setting (%)
Byte[16] = Analog RF Max Setting (%)
Byte[17] = Analog RF setpoint (%)
Byte[18] = Priority control (0 = analog inputs / 1 = weekly program)

Response: ACK
*/
#define CMD_SET_ANALOG_VALUES 0x9F
#define RES_SET_ANALOG_VALUES COMMAND_ACK

/*
Data: 8 bytes
Byte[0] = Bathroom switch switch-on delay (min)
Byte[1] = Bathroom switch switch-off delay (min)
Byte[2] = L1 switch-off delay (min)
Byte[3] = Shock ventilation (min)
Byte[4] = Filter counter (weeks)
Byte[5] = RF high time short (min)
Byte[6] = RF high time long (min)
Byte[7] = Kitchen hood switch-off delay (min)

Response: ACK
*/
#define CMD_SET_TIME_DELAY 0xCB
#define RES_SET_TIME_DELAY COMMAND_ACK

/*
Data: 9 bytes
Byte[0] = Exhaust air absent (%)
Byte[1] = Exhaust air low / level 1 (%)
Byte[2] = Exhaust air medium / level 2 (%)
Byte[3] = Supply air level absent (%)
Byte[4] = Supply air low / level 1 (%)
Byte[5] = Supply air medium / level 2 (%)
Byte[6] = Exhaust air high / level 3 (%)
Byte[7] = Supply air high / level 3 (%)
Byte[8] =

Response: ACK
*/
#define CMD_SET_VENTILATION_LEVEL 0xCF
#define RES_SET_VENTILATION_LEVEL COMMAND_ACK

/*
Data: 1 byte
Byte[0] = Comfort temperature (°C*)

Response: ACK
*/
#define CMD_SET_COMFORT_TEMPERATURE 0xD3
#define RES_SET_COMFORT_TEMPERATURE COMMAND_ACK

/*
Data: 8 bytes
Byte[0] = Preheater present (1 = present / 0 = absent)
Byte[1] = Bypass present (1 = present / 0 = absent)
Byte[2] = Type (1 = left / 2 = right)
Byte[3] = Size (1 = large / 2 = small)
Byte[4] = Options: (1 = present / 0 = absent)
    0x01 = fireplace
    0x02 = kitchen hood
    0x04 = post-heating
    0x40 = Afterheating PWM mode
    0x80 =
Byte[5] = 0x00
Byte[6] = Enthalpy present (1 = present / 0 = absent / 2 = without Sensor)
Byte[7] = EWT present (1 = regulated / 0 = absent / 2 = unregulated)

Response: ACK
*/
#define CMD_SET_STATUS 0xD7
#define RES_SET_STATUS COMMAND_ACK

/*
Data: 4 bytes
Byte[0] = Reset faults (1 = reset / 0 = -)
Byte[1] = Reset settings (1 = reset / 0 = -)
Byte[2] = Start self-test (1 = start / 0 = -)
Byte[3] = Reset filter operating hours (1 = reset / 0 = -)

Response: ACK
*/
#define CMD_RESET_AND_SELF_TEST 0xDB
#define RES_RESET_AND_SELF_TEST COMMAND_ACK

/*
Data: 5 bytes
Byte[0] = EWT low (°C)
Byte[1] = EWT high (°C)
Byte[2] = EWT speed up (%)
Byte[3] = Kitchen hood speed up (%)
Byte[4] = Reheating T desired (°C)

*  Value sent is (temperature + 20) * 2
** Sent value is 1875000 / (RPM)

Response: ACK
*/
#define CMD_SET_EWT_POSTHEATING 0xED
#define RES_SET_EWT_POSTHEATING COMMAND_ACK

// ================================================
// ========== Test mode (PC to ComfoAir) ==========
// ================================================

/*
Data: -

Response: -
Confirmation test mode
*/
#define CMD_SET_TEST_MODE 0x01
#define RES_SET_TEST_MODE 0x02

/*
Data: 2 bytes
Byte[0] = relay
    0x01 = Preheating relay
    0x02 = Triac preheating
    0x04 = EWT supply
    0x08 = EWT direction
    0x10 = kitchen hood
    0x20 = error
Byte[1] = Return message
    0x01 = Filter full LED

Response: -
*/
#define CMD_SET_OUTPUTS 0x05
#define RES_SET_OUTPUTS []

/*
Data: 3 bytes
Byte[0] = Supply air (%)
Byte[1] = Exhaust air (%)
Byte[2] = Post-heating (%)

Response: -
*/
#define CMD_SET_ANALOG_OUTPUTS 0x07
#define RES_SET_ANALOG_OUTPUTS []

/*
Data: 2 bytes
Byte[0] = Bypass (1 = open / 0 = closed / 3 = stop)
Byte[1] = Preheating (1 = open / 0 = closed / 3 = stop)

Response:
*/
#define CMD_SET_FLAPS 0x09
#define RES_SET_FLAPS []

/*
Data: -

Response: -
Confirmation end of test mode
*/
#define CMD_EXIT_TEST_MODE 0x19
#define RES_EXIT_TEST_MODE 0x1A

// ========================================================
// ========== Command list (CC-Ease to ComfoAir) ==========
// ========================================================

/*
Data: -

Response: -

See ComfoAir command list on CC-Ease
*/
#define CMD_GET_DATA 0x33
#define RES_GET_DATA_1 0x38
#define RES_GET_DATA_2 0x3E
#define RES_GET_DATA_3 0x40
#define RES_GET_DATA_4 0x98
#define RES_GET_DATA_5 0x9C
#define RES_GET_DATA_6 0xAA
#define RES_GET_DATA_7 0xCE
#define RES_GET_DATA_8 0xD2
#define RES_GET_DATA_9 0xE0
#define RES_GET_DATA_10 0xE2
#define RES_GET_DATA_11 0xEC

/*
Data: 5 bytes
Byte[0] = RTC day
    0x00 = Saturday
    0x01 = Sunday
    0x02 = Monday
    0x03 = Tuesday
    0x04 = Wednesday
    0x05 = Thursday
    0x06 = Friday
Byte[1] = RTC hour (0..23)
Byte[2] = RTC minutes (0..59)
Byte[3] = Temperature (°C*)
Byte[4] = CC Ease version
    Bit 7..4 = Version Major
    Bit 3..0 = Version Minor

* Value sent is (temperature + 20) * 2

Response: -
See ComfoAir command list on CC-Ease

*/
#define CMD_GET_CC_EASE_PARAMETERS 0x35
#define RES_GET_CC_EASE_PARAMETERS 0x3C

/*
Data: 7 bytes
Byte[0] Fan (0..255 = 0..4080 milliseconds)
Byte[1] Mode (0..255 = 0..4080 milliseconds)
Byte[2] Clock (0..255 = 0..4080 milliseconds)
Byte[3] Temperature (0..255 = 0..4080 milliseconds)
Byte[4] Plus (0..255 = 0..4080 milliseconds)
Byte[5] Minus (0..255 = 0..4080 milliseconds)
Byte[6] Status bits
    0x01 = Flashing on/off

Response: -
See ComfoAir command list on CC-Ease
*/
#define CMD_CCEASE_BUTTON_STATUS 0x37
#define RES_CCEASE_BUTTON_STATUS 0x3C

/*
Data: 16 bytes
Byte[0] = Module type receiver
Byte[1] = Module ID receiver
Byte[2] = Module type transmitter
Byte[3] = Module ID transmitter
Byte[4] = Lifetime
Byte[5] = Data type
Byte[6] = Data byte 1
Byte[7] = Data byte 2
Byte[8] = Data byte 3
Byte[9] = Data byte 4
Byte[10] = Data byte 5
Byte[11] = Data byte 6
Byte[12] = Data byte 7
Byte[13] = Data byte 8
Byte[14] = Data byte 9
Byte[15] = Data byte 10

Response: -
See ComfoAir command list on CC-Ease
*/
#define CMD_GET_RF_COMMAND 0x39
#define RES_GET_RF_COMMAND 0x40

// ========================================================
// ========== Command list (ComfoAir to CC-Ease) ==========
// ========================================================

/*
Data: 5 bytes
Byte[0] = RTC day
    0x00 = Saturday
    0x01 = Sunday
    0x02 = Monday
    0x03 = Tuesday
    0x04 = Wednesday
    0x05 = Thursday
    0x06 = Friday
Byte[1] = RTC hour (0..23)
Byte[2] = RTC minutes (0..59)
Byte[3] = Backlight timeout (Fixed at 30)
Byte[4] = Backlight (fixed at 100%)

Response:
*/
#define CMD_SET_PARAMETER 0x38
#define RES_SET_PARAMETER []

/*
Data: 10 bytes
Byte[0] = (1 = on / 0 = off)
    0x01 = Saturday
    0x02 = Sunday
    0x04 = Monday
    0x08 = Tuesday
    0x10 = Wednesday
    0x20 = Thursday
    0x40 = Friday
    0x80 = colon
Byte[1] = (1 = on / 0 = off)
    0x01 = 1AEGED
    0x02 = 1B
    0x04 = 1C
    0x08 = AUTO symbol
    0x10 = MANUAL symbol
    0x20 = FILTER icon
    0x40 = Symbol I
    0x80 = Symbol E
Byte[2] = (1 = on / 0 = off)
    0x01 = 2A
    0x02 = 2B
    0x04 = 2C
    0x08 = 2D
    0x10 = 2E
    0x20 = 2F
    0x40 = 2G
    0x80 = FAN symbol
Byte[3] = (1 = on / 0 = off)
    0x01 = 3A
    0x02 = 3B
    0x04 = 3C
    0x08 = 3DByte[1] =
    0x10 = 3E
    0x20 = 3F
    0x40 = 3G
    0x80 = kitchen hood symbol
Byte[4] = (1 = on / 0 = off)
    0x01 = 4A
    0x02 = 4B
    0x04 = 4C
    0x08 = 4D
    0x10 = 4E
    0x20 = 4F
    0x40 = 4G
    0x80 = Preheating symbol
Byte[5] = (1 = on / 0 = off)
    0x01 = 5A
    0x02 = 5B
    0x04 = 5C
    0x08 = 5D
    0x10 = 5E
    0x20 = 5F
    0x40 = 5G
    0x80 = Frost symbol
Byte[6] = (1 = on / 0 = off)
    0x01 = 6A
    0x02 = 6B
    0x04 = 6C
    0x08 = 6D
    0x10 = 6E
    0x20 = 6F
    0x40 = 6G
    0x80 = Symbol EWT
Byte[7] = (1 = on / 0 = off)
    0x01 = 7A
    0x02 = 7B
    0x04 = 7C
    0x08 = 7D
    0x10 = 7E
    0x20 = 7F
    0x40 = 7G
    0x80 = Reheating symbol
Byte[8] = (1 = on / 0 = off)
    0x01 = 8A
    0x02 = 8B
    0x04 = 8C
    0x08 = 8D
    0x10 = 8E
    0x20 = 8F
    0x40 = 8G
    0x80 = point
Byte[9] = (1 = on / 0 = off)
    0x01 = Degree symbol
    0x02 = Bypass symbol
    0x04 = X1
    0x08 = X2
    0x10 = X3
    0x20 = House symbol
    0x40 = supply air symbol
    0x80 = Exhaust air symbol

Response: -
*/
#define CMD_SET_DISPLAY 0x39
#define RES_SET_DISPLAY []

/*
Data: 4 bytes
Byte[0] = RF Address 4 (Low Byte)
Byte[1] = RF Address 3
Byte[2] = RF Address 2
Byte[3] = RF Address 1 (High Byte)

Response: -
*/
#define CMD_SET_RF_ADDRESS 0x3E
#define RES_SET_RF_ADDRESS []

/*
Data: 21 bytes
Byte[0] = Module type receiver
Byte[1] = Module ID receiver
Byte[2] = Module type transmitter
Byte[3] = Module ID transmitter
Byte[4] = Lifetime
Byte[5] = Date type
Byte[6] = Data byte 1
Byte[7] = Data byte 2
Byte[8] = Data byte 3
Byte[9] = Data byte 4
Byte[10] = Data byte 5
Byte[11] = Data byte 6
Byte[12] = Data byte 7
Byte[13] = Data byte 8
Byte[14] = Data byte 9
Byte[15] = Data byte 10
Byte[16] = RF address 4 (low byte)
Byte[17] = RF address 3
Byte[18] = RF address 2
Byte[19] = RF address 1 (high byte)
Byte[20] = Control bits
    0x01 = Repeat previous package first
    0x02 = 250ms pause before sending
    0x04 = Received at sender address

Response:
*/
#define CMD_SEND_RF_COMMAND 0x40
#define RES_SEND_RF_COMMAND []
