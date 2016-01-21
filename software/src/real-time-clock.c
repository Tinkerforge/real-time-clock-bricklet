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
		case FID_SET_MODE:
			set_mode(com, (SetMode*)data);
			return;

		case FID_GET_MODE:
			get_mode(com, (GetMode*)data);
			return;

		case FID_SET_RTC_DATE_TIME:
			set_rtc_date_time(com, (SetRTCDateTime*)data);
			return;

		case FID_GET_RTC_DATE_TIME:
			get_rtc_date_time(com, (GetRTCDateTime*)data);
			return;

		case FID_SET_RTC_ALARM1:
			set_rtc_alarm1(com, (SetRTCAlarm1*)data);
			return;

		case FID_GET_RTC_ALARM1:
			get_rtc_alarm1(com, (GetRTCAlarm1*)data);
			return;

		case FID_SET_RTC_ALARM2:
			set_rtc_alarm2(com, (SetRTCAlarm2*)data);
			return;

		case FID_GET_RTC_ALARM2:
			get_rtc_alarm2(com, (GetRTCAlarm2*)data);
			return;

		case FID_SET_STOPWATCH_TIME:
			set_stopwatch_time(com, (SetStopwatchTime*)data);
			return;

		case FID_GET_STOPWATCH_TIME:
			get_stopwatch_time(com, (GetStopwatchTime*)data);
			return;

		case FID_SET_STOPWATCH_ALARM1:
			set_stopwatch_alarm1(com, (SetStopwatchAlarm1*)data);
			return;

		case FID_GET_STOPWATCH_ALARM1:
			get_stopwatch_alarm1(com, (GetStopwatchAlarm1*)data);
			return;

		case FID_SET_STOPWATCH_ALARM2:
			set_stopwatch_alarm2(com, (SetStopwatchAlarm2*)data);
			return;

		case FID_GET_STOPWATCH_ALARM2:
			get_stopwatch_alarm2(com, (GetStopwatchAlarm2*)data);
			return;

		case FID_SET_COUNTDOWN:
			set_countdown(com, (SetCountdown*)data);
			return;

		case FID_GET_COUNTDOWN:
			get_countdown(com, (GetCountdown*)data);
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

	// Read RTCM bit
	uint8_t function = read_register(REG_FUNCTION);

	if ((function & REG_FUNCTION_RTCM_mask) == REG_FUNCTION_RTCM_RTC) {
		BC->mode = MODE_RTC;
	} else {
		BC->mode = MODE_STOPWATCH;
	}

	// Set 24h mode and select 12.5pF oscillator load capacitance
	write_register(REG_OSCILLATOR, REG_OSCILLATOR_1224_24 | REG_OSCILLATOR_CL_12PF);

	// Enable centiseconds
	//function = (function & ~REG_FUNCTION_100TH_mask) | REG_FUNCTION_100TH_ENABLED;

	//write_register(REG_FUNCTION, function);

	// Enable nINTA output pin
	write_register(REG_PIN_IO, REG_PIN_IO_INTAPM_INTA);

	// Enable nINTA interrupts
	write_register(REG_INTA_ENABLE, REG_INTA_ENABLE_WATCHDOG | REG_INTA_ENABLE_ALARM1 |
	                                REG_INTA_ENABLE_ALARM2 | REG_INTA_ENABLE_LEVEL_MODE);
}

void destructor(void) {
}

void tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(!(PIN_INT.pio->PIO_PDSR & PIN_INT.mask)) { // nINTA
			BC->flags = read_register(REG_FLAGS);
			write_register(REG_FLAGS, 0);
		}


		/*BC->tick++;

		if (BC->tick % 1000 == 0) {
			uint8_t data[8] = {5,5,5,5,5,5,5,5};

			read_registers(REG_RTC_TIME_100TH_SECOND, data, 8);
			BA->printf("%d %d %d %d %d %d %d %d\n\r", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);

			read_registers(REG_OSCILLATOR, data, 1);
			BA->printf("o %d\n\r", data[0]);

			read_registers(REG_FUNCTION, data, 1);
			BA->printf("f %d\n\r", data[0]);

			read_registers(REG_STOP, data, 1);
			BA->printf("s %d\n\r", data[0]);
*/
			/*if (data[1] & (1 << 7)) {
				BA->printf("clearing os\n\r");
				uint8_t data[8] = {5,5,5,5,5,5,5,5};

				write_registers(REG_RTC_TIME_100TH_SECONDS, data, 8);
			}*/
		//}
	}

	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		const uint8_t flags = BC->flags;
		
		if (flags & (REG_FLAGS_ALARM1 | REG_FLAGS_ALARM2)) {
			if (BC->mode == MODE_RTC) {
				RTCAlarm ra;
				
				ra.alarm = flags & REG_FLAGS_ALARM1 ? 1 : 2;

				BA->com_make_default_header(&ra, BS->uid, sizeof(ra), FID_RTC_ALARM);
				BA->send_blocking_with_timeout(&ra, sizeof(ra), *BA->com_current);
			} else if (BC->mode == MODE_STOPWATCH) {
				StopwatchAlarm swa;
				
				swa.alarm = flags & REG_FLAGS_ALARM1 ? 1 : 2;

				BA->com_make_default_header(&swa, BS->uid, sizeof(swa), FID_STOPWATCH_ALARM);
				BA->send_blocking_with_timeout(&swa, sizeof(swa), *BA->com_current);
			}
		}

		if (BC->flags & REG_FLAGS_WATCHDOG) {
			Countdown c;

			BA->com_make_default_header(&c, BS->uid, sizeof(c), FID_COUNTDOWN);
			BA->send_blocking_with_timeout(&c, sizeof(c), *BA->com_current);
		}

		BC->flags = 0;
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
	uint8_t enable = read_register(REG_RAW_ALARM_ENABLE);

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
	if(data->mode > MODE_STOPWATCH) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	if (BC->mode == data->mode) {
		return;
	}

	BC->mode = data->mode;

	// Stop clock and clear prescaler
	write_register(REG_STOP_ENABLE, REG_STOP_ENABLE_STOP_ENABLED);
	write_register(REG_RESET, REG_RESET_CLEAR_PRESCALER);

	// Reset clock and clear alarms
	uint8_t bytes[17] = {0};

	if (BC->mode == MODE_RTC) {
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
	 
	BC->flags = flags;

	// Switch clock mode
	// FIXME: not enough code space
	/*uint8_t function = read_register(REG_FUNCTION);

	function &= ~REG_FUNCTION_RTCM_mask;

	if (BC->mode == MODE_RTC) {
		function |= REG_FUNCTION_RTCM_RTC;
	} else if (BC->mode == MODE_STOPWATCH) {
		function |= REG_FUNCTION_RTCM_STOPWATCH;
	}

	write_register(REG_FUNCTION, function);*/

	// Start clock
	write_register(REG_STOP_ENABLE, REG_STOP_ENABLE_STOP_DISABLED);

	BA->com_return_setter(com, data);
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

	BA->com_return_setter(com, data);
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

void set_rtc_alarm1(const ComType com, const SetRTCAlarm1 *data) {
	if (BC->mode == MODE_RTC) {
		// FIXME: validate input values

		uint8_t bytes[5] = {
			data->second,
			data->minute,
			data->hour,
			data->day,
			data->month
		};

		set_raw_alarm(REG_RTC_ALARM1_SECOND, bytes, 5, REG_RTC_ALARM_ENABLE_A1E_mask, 0);
	}

	BA->com_return_setter(com, data);
}

void get_rtc_alarm1(const ComType com, const GetRTCAlarm1 *data) {
	GetRTCAlarm1Return gra1r = {{0}};

	gra1r.header        = data->header;
	gra1r.header.length = sizeof(gra1r);

	if (BC->mode == MODE_RTC) {
		uint8_t bytes[5];

		get_raw_alarm(REG_RTC_ALARM1_SECOND, bytes, 5, 0);

		gra1r.month  = bytes[4];
		gra1r.day    = bytes[3];
		gra1r.hour   = bytes[2];
		gra1r.minute = bytes[1];
		gra1r.second = bytes[0];
	}

	BA->send_blocking_with_timeout(&gra1r, sizeof(gra1r), com);
}

void set_rtc_alarm2(const ComType com, const SetRTCAlarm2 *data) {
	if (BC->mode == MODE_RTC) {
		// FIXME: validate input values

		uint8_t bytes[3] = {
			data->minute,
			data->hour,
			data->weekday
		};

		set_raw_alarm(REG_RTC_ALARM2_MINUTE, bytes, 3, REG_RTC_ALARM_ENABLE_A2E_mask, 5);
	}

	BA->com_return_setter(com, data);
}

void get_rtc_alarm2(const ComType com, const GetRTCAlarm2 *data) {
	GetRTCAlarm2Return gra2r = {{0}};

	gra2r.header        = data->header;
	gra2r.header.length = sizeof(gra2r);

	if (BC->mode == MODE_RTC) {
		uint8_t bytes[3];

		get_raw_alarm(REG_RTC_ALARM2_MINUTE, bytes, 3, 5);

		gra2r.weekday = bytes[2];
		gra2r.hour    = bytes[1];
		gra2r.minute  = bytes[0];
	}

	BA->send_blocking_with_timeout(&gra2r, sizeof(gra2r), com);
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

	BA->com_return_setter(com, data);
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

void set_stopwatch_alarm1(const ComType com, const SetStopwatchAlarm1 *data) {
	if (BC->mode == MODE_STOPWATCH) {
		// FIXME: validate input values

		uint8_t bytes[5] = {
			data->second,
			data->minute,
			data->hour % 100,
			(data->hour / 100) % 100,
			(data->hour / 10000) % 100,
		};

		set_raw_alarm(REG_STOPWATCH_ALARM1_SECOND, bytes, 5, REG_STOPWATCH_ALARM_ENABLE_A1E_mask, 0);
	}

	BA->com_return_setter(com, data);
}

void get_stopwatch_alarm1(const ComType com, const GetStopwatchAlarm1 *data) {
	GetStopwatchAlarm1Return gsa1r = {{0}};

	gsa1r.header        = data->header;
	gsa1r.header.length = sizeof(gsa1r);

	if (BC->mode == MODE_STOPWATCH) {
		uint8_t bytes[5];

		get_raw_alarm(REG_STOPWATCH_ALARM1_SECOND, bytes, 5, 0);

		if ((int8_t)bytes[4] < 0) {
			gsa1r.hour = -1;
		} else {
			gsa1r.hour = bytes[4] * 10000 + bytes[3] * 100 + bytes[2];
		}

		gsa1r.minute = bytes[1];
		gsa1r.second = bytes[0];
	}

	BA->send_blocking_with_timeout(&gsa1r, sizeof(gsa1r), com);
}

void set_stopwatch_alarm2(const ComType com, const SetStopwatchAlarm2 *data) {
	if (BC->mode == MODE_STOPWATCH) {
		// FIXME: validate input values

		uint8_t bytes[3] = {
			data->minute,
			data->hour % 100,
			(data->hour / 100) % 100,
		};

		set_raw_alarm(REG_STOPWATCH_ALARM2_MINUTE, bytes, 3, REG_STOPWATCH_ALARM_ENABLE_A2E_mask, 5);
	}

	BA->com_return_setter(com, data);
}

void get_stopwatch_alarm2(const ComType com, const GetStopwatchAlarm2 *data) {
	GetStopwatchAlarm2Return gsa2r = {{0}};

	gsa2r.header        = data->header;
	gsa2r.header.length = sizeof(gsa2r);

	if (BC->mode == MODE_STOPWATCH) {
		uint8_t bytes[3];

		get_raw_alarm(REG_STOPWATCH_ALARM2_MINUTE, bytes, 3, 5);

		if ((int8_t)bytes[2] < 0) {
			gsa2r.hour = -1;
		} else {
			gsa2r.hour = bytes[2] * 100 + bytes[1];
		}

		gsa2r.minute = bytes[0];
	}

	BA->send_blocking_with_timeout(&gsa2r, sizeof(gsa2r), com);
}

void set_countdown(const ComType com, const SetCountdown *data) {
	if(data->mode > COUNTDOWN_MODE_REPEAT ||
	   data->counter > 31 ||
	   data->step > COUNTDOWN_STEP_62MS) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	uint8_t watchdog = 0;

	watchdog |= data->mode    << REG_WATCHDOG_MODE_offset;
	watchdog |= data->counter << REG_WATCHDOG_COUNTER_offset;
	watchdog |= data->step    << REG_WATCHDOG_STEP_offset;

	write_register(REG_WATCHDOG, watchdog);

	BA->com_return_setter(com, data);
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

uint8_t read_register(const uint8_t reg) {
	uint8_t value;

	read_registers(reg, &value, 1);

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

void clear_interrupt() { // FIXME: color bricklet specific
	i2c_start();
	i2c_send_byte((I2C_ADDRESS << 1) | I2C_WRITE);
	i2c_send_byte(0b11100110);
	i2c_stop();
}

void write_register(const uint8_t reg, uint8_t value) {
	write_registers(reg, &value, 1);
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

void foobar(bool high) {
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
	/*i2c_scl_low();

	if(ack) {
		i2c_sda_low();
	} else {
		i2c_sda_high();
	}

	i2c_sleep_halfclock();
	i2c_scl_high();
	i2c_sleep_halfclock();*/
	foobar(!ack);

	return value;
}

bool i2c_send_byte(const uint8_t value) {
	for(int8_t i = 7; i >= 0; i--) {
		/*i2c_scl_low();

		if((value >> i) & 1) {
			i2c_sda_high();
		} else {
			i2c_sda_low();
		}

		i2c_sleep_halfclock();
		i2c_scl_high();
		i2c_sleep_halfclock();*/
		foobar((value >> i) & 1);
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
