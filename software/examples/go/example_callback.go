package main

import (
	"fmt"
	"tinkerforge/ipconnection"
	"tinkerforge/real_time_clock_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Real-Time Clock Bricklet.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	rtc, _ := real_time_clock_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	rtc.RegisterDateTimeCallback(func(year uint16, month uint8, day uint8, hour uint8, minute uint8, second uint8, centisecond uint8, weekday real_time_clock_bricklet.Weekday, timestamp int64) {
		fmt.Printf("Year: %d\n", year)
		fmt.Printf("Month: %d\n", month)
		fmt.Printf("Day: %d\n", day)
		fmt.Printf("Hour: %d\n", hour)
		fmt.Printf("Minute: %d\n", minute)
		fmt.Printf("Second: %d\n", second)
		fmt.Printf("Centisecond: %d\n", centisecond)

		if weekday == real_time_clock_bricklet.WeekdayMonday {
			fmt.Println("Weekday: Monday")
		} else if weekday == real_time_clock_bricklet.WeekdayTuesday {
			fmt.Println("Weekday: Tuesday")
		} else if weekday == real_time_clock_bricklet.WeekdayWednesday {
			fmt.Println("Weekday: Wednesday")
		} else if weekday == real_time_clock_bricklet.WeekdayThursday {
			fmt.Println("Weekday: Thursday")
		} else if weekday == real_time_clock_bricklet.WeekdayFriday {
			fmt.Println("Weekday: Friday")
		} else if weekday == real_time_clock_bricklet.WeekdaySaturday {
			fmt.Println("Weekday: Saturday")
		} else if weekday == real_time_clock_bricklet.WeekdaySunday {
			fmt.Println("Weekday: Sunday")
		}

		fmt.Printf("Timestamp: %d\n", timestamp)
		fmt.Println()
	})

	// Set period for date and time receiver to 5s (5000ms).
	// Note: The date and time callback is only called every 5 seconds
	//       if the date and time has changed since the last call!
	rtc.SetDateTimeCallbackPeriod(5000)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
