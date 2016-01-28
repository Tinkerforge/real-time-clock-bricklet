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
#define CALIBRATION_EEPROM_MAGIC0   0b00110011
#define CALIBRATION_EEPROM_MAGIC1   0b10101010

#define FID_SET_DATE_TIME 1
#define FID_GET_DATE_TIME 2
#define FID_GET_TIMESTAMP 3
#define FID_SET_OFFSET 4
#define FID_GET_OFFSET 5

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) StandardMessage;

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
} __attribute__((__packed__)) SetDateTime;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetDateTime;

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
} __attribute__((__packed__)) GetDateTimeReturn;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetTimestamp;

typedef struct {
	MessageHeader header;
	int64_t timestamp;
} __attribute__((__packed__)) GetTimestampReturn;

typedef struct {
	MessageHeader header;
	int8_t offset;
} __attribute__((__packed__)) SetOffset;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetOffset;

typedef struct {
	MessageHeader header;
	int8_t offset;
} __attribute__((__packed__)) GetOffsetReturn;

void invocation(const ComType com, const uint8_t *data);
void constructor(void);
void destructor(void);
void tick(const uint8_t tick_type);

void set_date_time(const ComType com, const SetDateTime *data);
void get_date_time(const ComType com, const GetDateTime *data);

void get_timestamp(const ComType com, const GetTimestamp *data);

void set_offset(const ComType com, const SetOffset *data);
void get_offset(const ComType com, const GetOffset *data);

uint8_t read_register(const uint8_t reg);
void read_registers(const uint8_t reg, uint8_t *data, const uint8_t length);
void write_register(const uint8_t reg, const uint8_t value);
void write_registers(const uint8_t reg, const uint8_t *data, const uint8_t length);
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

#endif
