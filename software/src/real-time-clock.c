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
	BrickletAPI *ba = BA;

	switch(((MessageHeader*)data)->fid) {
		case FID_SET_MODE:
			set_mode(com, (SetMode*)data);
			ba->com_return_setter(com, data);
			return;

		case FID_GET_MODE:
			get_mode(com, (GetMode*)data);
			return;

		case FID_SET_RTC_DATE_TIME:
			set_rtc_date_time(com, (SetRTCDateTime*)data);
			ba->com_return_setter(com, data);
			return;

		case FID_GET_RTC_DATE_TIME:
			get_rtc_date_time(com, (GetRTCDateTime*)data);
			return;

		case FID_SET_RTC_ALARM:
			set_rtc_alarm(com, (SetRTCAlarm*)data);
			ba->com_return_setter(com, data);
			return;

		case FID_GET_RTC_ALARM:
			get_rtc_alarm(com, (GetRTCAlarm*)data);
			return;

		case FID_SET_STOPWATCH_TIME:
			set_stopwatch_time(com, (SetStopwatchTime*)data);
			ba->com_return_setter(com, data);
			return;

		case FID_GET_STOPWATCH_TIME:
			get_stopwatch_time(com, (GetStopwatchTime*)data);
			return;

		case FID_SET_STOPWATCH_ALARM:
			set_stopwatch_alarm(com, (SetStopwatchAlarm*)data);
			ba->com_return_setter(com, data);
			return;

		case FID_GET_STOPWATCH_ALARM:
			get_stopwatch_alarm(com, (GetStopwatchAlarm*)data);
			return;

		case FID_SET_COUNTDOWN:
			set_countdown(com, (SetCountdown*)data);
			ba->com_return_setter(com, data);
			return;

		case FID_GET_COUNTDOWN:
			get_countdown(com, (GetCountdown*)data);
			return;

		case FID_SET_USER_DATA:
			set_user_data(com, (SetUserData*)data);
			ba->com_return_setter(com, data);
			return;

		case FID_GET_USER_DATA:
			get_user_data(com, (GetUserData*)data);
			return;

		case FID_SET_CALIBRATION:
			set_calibration(com, (SetCalibration*)data);
			ba->com_return_setter(com, data);
			return;

		case FID_GET_CALIBRATION:
			get_calibration(com, (GetCalibration*)data);
			return;

		default:
			ba->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
			return;
	}
}

void constructor(void) {
	BrickletAPI *ba = BA;
	BrickContext *bc = BC;
	const char port = BS->port - 'a';

	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");

	bc->flags = 0;
	bc->user_data = read_register(REG_RAM_BYTE);

	// Setting the interrupt pin as input
	PIN_INT.pio->PIO_PUER = PIN_INT.mask;
	PIN_INT.pio->PIO_ODR = PIN_INT.mask;
	PIN_INT.pio->PIO_PER = PIN_INT.mask;

	// Load calibration from EEPROM
	ba->bricklet_select(port);

	ba->i2c_eeprom_master_read(ba->twid->pTwi,
	                           CALIBRATION_EEPROM_POSITION,
	                           (char *)bc->calibration,
	                           sizeof(bc->calibration));

	if (bc->calibration[0] == CALIBRATION_EEPROM_MAGIC0 &&
	    bc->calibration[1] == CALIBRATION_EEPROM_MAGIC0) {
		write_register(REG_OFFSET, bc->calibration[3]);
	} else {
		// EEPROM doesn't contain calibration, store current calibration
		bc->calibration[0] = CALIBRATION_EEPROM_MAGIC0;
		bc->calibration[1] = CALIBRATION_EEPROM_MAGIC1;

		uint8_t offm = read_register(REG_OSCILLATOR) & REG_OSCILLATOR_OFFM_mask;

		if (offm == REG_OSCILLATOR_OFFM_LOW_POWER) {
			bc->calibration[2] = CALIBRATION_MODE_LOW_POWER;
		} else {
			bc->calibration[2] = CALIBRATION_MODE_FAST_CORRECTION;
		}

		bc->calibration[3] = read_register(REG_OFFSET);

		ba->i2c_eeprom_master_write(ba->twid->pTwi,
		                            CALIBRATION_EEPROM_POSITION,
		                            (const char*)bc->calibration,
		                            sizeof(bc->calibration));
	}

	ba->bricklet_deselect(port);

	// Select offset calibration mode and 12.5pF oscillator load capacitance
	uint8_t oscillator;

	if (bc->calibration[2] == CALIBRATION_MODE_LOW_POWER) {
		oscillator = REG_OSCILLATOR_OFFM_LOW_POWER;
	} else {
		oscillator = REG_OSCILLATOR_OFFM_FAST_CONNECTION;
	}

	write_register(REG_OSCILLATOR, oscillator | REG_OSCILLATOR_CL_12PF);

	// Read RTCM bit
	uint8_t function = read_register(REG_FUNCTION);

	if ((function & REG_FUNCTION_RTCM_mask) == REG_FUNCTION_RTCM_RTC) {
		bc->mode = MODE_RTC;
	} else {
		bc->mode = MODE_STOPWATCH;
	}

	// Enable centiseconds
	write_register(REG_FUNCTION, function | REG_FUNCTION_100TH_ENABLED);

	// Enable nINTA output pin
	write_register(REG_PIN_IO, REG_PIN_IO_INTAPM_INTA);

	// Enable nINTA interrupts
	write_register(REG_INTA_ENABLE, REG_INTA_ENABLE_WATCHDOG | REG_INTA_ENABLE_ALARM1 |
	                                REG_INTA_ENABLE_ALARM2 | REG_INTA_ENABLE_LEVEL_MODE);
}

void destructor(void) {
}

void tick(const uint8_t tick_type) {
	BrickletAPI *ba = BA;
	BrickContext *bc = BC;
	const uint32_t uid = BS->uid;

	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(!(PIN_INT.pio->PIO_PDSR & PIN_INT.mask)) { // nINTA
			bc->flags = read_register(REG_FLAGS);
			write_register(REG_FLAGS, 0);
		}
	}

	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		if (bc->flags & (REG_FLAGS_ALARM1 | REG_FLAGS_ALARM2)) {
			if (bc->mode == MODE_RTC) {
				RTCAlarm ra;

				ra.alarm = bc->flags & REG_FLAGS_ALARM1 ? 0 : 1;

				ba->com_make_default_header(&ra, uid, sizeof(ra), FID_RTC_ALARM);
				ba->send_blocking_with_timeout(&ra, sizeof(ra), *ba->com_current);
			} else {
				StopwatchAlarm swa;

				swa.alarm = bc->flags & REG_FLAGS_ALARM1 ? 0 : 1;

				ba->com_make_default_header(&swa, uid, sizeof(swa), FID_STOPWATCH_ALARM);
				ba->send_blocking_with_timeout(&swa, sizeof(swa), *ba->com_current);
			}
		}

		if (bc->flags & REG_FLAGS_WATCHDOG) {
			Countdown c;

			ba->com_make_default_header(&c, uid, sizeof(c), FID_COUNTDOWN);
			ba->send_blocking_with_timeout(&c, sizeof(c), *ba->com_current);
		}

		bc->flags = 0;
	}
}

void set_raw_clock(uint8_t *bytes, const uint8_t length) {
	for (uint8_t i = 0; i < length; ++i) {
		bytes[i] = bin2bcd(bytes[i]);
	}

	// Stop clock and clear prescaler
	write_register(REG_STOP_ENABLE, REG_STOP_ENABLE_STOP_ENABLED);
	write_register(REG_RESET, REG_RESET_CLEAR_PRESCALER);

	// Set clock
	write_registers(REG_RAW_CLOCK_100TH_SECOND, bytes, length);

	// Start clock
	write_register(REG_STOP_ENABLE, REG_STOP_ENABLE_STOP_DISABLED);
}

void get_raw_clock(uint8_t *bytes, const uint8_t length) {
	read_registers(REG_RAW_CLOCK_100TH_SECOND, bytes, length);

	bytes[1] &= 0b01111111; // remove OS bit
	bytes[2] &= 0b01111111; // remove EMON bit

	for (uint8_t i = 0; i < length; ++i) {
		bytes[i] = bcd2bin(bytes[i]);
	}
}

void set_raw_alarm(const uint8_t reg, uint8_t *bytes, const uint8_t length,
                   const uint8_t enable_mask, const uint8_t enable_offset) {
	// Clear alarm enable bits
	uint8_t enable = read_register(REG_RAW_ALARM_ENABLE);

	enable &= ~enable_mask;

	write_register(REG_RAW_ALARM_ENABLE, enable);

	// Set alarm
	for (uint8_t i = 0; i < length; ++i) {
		if ((int8_t)bytes[i] >= 0) {
			enable |= 1 << (enable_offset + i);
			bytes[i] = bin2bcd(bytes[i]);
		} else {
			bytes[i] = 0;
		}
	}

	write_registers(reg, bytes, length);

	// Set alarm enable bits
	write_register(REG_RAW_ALARM_ENABLE, enable);
}

void get_raw_alarm(const uint8_t reg, uint8_t *bytes, const uint8_t length,
                   const uint8_t enable_offset) {
	const uint8_t enable = read_register(REG_RAW_ALARM_ENABLE);

	read_registers(reg, bytes, length);

	for (uint8_t i = 0; i < length; ++i) {
		if (enable & (1 << (enable_offset + i))) {
			bytes[i] = bcd2bin(bytes[i]);
		} else {
			bytes[i] = -1;
		}
	}
}

void set_mode(const ComType com, const SetMode *data) {
	BrickletAPI *ba = BA;
	BrickContext *bc = BC;

	if(data->mode > MODE_STOPWATCH) {
		ba->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	if (bc->mode == data->mode) {
		return;
	}

	bc->mode = data->mode;

	// Stop clock and clear prescaler
	write_register(REG_STOP_ENABLE, REG_STOP_ENABLE_STOP_ENABLED);
	write_register(REG_RESET, REG_RESET_CLEAR_PRESCALER);

	// Reset clock and clear alarms
	uint8_t bytes[17] = {0};

	if (bc->mode == MODE_RTC) {
		bytes[4] = 1;
		bytes[5] = 6;
		bytes[6] = 1;
	}

	write_registers(REG_RAW_CLOCK_100TH_SECOND, bytes, 17);

	// Clear alarm flags
	uint8_t flags = read_register(REG_FLAGS);

	flags &= ~REG_FLAGS_ALARM1;
	flags &= ~REG_FLAGS_ALARM2;

	write_register(REG_FLAGS, flags);

	bc->flags = flags;

	// Switch clock mode
	uint8_t function = read_register(REG_FUNCTION);

	function &= ~REG_FUNCTION_RTCM_mask;

	if (bc->mode == MODE_RTC) {
		function |= REG_FUNCTION_RTCM_RTC;
	} else if (bc->mode == MODE_STOPWATCH) {
		function |= REG_FUNCTION_RTCM_STOPWATCH;
	}

	write_register(REG_FUNCTION, function);

	// Start clock
	write_register(REG_STOP_ENABLE, REG_STOP_ENABLE_STOP_DISABLED);
}

void get_mode(const ComType com, const GetMode *data) {
	GetModeReturn gmr;

	gmr.header        = data->header;
	gmr.header.length = sizeof(gmr);
	gmr.mode          = BC->mode;

	BA->send_blocking_with_timeout(&gmr, sizeof(gmr), com);
}

void set_rtc_date_time(const ComType com, const SetRTCDateTime *data) {
	if (BC->mode == MODE_RTC) {
		// FIXME: validate input values

		uint8_t bytes[8] = {
			data->centisecond,
			data->second,
			data->minute,
			data->hour,
			data->day,
			data->weekday,
			data->month,
			data->year - 2000
		};

		set_raw_clock(bytes, 8);
	}
}

void get_rtc_date_time(const ComType com, const GetRTCDateTime *data) {
	GetRTCDateTimeReturn grdtr = {{0}};

	grdtr.header        = data->header;
	grdtr.header.length = sizeof(grdtr);

	if (BC->mode == MODE_RTC) {
		uint8_t bytes[8];

		get_raw_clock(bytes, 8);

		grdtr.year        = 2000 + bytes[7];
		grdtr.month       = bytes[6];
		grdtr.day         = bytes[4];
		grdtr.hour        = bytes[3];
		grdtr.minute      = bytes[2];
		grdtr.second      = bytes[1];
		grdtr.centisecond = bytes[0];
		grdtr.weekday     = bytes[5];
	}

	BA->send_blocking_with_timeout(&grdtr, sizeof(grdtr), com);
}

void set_rtc_alarm(const ComType com, const SetRTCAlarm *data) {
	if (data->alarm > 1) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	if (BC->mode == MODE_RTC) {
		// FIXME: validate input values

		if (data->alarm == 0) {
			uint8_t bytes[5] = {
				data->second,
				data->minute,
				data->hour,
				data->day,
				data->month
			};

			set_raw_alarm(REG_RTC_ALARM1_SECOND, bytes, 5, REG_RTC_ALARM_ENABLE_A1E_mask, 0);
		} else {
			uint8_t bytes[3] = {
				data->minute,
				data->hour,
				data->weekday
			};

			set_raw_alarm(REG_RTC_ALARM2_MINUTE, bytes, 3, REG_RTC_ALARM_ENABLE_A2E_mask, 5);
		}
	}
}

void get_rtc_alarm(const ComType com, const GetRTCAlarm *data) {
	BrickletAPI *ba = BA;

	if (data->alarm > 1) {
		ba->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetRTCAlarmReturn grar = {{-1}};

	grar.header        = data->header;
	grar.header.length = sizeof(grar);

	if (BC->mode == MODE_RTC) {
		uint8_t bytes[5];

		if (data->alarm == 0) {
			get_raw_alarm(REG_RTC_ALARM1_SECOND, bytes, 5, 0);

			grar.month  = bytes[4];
			grar.day    = bytes[3];
			grar.hour   = bytes[2];
			grar.minute = bytes[1];
			grar.second = bytes[0];
		} else {
			get_raw_alarm(REG_RTC_ALARM2_MINUTE, bytes, 3, 5);

			grar.weekday = bytes[2];
			grar.hour    = bytes[1];
			grar.minute  = bytes[0];
		}
	}

	ba->send_blocking_with_timeout(&grar, sizeof(grar), com);
}

void set_stopwatch_time(const ComType com, const SetStopwatchTime *data) {
	if (BC->mode == MODE_STOPWATCH) {
		// FIXME: validate input values

		uint8_t bytes[6] = {
			data->centisecond,
			data->second,
			data->minute,
			data->hour % 100,
			(data->hour / 100) % 100,
			(data->hour / 10000) % 100,
		};

		set_raw_clock(bytes, 6);
	}
}

void get_stopwatch_time(const ComType com, const GetStopwatchTime *data) {
	GetStopwatchTimeReturn gstr = {{0}};

	gstr.header        = data->header;
	gstr.header.length = sizeof(gstr);

	if (BC->mode == MODE_STOPWATCH) {
		uint8_t bytes[6];

		get_raw_clock(bytes, 6);

		gstr.hour        = bytes[5] * 10000 + bytes[4] * 100 + bytes[3];
		gstr.minute      = bytes[2];
		gstr.second      = bytes[1];
		gstr.centisecond = bytes[0];
	}

	BA->send_blocking_with_timeout(&gstr, sizeof(gstr), com);
}

void set_stopwatch_alarm(const ComType com, const SetStopwatchAlarm *data) {
	BrickletAPI *ba = BA;

	if (data->alarm > 1) {
		ba->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	if (BC->mode == MODE_STOPWATCH) {
		// FIXME: validate input values

		const int32_t hour = data->hour;
		const uint8_t hour1 = hour % 100;
		const uint8_t hour2 = (hour / 100) % 100;
		const uint8_t hour3 = (hour / 10000) % 100;
		uint8_t bytes[5] = {-1};

		if (data->alarm == 0) {
			bytes[0] = data->second;
			bytes[1] = data->minute;

			if (hour >= 0) {
				bytes[2] = hour1;
				bytes[3] = hour2;
				bytes[4] = hour3;
			}

			set_raw_alarm(REG_STOPWATCH_ALARM1_SECOND, bytes, 5, REG_STOPWATCH_ALARM_ENABLE_A1E_mask, 0);
		} else {
			bytes[0] = data->minute;

			if (hour >= 0) {
				bytes[1] = hour1;
				bytes[2] = hour2;
			}

			set_raw_alarm(REG_STOPWATCH_ALARM2_MINUTE, bytes, 3, REG_STOPWATCH_ALARM_ENABLE_A2E_mask, 5);
		}
	}
}

void get_stopwatch_alarm(const ComType com, const GetStopwatchAlarm *data) {
	BrickletAPI *ba = BA;

	if (data->alarm > 1) {
		ba->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetStopwatchAlarmReturn gsar = {{-1}};

	gsar.header        = data->header;
	gsar.header.length = sizeof(gsar);

	if (BC->mode == MODE_STOPWATCH) {
		uint8_t bytes[5];

		if (data->alarm == 1) {
			get_raw_alarm(REG_STOPWATCH_ALARM1_SECOND, bytes, 5, 0);

			if ((int8_t)bytes[4] < 0) {
				gsar.hour = -1;
			} else {
				gsar.hour = bytes[4] * 10000 + bytes[3] * 100 + bytes[2];
			}

			gsar.minute = bytes[1];
			gsar.second = bytes[0];
		} else {
			get_raw_alarm(REG_STOPWATCH_ALARM2_MINUTE, bytes, 3, 5);

			if ((int8_t)bytes[2] < 0) {
				gsar.hour = -1;
			} else {
				gsar.hour = bytes[2] * 100 + bytes[1];
			}

			gsar.minute = bytes[0];
		}
	}

	ba->send_blocking_with_timeout(&gsar, sizeof(gsar), com);
}

void set_countdown(const ComType com, const SetCountdown *data) {
	// FIXME: validate input values

	uint8_t watchdog = 0;

	watchdog |= (data->mode    << REG_WATCHDOG_MODE_offset)    & REG_WATCHDOG_MODE_mask;
	watchdog |= (data->counter << REG_WATCHDOG_COUNTER_offset) & REG_WATCHDOG_COUNTER_mask;
	watchdog |= (data->step    << REG_WATCHDOG_STEP_offset)    & REG_WATCHDOG_STEP_mask;

	write_register(REG_WATCHDOG, watchdog);
}

void get_countdown(const ComType com, const GetCountdown *data) {
	GetCountdownReturn gcr;

	gcr.header        = data->header;
	gcr.header.length = sizeof(gcr);

	uint8_t watchdog = read_register(REG_WATCHDOG);

	gcr.mode    = (watchdog & REG_WATCHDOG_MODE_mask)    >> REG_WATCHDOG_MODE_offset;
	gcr.counter = (watchdog & REG_WATCHDOG_COUNTER_mask) >> REG_WATCHDOG_COUNTER_offset;
	gcr.step    = (watchdog & REG_WATCHDOG_STEP_mask)    >> REG_WATCHDOG_STEP_offset;

	BA->send_blocking_with_timeout(&gcr, sizeof(gcr), com);
}

void set_user_data(const ComType com, const SetUserData *data) {
	write_register(REG_WATCHDOG, data->data);
}

void get_user_data(const ComType com, const GetUserData *data) {
	GetUserDataReturn gudr;

	gudr.header        = data->header;
	gudr.header.length = sizeof(gudr);
	gudr.data          = BC->user_data;

	BA->send_blocking_with_timeout(&gudr, sizeof(gudr), com);
}

void set_calibration(const ComType com, const SetCalibration *data) {
	BrickletAPI *ba = BA;
	BrickContext *bc = BC;
	const char port = BS->port - 'a';

	if (data->mode > CALIBRATION_MODE_FAST_CORRECTION) {
		ba->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	bc->calibration[2] = data->mode;
	bc->calibration[3] = data->offset;

	uint8_t oscillator = REG_OSCILLATOR_CL_12PF;

	if (bc->calibration[2] != CALIBRATION_MODE_LOW_POWER) {
		oscillator |= REG_OSCILLATOR_OFFM_LOW_POWER;
	} else {
		oscillator |= REG_OSCILLATOR_OFFM_FAST_CONNECTION;
	}

	write_register(REG_OSCILLATOR, oscillator);
	write_register(REG_OFFSET, bc->calibration[3]);

	ba->bricklet_select(port);
	ba->i2c_eeprom_master_write(ba->twid->pTwi,
	                            CALIBRATION_EEPROM_POSITION,
	                            (const char*)bc->calibration,
	                            sizeof(bc->calibration));
	ba->bricklet_deselect(port);
}

void get_calibration(const ComType com, const GetCalibration *data) {
	BrickContext *bc = BC;
	GetCalibrationReturn gcr;

	gcr.header        = data->header;
	gcr.header.length = sizeof(gcr);
	gcr.mode          = bc->calibration[2];
	gcr.offset        = bc->calibration[3];

	BA->send_blocking_with_timeout(&gcr, sizeof(gcr), com);
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

void write_register(const uint8_t reg, uint8_t value) {
	i2c_start();
	i2c_send_byte((I2C_ADDRESS << 1) | I2C_WRITE);
	i2c_send_byte(reg);
	i2c_send_byte(value);
	i2c_stop();
}

void write_registers(const uint8_t reg, uint8_t *data, const uint8_t length) {
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
