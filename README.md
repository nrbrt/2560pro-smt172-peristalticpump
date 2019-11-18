# 2560pro-smt172-peristalticpump
Controls a peristaltic pump, reports temperature from 2 smt172 sensors and measures differential pressure using a mpx7002dp sensor
The commands that can be sent are the following:

`pos:w`

where w is the amount of steps that will be moved CW or CCW. The motor will stop when the position is reached.

`rot:x`

where x is the speed in steps per second. This command will only work if the `spd` and `acc` are already set.
a postive x will rotate the motor CW a negative value for x will rotate the motor CCW. The motor will continuously turn until a `rot:0` command is sent.

`spd:y`

where y is the speed in steps per second to be used during rotation or positioning.

`acc:z`

where z is the acceleration in steps per second per second to be used during rotation or positioning. With this setting you can ramp up and down. Useful at higher speeds and microstepping

The Mega will report every second the temperatures in Celsius and the differential pressure in kPa using the format `t1:xx,t2:yy,p:zz`. If the temperature sensors are not present,
it will report -1 as the value for that sensor. Absence of the pressure sensor will result in a negative value caused by the floating A0 pin.

The install script is meant to be run on a raspberry pi like this:

`wget -O - https://raw.githubusercontent.com/nrbrt/2560pro-smt172-peristalticpump/master/install.sh | sh`

This will program the mega, that needs to be connected at that moment, without any user interaction and is meant for novice users
and easy installation.

The smt172 sensors need to be connected to pin 48 and pin 49 and need some electronics to connect to the Arduino as shown in the connection diagram, the drv8825 connects to pin 12(dir) and pin 11(step), the home-switch to pin 10 and the mpx7002dp connects to A0.
This sketch uses the great smt172 library and smt172 connection diagram picture by Edwin Croissant.
