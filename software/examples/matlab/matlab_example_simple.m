function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletRealTimeClock;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    rtc = handle(BrickletRealTimeClock(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current date and time
    dateTime = rtc.getDateTime();

    fprintf('Year: %i\n', dateTime.year);
    fprintf('Month: %i\n', dateTime.month);
    fprintf('Day: %i\n', dateTime.day);
    fprintf('Hour: %i\n', dateTime.hour);
    fprintf('Minute: %i\n', dateTime.minute);
    fprintf('Second: %i\n', dateTime.second);
    fprintf('Centisecond: %i\n', dateTime.centisecond);
    fprintf('Weekday: %i\n', dateTime.weekday);

    % Get current timestamp (unit is ms)
    timestamp = rtc.getTimestamp();
    fprintf('Timestamp: %i ms\n', timestamp);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
