/* real-time-clock
 * Copyright (C) 2016 Matthias Bolte <matthias@tinkerforge.com>
 *
 * real-time-clock.h: Implementation of Real Time Clock Bricklet messages
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

#ifndef REAL_TIME_CLOCK_H
#define REAL_TIME_CLOCK_H

#include <stdint.h>
#include "bricklib/com/com_common.h"

#define CALIBRATION_EEPROM_POSITION (BRICKLET_PLUGIN_MAX_SIZE + 96)
#define CALIBRATION_EEPROM_MAGIC0   0b10101010
#define CALIBRATION_EEPROM_MAGIC1   0b00110011

#define FID_SET_MODE 1
#define FID_GET_MODE 2
#define FID_SET_RTC_DATE_TIME 3
#define FID_GET_RTC_DATE_TIME 4
#define FID_SET_RTC_ALARM 5
#define FID_GET_RTC_ALARM 6
#define FID_SET_STOPWATCH_TIME 7
#define FID_GET_STOPWATCH_TIME 8
#define FID_SET_STOPWATCH_ALARM 9
#define FID_GET_STOPWATCH_ALARM 10
#define FID_SET_COUNTDOWN 11
#define FID_GET_COUNTDOWN 12
#define FID_SET_USER_DATA 13
#define FID_GET_USER_DATA 14
#define FID_SET_CALIBRATION 15
#define FID_GET_CALIBRATION 16
#define FID_RTC_ALARM 17
#define FID_STOPWATCH_ALARM 18
#define FID_COUNTDOWN 19

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) StandardMessage;

typedef struct {
	MessageHeader header;
	uint8_t mode;
} __attribute__((__packed__)) SetMode;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetMode;

typedef struct {
	MessageHeader header;
	uint8_t mode;
} __attribute__((__packed__)) GetModeReturn;

typedef struct {
	MessageHeader header;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t centisecond;
	uint8_t weekday;
} __attribute__((__packed__)) SetRTCDateTime;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetRTCDateTime;

typedef struct {
	MessageHeader header;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t centisecond;
	uint8_t weekday;
} __attribute__((__packed__)) GetRTCDateTimeReturn;

typedef struct {
	MessageHeader header;
	uint8_t alarm;
	int8_t month;
	int8_t day;
	int8_t hour;
	int8_t minute;
	int8_t second;
	int8_t weekday;
} __attribute__((__packed__)) SetRTCAlarm;

typedef struct {
	MessageHeader header;
	uint8_t alarm;
} __attribute__((__packed__)) GetRTCAlarm;

typedef struct {
	MessageHeader header;
	int8_t month;
	int8_t day;
	int8_t hour;
	int8_t minute;
	int8_t second;
	int8_t weekday;
} __attribute__((__packed__)) GetRTCAlarmReturn;

typedef struct {
	MessageHeader header;
	uint32_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t centisecond;
} __attribute__((__packed__)) SetStopwatchTime;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetStopwatchTime;

typedef struct {
	MessageHeader header;
	uint32_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t centisecond;
} __attribute__((__packed__)) GetStopwatchTimeReturn;

typedef struct {
	MessageHeader header;
	uint8_t alarm;
	int32_t hour;
	int8_t minute;
	int8_t second;
} __attribute__((__packed__)) SetStopwatchAlarm;

typedef struct {
	MessageHeader header;
	uint8_t alarm;
} __attribute__((__packed__)) GetStopwatchAlarm;

typedef struct {
	MessageHeader header;
	int32_t hour;
	int8_t minute;
	int8_t second;
} __attribute__((__packed__)) GetStopwatchAlarmReturn;

typedef struct {
	MessageHeader header;
	uint8_t mode;
	uint8_t counter;
	uint8_t step;
} __attribute__((__packed__)) SetCountdown;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetCountdown;

typedef struct {
	MessageHeader header;
	uint8_t mode;
	uint8_t counter;
	uint8_t step;
} __attribute__((__packed__)) GetCountdownReturn;

typedef struct {
	MessageHeader header;
	uint8_t data;
} __attribute__((__packed__)) SetUserData;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetUserData;

typedef struct {
	MessageHeader header;
	uint8_t data;
} __attribute__((__packed__)) GetUserDataReturn;

typedef struct {
	MessageHeader header;
	uint8_t mode;
	int8_t offset;
} __attribute__((__packed__)) SetCalibration;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetCalibration;

typedef struct {
	MessageHeader header;
	uint8_t mode;
	int8_t offset;
} __attribute__((__packed__)) GetCalibrationReturn;

typedef struct {
	MessageHeader header;
	uint8_t alarm;
} __attribute__((__packed__)) RTCAlarm;

typedef struct {
	MessageHeader header;
	uint8_t alarm;
} __attribute__((__packed__)) StopwatchAlarm;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) Countdown;

void invocation(const ComType com, const uint8_t *data);
void constructor(void);
void destructor(void);
void tick(const uint8_t tick_type);

void set_raw_clock(uint8_t *bytes, const uint8_t length);
void get_raw_clock(uint8_t *bytes, const uint8_t length);

void set_raw_alarm(const uint8_t reg, uint8_t *bytes, const uint8_t length,
                   const uint8_t enable_mask, const uint8_t enable_offset);
void get_raw_alarm(const uint8_t reg, uint8_t *bytes, const uint8_t length,
                   const uint8_t enable_offset);

void set_mode(const ComType com, const SetMode *data);
void get_mode(const ComType com, const GetMode *data);

void set_rtc_date_time(const ComType com, const SetRTCDateTime *data);
void get_rtc_date_time(const ComType com, const GetRTCDateTime *data);

void set_rtc_alarm(const ComType com, const SetRTCAlarm *data);
void get_rtc_alarm(const ComType com, const GetRTCAlarm *data);

void set_stopwatch_time(const ComType com, const SetStopwatchTime *data);
void get_stopwatch_time(const ComType com, const GetStopwatchTime *data);

void set_stopwatch_alarm(const ComType com, const SetStopwatchAlarm *data);
void get_stopwatch_alarm(const ComType com, const GetStopwatchAlarm *data);

void set_countdown(const ComType com, const SetCountdown *data);
void get_countdown(const ComType com, const GetCountdown *data);

void set_user_data(const ComType com, const SetUserData *data);
void get_user_data(const ComType com, const GetUserData *data);

void set_calibration(const ComType com, const SetCalibration *data);
void get_calibration(const ComType com, const GetCalibration *data);

uint8_t read_register(const uint8_t reg);
void read_registers(const uint8_t reg, uint8_t *data, const uint8_t length);
void write_register(const uint8_t reg, uint8_t value);
void write_registers(const uint8_t reg, uint8_t *data, const uint8_t length);
void i2c_foobar(bool high);
bool i2c_scl_value(void);
void i2c_scl_high(void);
void i2c_scl_low(void);
bool i2c_sda_value(void);
void i2c_sda_high(void);
void i2c_sda_low(void);
void i2c_sleep_halfclock(void);
void i2c_stop(void);
void i2c_start(void);
uint8_t i2c_recv_byte(bool ack);
bool i2c_send_byte(const uint8_t value);

uint8_t bcd2bin(const uint8_t bcd);
uint8_t bin2bcd(const uint8_t bin);
/*
#define bcd2bin(bcd) (((bcd) >> 4) * 10 + ((bcd) & 0b00001111))
#define bin2bcd(bin) ((((bin) / 10) << 4) + ((bin) % 10))
*/
#endif
