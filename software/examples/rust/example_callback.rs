use std::{error::Error, io, thread};
use tinkerforge::{ipconnection::IpConnection, real_time_clock_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Real-Time Clock Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let real_time_clock_bricklet = RealTimeClockBricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    //Create listener for date and time events.
    let date_time_listener = real_time_clock_bricklet.get_date_time_receiver();
    // Spawn thread to handle received events. This thread ends when the real_time_clock_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in date_time_listener {
            println!("Year: {}", event.year);
            println!("Month: {}", event.month);
            println!("Day: {}", event.day);
            println!("Hour: {}", event.hour);
            println!("Minute: {}", event.minute);
            println!("Second: {}", event.second);
            println!("Centisecond: {}", event.centisecond);
            println!("Weekday: {}", event.weekday);
            println!("Timestamp: {}", event.timestamp);
            println!();
        }
    });

    // Set period for date and time listener to 5s (5000ms)
    // Note: The date and time callback is only called every 5 seconds
    //       if the date and time has changed since the last call!
    real_time_clock_bricklet.set_date_time_callback_period(5000);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
