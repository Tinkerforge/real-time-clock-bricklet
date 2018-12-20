function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletRealTimeClock;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Real-Time Clock Bricklet

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

    if dateTime.weekday == BrickletRealTimeClock.WEEKDAY_MONDAY
        fprintf('Weekday: Monday\n');
    elseif dateTime.weekday == BrickletRealTimeClock.WEEKDAY_TUESDAY
        fprintf('Weekday: Tuesday\n');
    elseif dateTime.weekday == BrickletRealTimeClock.WEEKDAY_WEDNESDAY
        fprintf('Weekday: Wednesday\n');
    elseif dateTime.weekday == BrickletRealTimeClock.WEEKDAY_THURSDAY
        fprintf('Weekday: Thursday\n');
    elseif dateTime.weekday == BrickletRealTimeClock.WEEKDAY_FRIDAY
        fprintf('Weekday: Friday\n');
    elseif dateTime.weekday == BrickletRealTimeClock.WEEKDAY_SATURDAY
        fprintf('Weekday: Saturday\n');
    elseif dateTime.weekday == BrickletRealTimeClock.WEEKDAY_SUNDAY
        fprintf('Weekday: Sunday\n');
    end

    % Get current timestamp
    timestamp = rtc.getTimestamp();
    fprintf('Timestamp: %i ms\n', timestamp);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
