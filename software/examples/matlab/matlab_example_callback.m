function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletRealTimeClock;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Real-Time Clock Bricklet

    ipcon = IPConnection(); % Create IP connection
    rtc = handle(BrickletRealTimeClock(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register date and time callback to function cb_date_time
    set(rtc, 'DateTimeCallback', @(h, e) cb_date_time(e));

    % Set period for date and time callback to 5s (5000ms)
    % Note: The date and time callback is only called every 5 seconds
    %       if the date and time has changed since the last call!
    rtc.setDateTimeCallbackPeriod(5000);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for date and time callback
function cb_date_time(e)
    fprintf('Year: %i\n', e.year);
    fprintf('Month: %i\n', e.month);
    fprintf('Day: %i\n', e.day);
    fprintf('Hour: %i\n', e.hour);
    fprintf('Minute: %i\n', e.minute);
    fprintf('Second: %i\n', e.second);
    fprintf('Centisecond: %i\n', e.centisecond);
    fprintf('Weekday: %i\n', e.weekday);
    fprintf('Timestamp: %i\n', e.timestamp);
    fprintf('\n');
end
