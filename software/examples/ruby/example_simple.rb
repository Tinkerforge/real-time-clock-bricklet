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

# Get current date and time (returned as [year, month, day, hour, minute, second, centisecond, weekday])
date_time = rtc.get_date_time

puts "Year: #{date_time[0]}"
puts "Month: #{date_time[1]}"
puts "Day: #{date_time[2]}"
puts "Hour: #{date_time[3]}"
puts "Minute: #{date_time[4]}"
puts "Second: #{date_time[5]}"
puts "Centisecond: #{date_time[6]}"
puts "Weekday: #{date_time[7]}"

# Get current timestamp (unit is ms)
timestamp = rtc.get_timestamp
puts "Timestamp: #{timestamp} ms"

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
