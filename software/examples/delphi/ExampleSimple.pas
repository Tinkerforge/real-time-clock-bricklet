program ExampleSimple;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletRealTimeClock;

type
  TExample = class
  private
    ipcon: TIPConnection;
    rtc: TBrickletRealTimeClock;
  public
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change XYZ to the UID of your Real-Time Clock Bricklet }

var
  e: TExample;

procedure TExample.Execute;
var year: word; month, day, hour, minute, second, centisecond, weekday: byte;
    timestamp: int64;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  rtc := TBrickletRealTimeClock.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Get current date and time }
  rtc.GetDateTime(year, month, day, hour, minute, second, centisecond, weekday);

  WriteLn(Format('Year: %d', [year]));
  WriteLn(Format('Month: %d', [month]));
  WriteLn(Format('Day: %d', [day]));
  WriteLn(Format('Hour: %d', [hour]));
  WriteLn(Format('Minute: %d', [minute]));
  WriteLn(Format('Second: %d', [second]));
  WriteLn(Format('Centisecond: %d', [centisecond]));

  if (weekday = BRICKLET_REAL_TIME_CLOCK_WEEKDAY_MONDAY) then begin
    WriteLn('Weekday: Monday');
  end
  else if (weekday = BRICKLET_REAL_TIME_CLOCK_WEEKDAY_TUESDAY) then begin
    WriteLn('Weekday: Tuesday');
  end
  else if (weekday = BRICKLET_REAL_TIME_CLOCK_WEEKDAY_WEDNESDAY) then begin
    WriteLn('Weekday: Wednesday');
  end
  else if (weekday = BRICKLET_REAL_TIME_CLOCK_WEEKDAY_THURSDAY) then begin
    WriteLn('Weekday: Thursday');
  end
  else if (weekday = BRICKLET_REAL_TIME_CLOCK_WEEKDAY_FRIDAY) then begin
    WriteLn('Weekday: Friday');
  end
  else if (weekday = BRICKLET_REAL_TIME_CLOCK_WEEKDAY_SATURDAY) then begin
    WriteLn('Weekday: Saturday');
  end
  else if (weekday = BRICKLET_REAL_TIME_CLOCK_WEEKDAY_SUNDAY) then begin
    WriteLn('Weekday: Sunday');
  end;

  { Get current timestamp }
  timestamp := rtc.GetTimestamp;
  WriteLn(Format('Timestamp: %d ms', [timestamp]));

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
