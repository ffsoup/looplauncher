# looplauncher

LoopLauncher is an experiment in midi control using a Teensy and stomp buttons to send note on/off messages to a host.

## Parts list
Parts Total $62
- 1 [Teensy 3.2 ($20)](https://www.pjrc.com/store/teensy32.html)
- 9 [SPST Soft Touch Momentary Footswitch ($18)](http://www.bitcheslovemyswitches.com/#!/SPST-Soft-Touch-Momentary-Footswitch-Normally-Open/p/10220659/category=5027572)
- 1 [16x2 LCD ($10)](https://www.adafruit.com/product/181)
- 1 [Panel Mount USB Cable - B Female to Micro-B Male ($4)](https://www.adafruit.com/products/937)
- 1 [1590DD Enclosure ($10)](http://www.bitcheslovemyswitches.com/#!/1590DD/p/20370169/category=5027533)
- Miscellaneous hookup wire


## Modifications
I ended up using an 8x10" piece of plexi and laying out the pedal as if it were going to be 8x5.  Laying out the design in plans to use a box that size and only finding the 7.4x4.7 worked out so that the layout fit in the underside of the enclosure, so I went with it.

Rather than wiring up an external contrast control, I ended up using PWM to set the contrast, I'll need to do something about the brightness as well, as I want to allow flexibility in the pedal at runtime without making physical modifications.

## Hookup and Layout
The trigger buttons run 1-3, left to right on the bottom and 4-6 left to right on the top.  The right three control buttons run vertically on the right hand side.
The controller is perf board mounted vertically with the usb port facing downward.  This will allow feeding the micro to B usb cable out the bottom of the perf board and back up to the back/top of the enclosure.
This layout allows the lcd to be installed vertically centered between the 2 & 4 buttons and leaves just enough room for a perf board to the right of the lcd.  I've routed the wires from each of the bottoms on the outside of each button post and to the far right side of the enclosure so that all buttons will present on the right side of the controller.
The lcd and perf board are mounted to the front of the enclosure (or in my modification to an acrylic face) and held back by standoffs.  The upside to the acrylic face is that I just press fit the lcd to the acrylic rather than cutting a through hole.

Buttons to Teensy
The trigger and control buttons are wired to pins 1-9 and the second lugs are all sent to the ground.

Lcd to Teensy
- Lcd 1 to GND
- Lcd 2 to Vin (+5V instead of the 3.3V pin)
- Lcd 3 to 22
- Lcd 4 to 19
- Lcd 5 to GND
- Lcd 6 to 18
- Lcd 7, 8, 9, 10 Not Used
- Lcd 11, 12, 13, 14 to 17, 16, 15, 14
- Lcd 15 to Vin (+5V no the 3.3V pin)
- Lcd 16 to Gnd
