#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change to your UID

# Handle incoming date and time callbacks
tinkerforge dispatch real-time-clock-bricklet $uid date-time &

# Set period for date and time callback to 5s (5000ms)
# Note: The date and time callback is only called every 5 seconds
#       if the date and time has changed since the last call!
tinkerforge call real-time-clock-bricklet $uid set-date-time-callback-period 5000

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
