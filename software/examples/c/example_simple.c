#include <stdio.h>
#include <inttypes.h>

#include "ip_connection.h"
#include "bricklet_real_time_clock.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

int main(void) {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	RealTimeClock rtc;
	real_time_clock_create(&rtc, UID, &ipcon);

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		return 1;
	}
	// Don't use device before ipcon is connected

	// Get current date and time
	uint16_t year; uint8_t month, day, hour, minute, second, centisecond, weekday;
	if(real_time_clock_get_date_time(&rtc, &year, &month, &day, &hour, &minute, &second, &centisecond, &weekday) < 0) {
		fprintf(stderr, "Could not get date and time, probably timeout\n");
		return 1;
	}

	printf("Year: %d\n", year);
	printf("Month: %d\n", month);
	printf("Day: %d\n", day);
	printf("Hour: %d\n", hour);
	printf("Minute: %d\n", minute);
	printf("Second: %d\n", second);
	printf("Centisecond: %d\n", centisecond);
	printf("Weekday: %d\n", weekday);

	// Get current timestamp (unit is ms)
	int64_t timestamp;
	if(real_time_clock_get_timestamp(&rtc, &timestamp) < 0) {
		fprintf(stderr, "Could not get timestamp, probably timeout\n");
		return 1;
	}

	printf("Timestamp: %"PRId64" ms\n", timestamp);

	printf("Press key to exit\n");
	getchar();
	real_time_clock_destroy(&rtc);
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	return 0;
}
