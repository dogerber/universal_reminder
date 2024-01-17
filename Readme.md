
# Universal reminder

TODO - VIdeo

Did I already take these pills? Did I water the plants today? This device helps you track repetitive tasks. Set the times of day something needs doing and the message that should be displayed. When the task is done, shake the device to make the screen display, that it is done. The device will reset when the next task is due.

The device uses a eInk display and enters deep sleep in between tasks, to save as much energy as possible. 


## How to build
- Connect wires as shown in TODO
- 3D print enclosure from TODO
- Install these libraries:
	- [RTClib](https://github.com/adafruit/RTClib)
	- [Adafruit EPD](https://github.com/adafruit/Adafruit_EPD)
	- [Adafruit GFX and Adafruit GFX library](https://github.com/adafruit/Adafruit-GFX-Library)
	- [Arduino Low Power](https://github.com/arduino-libraries/ArduinoLowPower)
- Set time on RTC with this script TODO
- adjust the messages and times for the tasks in TODO
- upload TODO


## Troubleshooting
If the device every becomes unrepsonsive (can not upload code and power LED does not switch on), double-click the reset button and upload the basic "Blink" example. This always happens when the device enters deep sleep, so for trouble shooting replace deep sleep with a simple delay().

## Resources
- [Arduino Guide to low power design](https://docs.arduino.cc/learn/electronics/low-power)
- [Good resource on interrupts by Nick Gammon](http://gammon.com.au/interrupts)