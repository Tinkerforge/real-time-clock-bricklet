use std::{error::Error, io};

use tinkerforge::{ip_connection::IpConnection, real_time_clock_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Real-Time Clock Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let rtc = RealTimeClockBricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Get current date and time.
    let get_date_time_result = rtc.get_date_time().recv()?;

    println!("Year: {}", get_date_time_result.year);
    println!("Month: {}", get_date_time_result.month);
    println!("Day: {}", get_date_time_result.day);
    println!("Hour: {}", get_date_time_result.hour);
    println!("Minute: {}", get_date_time_result.minute);
    println!("Second: {}", get_date_time_result.second);
    println!("Centisecond: {}", get_date_time_result.centisecond);
    println!("Weekday: {}", get_date_time_result.weekday);

    // Get current timestamp.
    let timestamp = rtc.get_timestamp().recv()?;
    println!("Timestamp: {} ms", timestamp);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
