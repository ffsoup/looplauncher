#include <Bounce2.h>
enum LaunchMode { LaunchMode_Note, LaunchMode_PC, LaunchMode_Pad };
//typedef enum LaunchMode LaunchMode;

const int launchButtonCount = 6; // how many buttons we'll use to launch
const int launchButtonStartingPin = 7; // start with pin 7
const int launchButtonStartingNote = 1; // start with C#-2
const int stopButtonNote = 127;
const int MidiVelocity = 100; // towards the hot end, 64 is middle
const int MidiChannel = 0; // we'll allow user control of this at runtime



int currentOctave = 5; // allow for middle C at midi note value 6 (5 octaves down/up)
int currentBank = 0;
LaunchMode currentMode = LaunchMode_Note; // 0=note, 1=control, 2=pad


Bounce launchButtons[launchButtonCount];
Bounce stopButton;
Bounce prevButton;
Bounce nextButton;

void setup() {

  const int debounceInterval = 5;

  // setup trigger buttons
  for(int i = 0; i < launchButtonCount; i++) {
    pinMode(launchButtonStartingPin + i, INPUT);
    Bounce b = Bounce();
    setupButton(&b, launchButtonStartingPin + i, debounceInterval); 
    launchButtons[i] = b;
  }  

  // setup function buttons
  const int stopButtonPin = 6;
  stopButton = Bounce();
  setupButton(&stopButton, stopButtonPin, debounceInterval);
  
  const int prevButtonPin = 5;
  prevButton = Bounce();
  setupButton(&prevButton, prevButtonPin, debounceInterval);
  
  const int nextButtonPin = 4;
  nextButton = Bounce();
  setupButton(&nextButton, nextButtonPin, debounceInterval);
}


void loop() {
  updateBounces();

  handleLaunchButtons();

  handleStopButton(&stopButton);
  handlePrevButton(&prevButton);
  handleNextButton(&nextButton);

}

void setupButton(Bounce *b, int pin, int debounceInterval) {
  b->attach(pin);
  b->interval(debounceInterval);
}

void updateBounces() {
  for(int i = 0; i < launchButtonCount; i++) {
    launchButtons[i].update();
  }

  stopButton.update();
  prevButton.update();
  nextButton.update();
}

// iterates the array of launch buttons and fires the handler for each 
void handleLaunchButtons() {
  for(int i = 0; i < launchButtonCount; i++) {
    handleLaunchButton(&launchButtons[i], i + launchButtonStartingPin);
  }
}

void handleLaunchButton(Bounce *b, int pin) {

  if(b->fell()) {
    midiOn(findLaunchNoteByPin(pin));
  }

  if(b->rose()) {
    midiOff(findLaunchNoteByPin(pin));
  }
  
}

void handleStopButton(Bounce *b) {
  
  if(b->fell()) {
    midiOn(stopButtonNote);
  }

  if(b->rose()) {
    midiOff(stopButtonNote);
  }
}


void handlePrevButton(Bounce *b) {

  if(b->fell()) {
    shiftCurrentBank(false);
  }

  if(b->rose()) {
  }
  
}

void handleNextButton(Bounce *b) {

   if(b->fell()) {
    shiftCurrentBank(true);
   }

   if(b->rose()) {
   }
}







void shiftCurrentBank(bool forward) {
  const int maxBanks = 20;
  
  if(forward) {
    if(currentBank == maxBanks-1) // 0 indexed bank numbers
      currentBank = 0;
     else
      currentBank += 1;
  }
  else {
    if(currentBank == 0)
      currentBank = maxBanks-1;
     else
      currentBank -= 1;
  }
}

void shiftCurrentOctave(bool forward) {
   const int maxOctave = 10;
   
   if(forward) {
    if(currentOctave == maxOctave) // 0 indexed bank numbers
      currentOctave = 0;
     else
      currentOctave += 1;
  }
  else {
    if(currentOctave == 0)
      currentOctave = maxOctave;
     else
      currentOctave -= 1;
  }
}

int findLaunchNoteByPin(int pin) {
  return (currentBank * launchButtonCount) + pin + launchButtonStartingNote;
}

void midiOn(int note) {
  usbMIDI.sendNoteOn(note, MidiVelocity, MidiChannel);
}

void midiOff(int note) {
  usbMIDI.sendNoteOff(note, MidiVelocity, MidiChannel);
}

