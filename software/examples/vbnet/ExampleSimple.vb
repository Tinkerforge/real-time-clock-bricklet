Imports System
Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Real-Time Clock Bricklet

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim rtc As New BrickletRealTimeClock(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current date and time
        Dim year As Integer
        Dim month, day, hour, minute, second, centisecond, weekday As Byte

        rtc.GetDateTime(year, month, day, hour, minute, second, centisecond, weekday)

        Console.WriteLine("Year: " + year.ToString())
        Console.WriteLine("Month: " + month.ToString())
        Console.WriteLine("Day: " + day.ToString())
        Console.WriteLine("Hour: " + hour.ToString())
        Console.WriteLine("Minute: " + minute.ToString())
        Console.WriteLine("Second: " + second.ToString())
        Console.WriteLine("Centisecond: " + centisecond.ToString())

        If weekday = BrickletRealTimeClock.WEEKDAY_MONDAY Then
            Console.WriteLine("Weekday: Monday")
        Else If weekday = BrickletRealTimeClock.WEEKDAY_TUESDAY Then
            Console.WriteLine("Weekday: Tuesday")
        Else If weekday = BrickletRealTimeClock.WEEKDAY_WEDNESDAY Then
            Console.WriteLine("Weekday: Wednesday")
        Else If weekday = BrickletRealTimeClock.WEEKDAY_THURSDAY Then
            Console.WriteLine("Weekday: Thursday")
        Else If weekday = BrickletRealTimeClock.WEEKDAY_FRIDAY Then
            Console.WriteLine("Weekday: Friday")
        Else If weekday = BrickletRealTimeClock.WEEKDAY_SATURDAY Then
            Console.WriteLine("Weekday: Saturday")
        Else If weekday = BrickletRealTimeClock.WEEKDAY_SUNDAY Then
            Console.WriteLine("Weekday: Sunday")
        End If

        ' Get current timestamp
        Dim timestamp As Long = rtc.GetTimestamp()
        Console.WriteLine("Timestamp: " + timestamp.ToString() + " ms")

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
