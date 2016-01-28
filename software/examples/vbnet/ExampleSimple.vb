Imports System
Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change to your UID

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim rtc As New BrickletRealTimeClock(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current date and time
        Dim year As Integer
        Dim month As Byte
        Dim day As Byte
        Dim hour As Byte
        Dim minute As Byte
        Dim second As Byte
        Dim centisecond As Byte
        Dim weekday As Byte

        rtc.GetDateTime(year, month, day, hour, minute, second, centisecond, weekday)

        Console.WriteLine("Year: " + year.ToString())
        Console.WriteLine("Month: " + month.ToString())
        Console.WriteLine("Day: " + day.ToString())
        Console.WriteLine("Hour: " + hour.ToString())
        Console.WriteLine("Minute: " + minute.ToString())
        Console.WriteLine("Second: " + second.ToString())
        Console.WriteLine("Centisecond: " + centisecond.ToString())
        Console.WriteLine("Weekday: " + weekday.ToString())

        ' Get current timestamp (unit is ms)
        Dim timestamp As Long = rtc.GetTimestamp()
        Console.WriteLine("Timestamp: " + timestamp.ToString() + " ms")

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
