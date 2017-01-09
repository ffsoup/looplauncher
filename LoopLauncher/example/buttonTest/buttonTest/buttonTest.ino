#include <LiquidCrystal.h>
#include <Bounce2.h>

const int launchButtonCount = 6;
const int debounceInterval = 5;
int launchPins[launchButtonCount]= {2, 1, 0, 8, 7, 6};

//// define the lcd and pins
LiquidCrystal lcd(19, 18, 17, 16, 15, 14);
int lcdContrastPin = 22;

Bounce *launchButtons = new Bounce[launchButtonCount];

void setup() {
  Serial.begin(38400);
  
  pinMode(lcdContrastPin, OUTPUT);
  analogWrite(lcdContrastPin, 100);

  for(int i =0; i < launchButtonCount; i++) {
    Bounce b = Bounce();
    b.attach(launchPins[i], INPUT_PULLUP);
    b.interval(debounceInterval);
    launchButtons[i] = b;
  }
}

void loop() {

  for(int i = 0; i < launchButtonCount; i++) {
    launchButtons[i].update();
  }
  
  for(int i = 0; i < launchButtonCount; i++) {
    Bounce b = launchButtons[i];

    if(b.fell()) {
      serialButtonPress("fell", b.getPin(), i);
    }
    if(b.rose()) {
      serialButtonPress("rose", b.getPin(), i);
    }
  }
}

void serialButtonPress(String msg, int pin, int index) {
  Serial.print("button on pin ");
  Serial.print(pin);
  Serial.print(" and index ");
  Serial.print(index);
  Serial.print(" " + msg);
  Serial.print("\n");
}

