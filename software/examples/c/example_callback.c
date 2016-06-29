#include <stdio.h>
#include <inttypes.h>

#include "ip_connection.h"
#include "bricklet_real_time_clock.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

// Callback function for date and time callback
void cb_date_time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute,
                  uint8_t second, uint8_t centisecond, uint8_t weekday, int64_t timestamp,
                  void *user_data) {
	(void)user_data; // avoid unused parameter warning

	printf("Year: %d\n", year);
	printf("Month: %d\n", month);
	printf("Day: %d\n", day);
	printf("Hour: %d\n", hour);
	printf("Minute: %d\n", minute);
	printf("Second: %d\n", second);
	printf("Centisecond: %d\n", centisecond);
	printf("Weekday: %d\n", weekday);
	printf("Timestamp: %"PRId64"\n", timestamp);
	printf("\n");
}

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

	// Register date and time callback to function cb_date_time
	real_time_clock_register_callback(&rtc,
	                                  REAL_TIME_CLOCK_CALLBACK_DATE_TIME,
	                                  (void *)cb_date_time,
	                                  NULL);

	// Set period for date and time callback to 5s (5000ms)
	// Note: The date and time callback is only called every 5 seconds
	//       if the date and time has changed since the last call!
	real_time_clock_set_date_time_callback_period(&rtc, 5000);

	printf("Press key to exit\n");
	getchar();
	real_time_clock_destroy(&rtc);
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	return 0;
}
