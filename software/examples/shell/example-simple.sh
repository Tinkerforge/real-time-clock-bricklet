#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Real-Time Clock Bricklet

# Get current date and time
tinkerforge call real-time-clock-bricklet $uid get-date-time

# Get current timestamp (unit is ms)
tinkerforge call real-time-clock-bricklet $uid get-timestamp
