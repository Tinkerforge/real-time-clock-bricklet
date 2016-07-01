#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletRealTimeClock;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Real-Time Clock Bricklet

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $rtc = Tinkerforge::BrickletRealTimeClock->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current date and time
my ($year, $month, $day, $hour, $minute, $second, $centisecond, $weekday) = $rtc->get_date_time();

print "Year: $year\n";
print "Month: $month\n";
print "Day: $day\n";
print "Hour: $hour\n";
print "Minute: $minute\n";
print "Second: $second\n";
print "Centisecond: $centisecond\n";
print "Weekday: $weekday\n";

# Get current timestamp (unit is ms)
my $timestamp = $rtc->get_timestamp();
print "Timestamp: $timestamp ms\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
