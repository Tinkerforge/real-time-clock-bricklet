/* real-time-clock
 * Copyright (C) 2016 Matthias Bolte <matthias@tinkerforge.com>
 *
 * real-time-clock.c: Implementation of Real Time Clock Bricklet messages
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

#include "real-time-clock.h"

#include "brickletlib/bricklet_entry.h"
#include "bricklib/bricklet/bricklet_communication.h"
#include "config.h"
#include "bricklib/utility/util_definitions.h"

#define I2C_ADDRESS 0x51 // 0b1010001, I2C address of PCF85263A
#define I2C_HALF_CLOCK_400KHZ 1250 // 2500ns per clock
#define I2C_READ 1
#define I2C_WRITE 0

void invocation(const ComType com, const uint8_t *data) {
	switch(((MessageHeader*)data)->fid) {
		case FID_SET_DATE_TIME:
			set_date_time(com, (SetDateTime*)data);
			return;

		case FID_GET_DATE_TIME:
			get_date_time(com, (GetDateTime*)data);
			return;

		case FID_GET_TIMESTAMP:
			get_timestamp(com, (GetTimestamp*)data);
			return;

		case FID_SET_OFFSET:
			set_offset(com, (SetOffset*)data);
			return;

		case FID_GET_OFFSET:
			get_offset(com, (GetOffset*)data);
			return;

		default:
			BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
			return;
	}
}

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");

	// Setting the interrupt pin as input
	PIN_INT.pio->PIO_PUER = PIN_INT.mask;
	PIN_INT.pio->PIO_ODR = PIN_INT.mask;
	PIN_INT.pio->PIO_PER = PIN_INT.mask;

	// Load calibration from EEPROM
	BA->bricklet_select(BS->port - 'a');

	BA->i2c_eeprom_master_read(BA->twid->pTwi,
	                           CALIBRATION_EEPROM_POSITION,
	                           (char *)BC->calibration,
	                           sizeof(BC->calibration));

	if (BC->calibration[0] == CALIBRATION_EEPROM_MAGIC0 &&
	    BC->calibration[1] == CALIBRATION_EEPROM_MAGIC0) {
		write_register(REG_OFFSET, BC->calibration[3]);
	} else {
		// EEPROM doesn't contain calibration, store current calibration
		BC->calibration[0] = CALIBRATION_EEPROM_MAGIC0;
		BC->calibration[1] = CALIBRATION_EEPROM_MAGIC1;
		BC->calibration[2] = read_register(REG_OFFSET);

		BA->i2c_eeprom_master_write(BA->twid->pTwi,
		                            CALIBRATION_EEPROM_POSITION,
		                            (const char*)BC->calibration,
		                            sizeof(BC->calibration));
	}

	BA->bricklet_deselect(BS->port - 'a');

	// Select offset calibration mode, 12.5pF oscillator load capacitance
	uint8_t oscillator = REG_OSCILLATOR_CL_12PF;

	if (BC->calibration[2] == CALIBRATION_MODE_LOW_POWER) {
		oscillator |= REG_OSCILLATOR_OFFM_LOW_POWER;
	} else {
		oscillator |= REG_OSCILLATOR_OFFM_FAST_CONNECTION;
	}

	write_register(REG_OSCILLATOR, oscillator);

	// Enable 100th second and set CLK pin frequency to 0Hz
	write_register(REG_FUNCTION, REG_FUNCTION_100TH_ENABLED | REG_FUNCTION_COF_STATIC_LOW);

	// Disable CLK pin
	write_register(REG_PIN_IO, REG_PIN_IO_CLKPM_DISABLE);
}

void destructor(void) {
}

void tick(const uint8_t tick_type) {
}

void set_date_time(const ComType com, const SetDateTime *data) {
	if (data->year < 2000 || data->year > 2099 ||
	    data->month < 1 || data->month > 12 ||
	    data->day < 1 || data->day > 31 ||
	    data->hour > 23 ||
	    data->minute > 59 ||
	    data->second > 59 ||
	    data->centisecond > 99 ||
	    data->weekday < 1 || data->weekday > 7) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	const uint8_t bytes[8] = {
		bin2bcd(data->centisecond),
		bin2bcd(data->second),
		bin2bcd(data->minute),
		bin2bcd(data->hour),
		bin2bcd(data->day),
		bin2bcd(data->weekday % 7), // Convert from [Mon..Sun] == [1..7] to  [Sun..Sat] == [0..6]
		bin2bcd(data->month),
		bin2bcd(data->year - 2000)
	};

	// Stop clock and clear prescaler
	write_register(REG_STOP_ENABLE, REG_STOP_ENABLE_STOP_ENABLED);
	write_register(REG_RESET, REG_RESET_CLEAR_PRESCALER);

	// Set clock
	write_registers(REG_RTC_TIME_100TH_SECOND, bytes, 8);

	// Start clock
	write_register(REG_STOP_ENABLE, REG_STOP_ENABLE_STOP_DISABLED);

	BA->com_return_setter(com, data);
}

void get_date_time(const ComType com, const GetDateTime *data) {
	GetDateTimeReturn gdtr;
	uint8_t bytes[8];

	gdtr.header        = data->header;
	gdtr.header.length = sizeof(gdtr);

	read_registers(REG_RTC_TIME_100TH_SECOND, bytes, 8);

	gdtr.year        = 2000 + bcd2bin(bytes[7]);
	gdtr.month       = bcd2bin(bytes[6]);
	gdtr.day         = bcd2bin(bytes[4]);
	gdtr.hour        = bcd2bin(bytes[3]);
	gdtr.minute      = bcd2bin(bytes[2] & 0b01111111); // remove EMON bit
	gdtr.second      = bcd2bin(bytes[1] & 0b01111111); // remove OS bit
	gdtr.centisecond = bcd2bin(bytes[0]);
	gdtr.weekday     = bcd2bin(bytes[5]);

	// Convert from [Sun..Sat] == [0..6] to [Mon..Sun] == [1..7]
	if (gdtr.weekday == 0) {
		gdtr.weekday = 7;
	}

	BA->send_blocking_with_timeout(&gdtr, sizeof(gdtr), com);
}

static const uint16_t days_before_this_month_table[12] = {
	0,
	31,
	31 + 28,
	31 + 28 + 31,
	31 + 28 + 31 + 30,
	31 + 28 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30
};

void get_timestamp(const ComType com, const GetTimestamp *data) {
	GetTimestampReturn gtr;
	uint8_t bytes[8];

	gtr.header        = data->header;
	gtr.header.length = sizeof(gtr);

	read_registers(REG_RTC_TIME_100TH_SECOND, bytes, 8);

	uint8_t year        = bcd2bin(bytes[7]); // [0..99]
	uint8_t month       = bcd2bin(bytes[6]); // [1..12]
	uint8_t day         = bcd2bin(bytes[4]); // [1..31]
	uint8_t hour        = bcd2bin(bytes[3]); // [0..23]
	uint8_t minute      = bcd2bin(bytes[2] & 0b01111111); // [0..59], remove EMON bit
	uint8_t second      = bcd2bin(bytes[1] & 0b01111111); // [0..59], remove OS bit
	uint8_t centisecond = bcd2bin(bytes[0]); // [0..99]

	int64_t days_before_this_year  = year * 365 + (MAX((int8_t)year - 1, 0)) / 4 + (year > 0 ? 1 : 0);
	int64_t days_before_this_month = days_before_this_month_table[month - 1] + (month > 2 && year % 4 == 0 ? 1 : 0);

	gtr.milliseconds = (((((days_before_this_year + days_before_this_month + day - 1) * 24 + hour) * 60 + minute) * 60) + second) * 1000 + centisecond * 10;

	BA->send_blocking_with_timeout(&gtr, sizeof(gtr), com);
}

void set_offset(const ComType com, const SetOffset *data) {
	BC->calibration[2] = data->offset;

	write_register(REG_OFFSET, BC->calibration[2]);

	BA->bricklet_select(BS->port - 'a');
	BA->i2c_eeprom_master_write(BA->twid->pTwi,
	                            CALIBRATION_EEPROM_POSITION,
	                            (const char*)BC->calibration,
	                            sizeof(BC->calibration));
	BA->bricklet_deselect(BS->port - 'a');

	BA->com_return_setter(com, data);
}

void get_offset(const ComType com, const GetOffset *data) {
	GetOffsetReturn gor;

	gor.header        = data->header;
	gor.header.length = sizeof(gor);
	gor.offset        = BC->calibration[2];

	BA->send_blocking_with_timeout(&gor, sizeof(gor), com);
}

uint8_t read_register(const uint8_t reg) {
	i2c_start();
	i2c_send_byte((I2C_ADDRESS << 1) | I2C_WRITE);
	i2c_send_byte(reg);
	i2c_stop();
	i2c_start();
	i2c_send_byte((I2C_ADDRESS << 1) | I2C_READ);

	uint8_t value = i2c_recv_byte(false);

	i2c_stop();

	return value;
}

void read_registers(const uint8_t reg, uint8_t *data, const uint8_t length) {
	i2c_start();
	i2c_send_byte((I2C_ADDRESS << 1) | I2C_WRITE);
	i2c_send_byte(reg);
	i2c_stop();
	i2c_start();
	i2c_send_byte((I2C_ADDRESS << 1) | I2C_READ);

	for(uint8_t i = 0; i < length; i++) {
		data[i] = i2c_recv_byte(i != (length - 1));
	}

	i2c_stop();
}

void write_register(const uint8_t reg, const uint8_t value) {
	i2c_start();
	i2c_send_byte((I2C_ADDRESS << 1) | I2C_WRITE);
	i2c_send_byte(reg);
	i2c_send_byte(value);
	i2c_stop();
}

void write_registers(const uint8_t reg, const uint8_t *data, const uint8_t length) {
	i2c_start();
	i2c_send_byte((I2C_ADDRESS << 1) | I2C_WRITE);
	i2c_send_byte(reg);

	for(uint8_t i = 0; i < length; i++) {
		i2c_send_byte(data[i]);
	}

	i2c_stop();
}

void i2c_foobar(bool high) {
	i2c_scl_low();

	if (high) {
		i2c_sda_high();
	} else {
		i2c_sda_low();
	}

	i2c_sleep_halfclock();
	i2c_scl_high();
	i2c_sleep_halfclock();
}

bool i2c_scl_value(void) {
	return PIN_SCL.pio->PIO_PDSR & PIN_SCL.mask;
}

void i2c_scl_high(void) {
	PIN_SCL.pio->PIO_ODR = PIN_SCL.mask;
	while(!i2c_scl_value()); // allow slave to clock-stretch
}

void i2c_scl_low(void) {
	PIN_SCL.pio->PIO_OER = PIN_SCL.mask;
}

bool i2c_sda_value(void) {
	return PIN_SDA.pio->PIO_PDSR & PIN_SDA.mask;
}

void i2c_sda_high(void) {
	PIN_SDA.pio->PIO_ODR = PIN_SDA.mask;
}

void i2c_sda_low(void) {
	PIN_SDA.pio->PIO_OER = PIN_SDA.mask;
}

void i2c_sleep_halfclock(void) {
	SLEEP_NS(I2C_HALF_CLOCK_400KHZ);
}

void i2c_stop(void) {
	i2c_scl_low();
	i2c_sda_low();
	i2c_sleep_halfclock();
	i2c_scl_high();
	i2c_sleep_halfclock();
	i2c_sda_high();
	i2c_sleep_halfclock();
}

void i2c_start(void) {
	i2c_scl_high();
	i2c_sleep_halfclock();
	i2c_sda_low();
	i2c_sleep_halfclock();
}

uint8_t i2c_recv_byte(bool ack) {
	uint8_t value = 0;

	for(int8_t i = 7; i >= 0; i--) {
		i2c_scl_low();
		i2c_sda_high(); // allow slave to read
		i2c_sleep_halfclock();
		i2c_scl_high();

		if(i2c_sda_value()) {
			value |= (1 << i);
		}

		i2c_sleep_halfclock();
	}

	// ACK
#if 0
	i2c_scl_low();

	if(ack) {
		i2c_sda_low();
	} else {
		i2c_sda_high();
	}

	i2c_sleep_halfclock();
	i2c_scl_high();
	i2c_sleep_halfclock();
#else
	i2c_foobar(!ack);
#endif

	return value;
}

bool i2c_send_byte(const uint8_t value) {
	for(int8_t i = 7; i >= 0; i--) {
#if 0
		i2c_scl_low();

		if((value >> i) & 1) {
			i2c_sda_high();
		} else {
			i2c_sda_low();
		}

		i2c_sleep_halfclock();
		i2c_scl_high();
		i2c_sleep_halfclock();
#else
		i2c_foobar((value >> i) & 1);
#endif
	}

	// Wait for ACK
	bool ret = false;

	i2c_scl_low();
	i2c_sleep_halfclock();
	i2c_scl_high();

	if(!i2c_sda_value()) {
		ret = true;
	}

	i2c_sleep_halfclock();

	return ret;
}

uint8_t bcd2bin(const uint8_t bcd) {
	return (bcd >> 4) * 10 + (bcd & 0b00001111);
}

uint8_t bin2bcd(const uint8_t bin) {
	return ((bin / 10) << 4) + (bin % 10);
}
