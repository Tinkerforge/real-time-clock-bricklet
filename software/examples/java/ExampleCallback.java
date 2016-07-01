import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletRealTimeClock;

public class ExampleCallback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Real-Time Clock Bricklet
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletRealTimeClock rtc = new BrickletRealTimeClock(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Add date and time listener
		rtc.addDateTimeListener(new BrickletRealTimeClock.DateTimeListener() {
			public void dateTime(int year, short month, short day, short hour,
			                     short minute, short second, short centisecond,
			                     short weekday, long timestamp) {
				System.out.println("Year: " + year);
				System.out.println("Month: " + month);
				System.out.println("Day: " + day);
				System.out.println("Hour: " + hour);
				System.out.println("Minute: " + minute);
				System.out.println("Second: " + second);
				System.out.println("Centisecond: " + centisecond);
				System.out.println("Weekday: " + weekday);
				System.out.println("Timestamp: " + timestamp);
				System.out.println("");
			}
		});

		// Set period for date and time callback to 5s (5000ms)
		// Note: The date and time callback is only called every 5 seconds
		//       if the date and time has changed since the last call!
		rtc.setDateTimeCallbackPeriod(5000);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
