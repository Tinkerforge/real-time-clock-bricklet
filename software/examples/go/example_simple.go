package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
	"github.com/Tinkerforge/go-api-bindings/real_time_clock_bricklet"
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

	// Get current date and time.
	year, month, day, hour, minute, second, centisecond, weekday, _ := rtc.GetDateTime()

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

	// Get current timestamp.
	timestamp, _ := rtc.GetTimestamp()
	fmt.Printf("Timestamp: %d ms\n", timestamp)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
