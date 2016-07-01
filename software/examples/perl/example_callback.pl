#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletRealTimeClock;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Real-Time Clock Bricklet

# Callback subroutine for date and time callback
sub cb_date_time
{
    my ($year, $month, $day, $hour, $minute, $second, $centisecond, $weekday, $timestamp) = @_;

    print "Year: $year\n";
    print "Month: $month\n";
    print "Day: $day\n";
    print "Hour: $hour\n";
    print "Minute: $minute\n";
    print "Second: $second\n";
    print "Centisecond: $centisecond\n";
    print "Weekday: $weekday\n";
    print "Timestamp: $timestamp\n";
    print "\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $rtc = Tinkerforge::BrickletRealTimeClock->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register date and time callback to subroutine cb_date_time
$rtc->register_callback($rtc->CALLBACK_DATE_TIME, 'cb_date_time');

# Set period for date and time callback to 5s (5000ms)
# Note: The date and time callback is only called every 5 seconds
#       if the date and time has changed since the last call!
$rtc->set_date_time_callback_period(5000);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
