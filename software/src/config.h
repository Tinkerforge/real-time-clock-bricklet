/* real-time-clock
 * Copyright (C) 2016 Matthias Bolte <matthias@tinkerforge.com>
 *
 * config.h: Real Time Clock Bricklet specific configuration
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib/drivers/board/sam3s/SAM3S.h"

#include "real-time-clock.h"

#define BRICKLET_FIRMWARE_VERSION_MAJOR 2
#define BRICKLET_FIRMWARE_VERSION_MINOR 0
#define BRICKLET_FIRMWARE_VERSION_REVISION 0
#define BRICKLET_HARDWARE_VERSION_MAJOR 1
#define BRICKLET_HARDWARE_VERSION_MINOR 0
#define BRICKLET_HARDWARE_VERSION_REVISION 0

#define BRICKLET_DEVICE_IDENTIFIER 268

#define INVOCATION_IN_BRICKLET_CODE

#define PIN_SDA (BS->pin1_ad)
#define PIN_SCL (BS->pin2_da)
#define PIN_INT (BS->pin3_pwm)

//#define LOGGING_LEVEL LOGGING_DEBUG
//#define DEBUG_BRICKLET 1
#define BOARD_MCK 64000000

#define CALIBRATION_MODE_LOW_POWER                0
#define CALIBRATION_MODE_FAST_CORRECTION          1

// RTC Time and Date
#define REG_RTC_TIME_100TH_SECOND                 0x00
#define REG_RTC_TIME_OS_AND_SECOND                0x01
#define REG_RTC_TIME_MINUTE                       0x02
#define REG_RTC_TIME_HOUR                         0x03
#define REG_RTC_DATE_DAY                          0x04
#define REG_RTC_DATE_WEAKDAY                      0x05
#define REG_RTC_DATE_MONTH                        0x06
#define REG_RTC_DATE_YEAR                         0x07

// RTC Alarm 1 and 2
#define REG_RTC_ALARM1_SECOND                     0x08
#define REG_RTC_ALARM1_MINUTE                     0x09
#define REG_RTC_ALARM1_HOUR                       0x0A
#define REG_RTC_ALARM1_DAY                        0x0B
#define REG_RTC_ALARM1_MONTH                      0x0C

#define REG_RTC_ALARM2_MINUTE                     0x0D
#define REG_RTC_ALARM2_HOUR                       0x0E
#define REG_RTC_ALARM2_WEEKDAY                    0x0F

#define REG_RTC_ALARM_ENABLE                      0x10

// RTC Timestamp 1, 2, and 3
#define REG_RTC_TIMESTAMP1_SECOND                 0x11
#define REG_RTC_TIMESTAMP1_MINUTE                 0x12
#define REG_RTC_TIMESTAMP1_HOUR                   0x13
#define REG_RTC_TIMESTAMP1_DAY                    0x14
#define REG_RTC_TIMESTAMP1_MONTH                  0x15
#define REG_RTC_TIMESTAMP1_YEAR                   0x16

#define REG_RTC_TIMESTAMP2_SECOND                 0x17
#define REG_RTC_TIMESTAMP2_MINUTE                 0x18
#define REG_RTC_TIMESTAMP2_HOUR                   0x19
#define REG_RTC_TIMESTAMP2_DAY                    0x1A
#define REG_RTC_TIMESTAMP2_MONTH                  0x1B
#define REG_RTC_TIMESTAMP2_YEAR                   0x1C

#define REG_RTC_TIMESTAMP3_SECOND                 0x1D
#define REG_RTC_TIMESTAMP3_MINUTE                 0x1E
#define REG_RTC_TIMESTAMP3_HOUR                   0x1F
#define REG_RTC_TIMESTAMP3_DAY                    0x20
#define REG_RTC_TIMESTAMP3_MONTH                  0x21
#define REG_RTC_TIMESTAMP3_YEAR                   0x22

#define REG_RTC_TIMESTAMP_MODE                    0x23

// Stopwatch Time
#define REG_STOPWATCH_TIME_100TH_SECOND           0x00
#define REG_STOPWATCH_TIME_OS_AND_SECOND          0x01
#define REG_STOPWATCH_TIME_MINUTE                 0x02
#define REG_STOPWATCH_TIME_HOUR1                  0x03
#define REG_STOPWATCH_TIME_HOUR2                  0x04
#define REG_STOPWATCH_TIME_HOUR3                  0x05

// Stopwatch Alarm 1 and 2
#define REG_STOPWATCH_ALARM1_SECOND               0x08
#define REG_STOPWATCH_ALARM1_MINUTE               0x09
#define REG_STOPWATCH_ALARM1_HOUR1                0x0A
#define REG_STOPWATCH_ALARM1_HOUR2                0x0B
#define REG_STOPWATCH_ALARM1_HOUR3                0x0C

#define REG_STOPWATCH_ALARM2_MINUTE               0x0D
#define REG_STOPWATCH_ALARM2_HOUR1                0x0E
#define REG_STOPWATCH_ALARM2_HOUR2                0x0F

#define REG_STOPWATCH_ALARM_ENABLE                0x10

// Stopwatch Timestamp 1, 2, and 3
#define REG_STOPWATCH_TIMESTAMP1_SECOND           0x11
#define REG_STOPWATCH_TIMESTAMP1_MINUTE           0x12
#define REG_STOPWATCH_TIMESTAMP1_HOUR1            0x13
#define REG_STOPWATCH_TIMESTAMP1_HOUR2            0x14
#define REG_STOPWATCH_TIMESTAMP1_HOUR3            0x15

#define REG_STOPWATCH_TIMESTAMP2_SECOND           0x17
#define REG_STOPWATCH_TIMESTAMP2_MINUTE           0x18
#define REG_STOPWATCH_TIMESTAMP2_HOUR1            0x19
#define REG_STOPWATCH_TIMESTAMP2_HOUR2            0x1A
#define REG_STOPWATCH_TIMESTAMP2_HOUR3            0x1B

#define REG_STOPWATCH_TIMESTAMP3_SECOND           0x1D
#define REG_STOPWATCH_TIMESTAMP3_MINUTE           0x1E
#define REG_STOPWATCH_TIMESTAMP3_HOUR1            0x1F
#define REG_STOPWATCH_TIMESTAMP3_HOUR2            0x20
#define REG_STOPWATCH_TIMESTAMP3_HOUR3            0x21

#define REG_STOPWATCH_TIMESTAMP_MODE              0x23

// Controls and Functions
#define REG_OFFSET                                0x24
#define REG_OSCILLATOR                            0x25
#define REG_BATTERY_SWITCH                        0x26
#define REG_PIN_IO                                0x27
#define REG_FUNCTION                              0x28
#define REG_INTA_ENABLE                           0x29
#define REG_INTB_ENABLE                           0x2A
#define REG_FLAGS                                 0x2B
#define REG_RAM_BYTE                              0x2C
#define REG_WATCHDOG                              0x2D
#define REG_STOP_ENABLE                           0x2E
#define REG_RESET                                 0x2F

#define REG_RTC_ALARM_ENABLE_A1E_mask             0b00011111
#define REG_RTC_ALARM_ENABLE_A1E_SECOND           0b00000001
#define REG_RTC_ALARM_ENABLE_A1E_MINUTE           0b00000010
#define REG_RTC_ALARM_ENABLE_A1E_HOUR             0b00000100
#define REG_RTC_ALARM_ENABLE_A1E_DAY              0b00001000
#define REG_RTC_ALARM_ENABLE_A1E_MONTH            0b00010000

#define REG_RTC_ALARM_ENABLE_A2E_mask             0b11100000
#define REG_RTC_ALARM_ENABLE_A2E_MINUTE           0b00100000
#define REG_RTC_ALARM_ENABLE_A2E_HOUR             0b01000000
#define REG_RTC_ALARM_ENABLE_A2E_WEEKDAY          0b10000000

#define REG_STOPWATCH_ALARM_ENABLE_A1E_mask       0b00011111
#define REG_STOPWATCH_ALARM_ENABLE_A1E_SECOND     0b00000001
#define REG_STOPWATCH_ALARM_ENABLE_A1E_MINUTE     0b00000010
#define REG_STOPWATCH_ALARM_ENABLE_A1E_HOUR1      0b00000100
#define REG_STOPWATCH_ALARM_ENABLE_A1E_HOUR2      0b00001000
#define REG_STOPWATCH_ALARM_ENABLE_A1E_HOUR3      0b00010000

#define REG_STOPWATCH_ALARM_ENABLE_A2E_mask       0b11100000
#define REG_STOPWATCH_ALARM_ENABLE_A2E_MINUTE     0b00100000
#define REG_STOPWATCH_ALARM_ENABLE_A2E_HOUR1      0b01000000
#define REG_STOPWATCH_ALARM_ENABLE_A2E_HOUR2      0b10000000

#define REG_OSCILLATOR_OFFM_mask                  0b01000000
#define REG_OSCILLATOR_OFFM_LOW_POWER             0b00000000
#define REG_OSCILLATOR_OFFM_FAST_CONNECTION       0b01000000

#define REG_OSCILLATOR_1224_mask                  0b00100000
#define REG_OSCILLATOR_1224_24                    0b00000000
#define REG_OSCILLATOR_1224_12                    0b00100000

#define REG_OSCILLATOR_LOWJ_mask                  0b00010000
#define REG_OSCILLATOR_LOWJ_DISABLED              0b00000000
#define REG_OSCILLATOR_LOWJ_ENABLED               0b00010000

#define REG_OSCILLATOR_OSCD_mask                  0b00001100
#define REG_OSCILLATOR_OSCD_NORMAL                0b00000000
#define REG_OSCILLATOR_OSCD_LOW                   0b00000100
#define REG_OSCILLATOR_OSCD_HIGH                  0b00001000

#define REG_OSCILLATOR_CL_mask                    0b00000011
#define REG_OSCILLATOR_CL_7PF                     0b00000000
#define REG_OSCILLATOR_CL_6PF                     0b00000001
#define REG_OSCILLATOR_CL_12PF                    0b00000010

#define REG_PIN_IO_CLKPM_mask                     0b10000000
#define REG_PIN_IO_CLKPM_ENABLED                  0b00000000
#define REG_PIN_IO_CLKPM_DISABLE                  0b10000000

#define REG_PIN_IO_INTAPM_mask                    0b00000011
#define REG_PIN_IO_INTAPM_CLK                     0b00000000
#define REG_PIN_IO_INTAPM_BATTERY_MODE            0b00000001
#define REG_PIN_IO_INTAPM_INTA                    0b00000010
#define REG_PIN_IO_INTAPM_HI_Z                    0b00000011

#define REG_FUNCTION_100TH_mask                   0b10000000
#define REG_FUNCTION_100TH_DISABLED               0b00000000
#define REG_FUNCTION_100TH_ENABLED                0b10000000

#define REG_FUNCTION_RTCM_mask                    0b00010000
#define REG_FUNCTION_RTCM_RTC                     0b00000000
#define REG_FUNCTION_RTCM_STOPWATCH               0b00010000

#define REG_FUNCTION_COF_mask                     0b00000111
#define REG_FUNCTION_COF_32768HZ                  0b00000000
#define REG_FUNCTION_COF_16384HZ                  0b00000001
#define REG_FUNCTION_COF_8192HZ                   0b00000010
#define REG_FUNCTION_COF_4096HZ                   0b00000011
#define REG_FUNCTION_COF_2048HZ                   0b00000100
#define REG_FUNCTION_COF_1024HZ                   0b00000101
#define REG_FUNCTION_COF_1HZ                      0b00000110
#define REG_FUNCTION_COF_STATIC_LOW               0b00000111

#define REG_INTA_ENABLE_WATCHDOG                  0b00000001 // WDIEA
#define REG_INTA_ENABLE_BATTERY_SWITCH            0b00000010 // BSIEA
#define REG_INTA_ENABLE_TIMESTAMP                 0b00000100 // TSRIEA
#define REG_INTA_ENABLE_ALARM2                    0b00001000 // A2IEA
#define REG_INTA_ENABLE_ALARM1                    0b00010000 // A1IEA
#define REG_INTA_ENABLE_OFFSET_CORRECTION         0b00100000 // OIEA
#define REG_INTA_ENABLE_PERIODIC_INTERRUPT        0b01000000 // PIEA
#define REG_INTA_ENABLE_LEVEL_MODE                0b10000000 // ILPA

#define REG_FLAGS_TIMESTAMP1                      0b00000001 // TSR1F
#define REG_FLAGS_TIMESTAMP2                      0b00000010 // TSR2F
#define REG_FLAGS_TIMESTAMP3                      0b00000100 // TSR3F
#define REG_FLAGS_BATTERY_SWITCH                  0b00001000 // BSF
#define REG_FLAGS_WATCHDOG                        0b00010000 // WDF
#define REG_FLAGS_ALARM1                          0b00100000 // A1F
#define REG_FLAGS_ALARM2                          0b01000000 // A2F
#define REG_FLAGS_PERIODIC_INTERRUPT              0b10000000 // PIF

#define REG_WATCHDOG_MODE_mask                    0b10000000 // WDM
#define REG_WATCHDOG_MODE_offset                  7
#define REG_WATCHDOG_MODE_ONCE                    0b00000000
#define REG_WATCHDOG_MODE_REPEAT                  0b10000000

#define REG_WATCHDOG_COUNTER_mask                 0b01111100 // WDR
#define REG_WATCHDOG_COUNTER_offset               2

#define REG_WATCHDOG_STEP_mask                    0b00000011 // WDS
#define REG_WATCHDOG_STEP_offset                  0
#define REG_WATCHDOG_STEP_4000MS                  0b00000000
#define REG_WATCHDOG_STEP_1000MS                  0b00000001
#define REG_WATCHDOG_STEP_250MS                   0b00000010
#define REG_WATCHDOG_STEP_62MS                    0b00000011

#define REG_STOP_ENABLE_STOP_mask                 0b00000001
#define REG_STOP_ENABLE_STOP_DISABLED             0b00000000
#define REG_STOP_ENABLE_STOP_ENABLED              0b00000001

#define REG_RESET_SOFTWARE                        0b00101100 // SR
#define REG_RESET_CLEAR_PRESCALER                 0b10100100 // CPR
#define REG_RESET_CLEAR_TIMESTAMP                 0b00100101 // CTS

typedef struct {
	uint8_t calibration[3]; // magic0, magic1, offset

	uint32_t period_date_time;
	uint32_t period_date_time_counter;
	bool last_date_time_valid;
	uint64_t last_date_time_merged;

	uint32_t period_timestamp;
	uint32_t period_timestamp_counter;
	bool last_timestamp_valid;
	uint64_t last_timestamp_merged;
} BrickContext;

#endif
