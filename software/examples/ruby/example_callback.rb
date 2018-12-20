#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_real_time_clock'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Real-Time Clock Bricklet

ipcon = IPConnection.new # Create IP connection
rtc = BrickletRealTimeClock.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register date and time callback
rtc.register_callback(BrickletRealTimeClock::CALLBACK_DATE_TIME) do |year, month, day,
                                                                     hour, minute, second,
                                                                     centisecond, weekday,
                                                                     timestamp|
  puts "Year: #{year}"
  puts "Month: #{month}"
  puts "Day: #{day}"
  puts "Hour: #{hour}"
  puts "Minute: #{minute}"
  puts "Second: #{second}"
  puts "Centisecond: #{centisecond}"

  if weekday == BrickletRealTimeClock::WEEKDAY_MONDAY
    puts "Weekday: Monday"
  elsif weekday == BrickletRealTimeClock::WEEKDAY_TUESDAY
    puts "Weekday: Tuesday"
  elsif weekday == BrickletRealTimeClock::WEEKDAY_WEDNESDAY
    puts "Weekday: Wednesday"
  elsif weekday == BrickletRealTimeClock::WEEKDAY_THURSDAY
    puts "Weekday: Thursday"
  elsif weekday == BrickletRealTimeClock::WEEKDAY_FRIDAY
    puts "Weekday: Friday"
  elsif weekday == BrickletRealTimeClock::WEEKDAY_SATURDAY
    puts "Weekday: Saturday"
  elsif weekday == BrickletRealTimeClock::WEEKDAY_SUNDAY
    puts "Weekday: Sunday"
  end

  puts "Timestamp: #{timestamp}"
  puts ''
end

# Set period for date and time callback to 5s (5000ms)
# Note: The date and time callback is only called every 5 seconds
#       if the date and time has changed since the last call!
rtc.set_date_time_callback_period 5000

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
