function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Real-Time Clock Bricklet

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    rtc = java_new("com.tinkerforge.BrickletRealTimeClock", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current date and time
    dateTime = rtc.getDateTime();

    fprintf("Year: %d\n", dateTime.year);
    fprintf("Month: %d\n", java2int(dateTime.month));
    fprintf("Day: %d\n", java2int(dateTime.day));
    fprintf("Hour: %d\n", java2int(dateTime.hour));
    fprintf("Minute: %d\n", java2int(dateTime.minute));
    fprintf("Second: %d\n", java2int(dateTime.second));
    fprintf("Centisecond: %d\n", java2int(dateTime.centisecond));
    fprintf("Weekday: %d\n", java2int(dateTime.weekday));

    % Get current timestamp (unit is ms)
    timestamp = rtc.getTimestamp();
    fprintf("Timestamp: %d ms\n", java2int(timestamp));

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

function int = java2int(value)
    if compare_versions(version(), "3.8", "<=")
        int = value.intValue();
    else
        int = value;
    end
end
