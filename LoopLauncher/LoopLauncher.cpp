#include "Arduino.h"
#include "LoopLauncher.h"


LoopLauncher::LoopLauncher(
  int controlStopPin,
  int controlPreviousPin,
  int controlNextPin,
  int launchPins[], int launchButtonCount,
  LiquidCrystal *lcd)
{
  _controlStopPin = controlStopPin;
  _controlPreviousPin = controlPreviousPin;
  _controlNextPin = controlNextPin;
  _launchButtonCount = launchButtonCount;
  _lcd = lcd;

  int *_launchPins = new int[launchButtonCount];
  for(int i = 0; i < launchButtonCount; i++) {
    _launchPins[i] = launchPins[i];
    Bounce b = Bounce();
    this->setupButton(&b, launchPins[i]);
    _launchButtons[i] = b;
  }

  this->_stopButton = Bounce();
  this->setupButton(&_stopButton, controlStopPin);

  this->_previousButton = Bounce();
  this->setupButton(&_previousButton, controlPreviousPin);

  this->_nextButton = Bounce();
  this->setupButton(&_nextButton, controlNextPin);
};

void LoopLauncher::update() {

  this->updateButtons();

  this->handleLaunchButtons();

  this->handleStopButton();
  this->handlePreviousButton();
  this->handleNextButton();

  this->updateDisplay();
}

void LoopLauncher::updateButtons()
{
  for(int i = 0; i < _launchButtonCount; i++) {
    _launchButtons[i].update();
  }

  _stopButton.update();
  _previousButton.update();
  _nextButton.update();
}


// handles shuffling the currentBank forward or backward
void LoopLauncher::shiftCurrentBank(bool forward) {
  const int maxBanks = 20;

  if(forward) {
    if(_currentBank == maxBanks-1) // 0 indexed bank numbers
      _currentBank = 0;
     else
      _currentBank += 1;
  }
  else {
    if(_currentBank == 0)
      _currentBank = maxBanks-1;
     else
      _currentBank -= 1;
  }
}

void LoopLauncher::shiftCurrentOctave(bool forward) {
   const int maxOctave = 10;

   if(forward) {
    if(_currentOctave == maxOctave) // 0 indexed bank numbers
      _currentOctave = 0;
     else
      _currentOctave += 1;
  }
  else {
    if(_currentOctave == 0)
      _currentOctave = maxOctave;
     else
      _currentOctave -= 1;
  }
}


int LoopLauncher::findLaunchNoteByIndex(int index) {
  return (_currentBank * _launchButtonCount) + index + LaunchButtonStartingNote;
}

char LoopLauncher::findTriggerLabelByIndex(int index) {
  switch(index) {
    case 0:
      return 'A';
    case 1:
      return 'B';
    case 2:
      return 'C';
    case 3:
      return 'D';
    case 4:
      return 'E';
    case 5:
      return 'F';
    default:
      return '\0';
  }
}

void LoopLauncher::midiOn(int note) {
  usbMIDI.sendNoteOn(note, _midiVelocity, _midiChannel);
}

void LoopLauncher::midiOff(int note) {
  usbMIDI.sendNoteOff(note, _midiVelocity, _midiChannel);
}

void LoopLauncher::handleLaunchButtons(){
  for(int i = 0; i < _launchButtonCount; i++) {
      this->handleLaunchButton(&_launchButtons[i], i);
    }
}

void LoopLauncher::handleLaunchButton(Bounce *b, int index){
  if(b->fell()) {
    this->midiOn(this->findLaunchNoteByIndex(index));
    _currentTrigger = index;
  }

  if(b->rose()) {
    this->midiOff(this->findLaunchNoteByIndex(index));
  }
}

void LoopLauncher::handleStopButton() {
  if(_stopButton.fell()) {
    this->midiOn(StopButtonNote);
  }

  if(_stopButton.rose()) {
    this->midiOff(StopButtonNote);
  }
}

void LoopLauncher::handlePreviousButton() {
  if(_previousButton.fell()) {
    this->shiftCurrentBank(false);
  }

  if(_previousButton.rose()) {
  }
}

void LoopLauncher::handleNextButton() {
  if(_nextButton.fell()) {
   this->shiftCurrentBank(true);
  }

  if(_nextButton.rose()) {
  }
}

void LoopLauncher::setupButton(Bounce *b, int pin) {
  b->attach(pin);
  b->interval(DebounceInterval);
}

void LoopLauncher::updateDisplay() {
  _lcd->print(_currentBank + this->findTriggerLabelByIndex(_currentTrigger));
}
