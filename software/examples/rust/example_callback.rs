use std::{error::Error, io, thread};
use tinkerforge::{ip_connection::IpConnection, real_time_clock_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Real-Time Clock Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let rtc = RealTimeClockBricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    let date_time_receiver = rtc.get_date_time_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `rtc` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for date_time in date_time_receiver {
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

            println!("Timestamp: {}", date_time.timestamp);
            println!();
        }
    });

    // Set period for date and time receiver to 5s (5000ms).
    // Note: The date and time callback is only called every 5 seconds
    //       if the date and time has changed since the last call!
    rtc.set_date_time_callback_period(5000);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
