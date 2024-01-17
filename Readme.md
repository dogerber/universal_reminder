
# Universal reminder

TODO - Video

Did I already take these pills? Did I water the plants today? This device helps you track repetitive tasks. Set the times of day something needs doing and the message that should be displayed. When the task is done, shake the device to make the screen display, that it is done. The device will reset when the next task is due.

The device uses a eInk display and enters deep sleep in between tasks, to save as much energy as possible. 

## Used parts

- [Adafruit Feather M0 Basic Proto](https://www.adafruit.com/product/2772)
- [Adafruit 2.9" Grayscale eInk](https://www.adafruit.com/product/4777)
- [Adafruit PCF8523 Real Time Clock](https://www.adafruit.com/product/3295)
- Tilt Sensor ([for example this](https://naltronic.de/zubehoer/elek.-bauteile/sonstige/651/1x-sw-200d-vibration-erschuetterung-neigung-sensor-kugel-tilt-schalter-sw200d))
- 1x Resistor (1 kOhm or similar)
- Battery (CR1220)
- LiPo Battery (I use a swatronic 1100 mAh from [here](https://www.swaytronic.ch/SWAYTRONIC-LiPo-1S-3.7V-1100mAh-35C-70C-JST))
- 3D printed enclosure (available in this repo or [here on Onshape](https://cad.onshape.com/documents/8f49303399ef22a7e39bc2af/w/fa5502a23fe9f76c9c39dbc8/e/11aa9429d28955ad08e092fa?renderMode=0&uiState=632acc33edc92c60b2c7bdb9))

<details> 
  <summary>Wiring diagram (click to open)</summary>
   <img src="https://github.com/dogerber/universal_reminder/blob/main/img/wiring_diagram.png" />
</details><br>


## How to build
- Connect wires as shown [here](img/wiring_diagram.png)
- 3D print enclosure from [/3D_print](/3D_print)
- Install these libraries:
	- [RTClib](https://github.com/adafruit/RTClib)
	- [Adafruit EPD](https://github.com/adafruit/Adafruit_EPD)
	- [Adafruit GFX and Adafruit GFX library](https://github.com/adafruit/Adafruit-GFX-Library)
	- [Arduino Low Power](https://github.com/arduino-libraries/ArduinoLowPower)
- Set time on RTC with this script TODO
- adjust the messages (``str_*``) and ```times[]``` for the tasks in ```universal_reminder.ino```
- upload ```universal_reminder.ino```


## Troubleshooting
If the device every becomes unrepsonsive (can not upload code and power LED does not switch on), double-click the reset button and upload the basic "Blink" example. This always happens when the device enters deep sleep, so for trouble shooting replace deep sleep with a simple delay().

## Resources
- [Arduino Guide to low power design](https://docs.arduino.cc/learn/electronics/low-power)
- [Good resource on interrupts by Nick Gammon](http://gammon.com.au/interrupts)