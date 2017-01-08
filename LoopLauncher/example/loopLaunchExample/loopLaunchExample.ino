#include <LoopLauncher.h>

int launchPins[6]= {0, 1, 2, 6, 7, 8};

// define the lcd and pins
LiquidCrystal lcd(19, 18, 17, 16, 15, 14);

// define our loopLauncher instance and pass in the lcd
LoopLauncher launcher(3, 4, 5, launchPins, 6, &lcd);

void setup() {
  
}

void loop() {
  launcher.update();
}
