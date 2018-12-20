use std::{io, error::Error};

use tinkerforge::{ip_connection::IpConnection, 
                  real_time_clock_bricklet::*};


const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Real-Time Clock Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let rtc = RealTimeClockBricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
<<<<<<< Updated upstream
                                          // Don't use device before ipcon is connected.

    // Get current date and time.
    let date_time = rtc.get_date_time().recv()?;

    println!("Year: {}", date_time.year);
    println!("Month: {}", date_time.month);
    println!("Day: {}", date_time.day);
    println!("Hour: {}", date_time.hour);
    println!("Minute: {}", date_time.minute);
    println!("Second: {}", date_time.second);
    println!("Centisecond: {}", date_time.centisecond);

    if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_MONDAY {
        println!("Weekday: Monday");
    } else if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_TUESDAY {
        println!("Weekday: Tuesday");
    } else if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_WEDNESDAY {
        println!("Weekday: Wednesday");
    } else if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_THURSDAY {
        println!("Weekday: Thursday");
    } else if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_FRIDAY {
        println!("Weekday: Friday");
    } else if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_SATURDAY {
        println!("Weekday: Saturday");
    } else if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_SUNDAY {
        println!("Weekday: Sunday");
    }

    // Get current timestamp.
    let timestamp = rtc.get_timestamp().recv()?;
    println!("Timestamp: {} ms", timestamp);
=======
    // Don't use device before ipcon is connected.

		// Get current date and time.
let date_time = rtc.get_date_time().recv()?;

		println!("Year: {}", date_time.year);
		println!("Month: {}", date_time.month);
		println!("Day: {}", date_time.day);
		println!("Hour: {}", date_time.hour);
		println!("Minute: {}", date_time.minute);
		println!("Second: {}", date_time.second);
		println!("Centisecond: {}", date_time.centisecond);

		if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_MONDAY {
			println!("Weekday: Monday");
		}
		else if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_TUESDAY {
			println!("Weekday: Tuesday");
		}
		else if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_WEDNESDAY {
			println!("Weekday: Wednesday");
		}
		else if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_THURSDAY {
			println!("Weekday: Thursday");
		}
		else if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_FRIDAY {
			println!("Weekday: Friday");
		}
		else if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_SATURDAY {
			println!("Weekday: Saturday");
		}
		else if date_time.weekday == REAL_TIME_CLOCK_BRICKLET_WEEKDAY_SUNDAY {
			println!("Weekday: Sunday");
		}

		// Get current timestamp.
let timestamp = rtc.get_timestamp().recv()?;
		println!("Timestamp: {} ms", timestamp);
>>>>>>> Stashed changes

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
