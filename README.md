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
- Lcd 3 to 22 (we'll send a PWM to control contrast)
- Lcd 4 to 19
- Lcd 5 to GND
- Lcd 6 to 18
- Lcd 7, 8, 9, 10 Not Used
- Lcd 11, 12, 13, 14 to 17, 16, 15, 14
- Lcd 15 to Vin (+5V no the 3.3V pin)
- Lcd 16 to Gnd

Buttons to Teensy
- Trigger buttons and control buttons wired to 0-9

## Code
The LoopLauncher lib and an example that presumes you wired things up like I did is in the LoopLauncher subdirectory.  The pedal functions similar to the Looptimus pedal.  The 6 trigger buttons send note on/off when pressed according to the bank that you are in.  By default, when the pedal is powered, the first bank is selected (displayed as 1, but indexed at 0 in the code).  Hack away.

```c++
#include <LoopLauncher.h>

LoopLauncher launcher = LoopLauncher();
LiquidCrystal lcd(19, 18, 17, 16, 15, 14);

void setup() {
  int launchPins[6] = {2, 1, 0, 8, 7, 6};
  launcher.begin(5, 4, 3, launchPins, 6, &lcd);
}

void loop() {
  launcher.update();
}
```

That's all there is to running it.  This version presumes an lcd is used to display.  A 7-segment display could also be used, but you'd have to adjust the code as well.  I'll likely make the next one w a 4-digit 7-segment display to simplify config.  MidiChannel can be controlled in the code ... I suppose that should be made configurable from the pedal as well.

*Caveats*
I'm not really settled with the implementation.  I'm sure it's sloppy all over.  Feel free to suggest/make contribute changes.

### What's Next?
- Adding a means to send static notes regardless of selected bank or mode (perhaps long pressing to get to an alternate mode an then sending set notes for each button).
- Adding a root/5th mode to allow playing of pads rather than triggering.  The alternate mode will behave differently in that a triggered note will not send on off when the button is released.  Rather another button or 'stop' will have to be pressed to end the previous note.
-  Adding a means to change modes (Normal vs Pad)
 
