#include <LoopLauncher.h>


// define the lcd and pins
LiquidCrystal lcd(19, 18, 17, 16, 15, 14);

// define our loopLauncher instance and pass in the lcd
LoopLauncher launcher = LoopLauncher();

int contrastPin = 22;
void setup() {
  
  int launchPins[6] = {2, 1, 0, 8, 7, 6};
  launcher.begin(5, 4, 3, launchPins, 6, &lcd);
  
  pinMode(contrastPin, OUTPUT);
  analogWrite(contrastPin, 50);  

}

void loop() {
  launcher.update();
}
