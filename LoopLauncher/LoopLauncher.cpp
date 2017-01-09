#include "Arduino.h"
#include "LoopLauncher.h"

LoopLauncher::LoopLauncher(){};


void LoopLauncher::begin(
  int controlStopPin,
  int controlPreviousPin,
  int controlNextPin,
  int launchPins[], int launchButtonCount,
  LiquidCrystal *lcd)
{
  if(!initialized) {

    _controlStopPin = controlStopPin;
    _controlPreviousPin = controlPreviousPin;
    _controlNextPin = controlNextPin;
    _launchButtonCount = launchButtonCount;
    _lcd = lcd;

    _lcd->begin(16,2);

    _launchButtons = new Bounce[launchButtonCount];
    int *_launchPins = new int[launchButtonCount];
    for(int i = 0; i < launchButtonCount; i++) {
      int p = launchPins[i];
      _launchPins[i] = p;
      Bounce b = Bounce();
      pinMode(p, INPUT_PULLUP);
      b.attach(p);
      b.interval(DebounceInterval);
      _launchButtons[i] = b;
    }

    _stopButton = Bounce();
    _stopButton.attach(controlStopPin, INPUT_PULLUP);
    _stopButton.interval(DebounceInterval);

    _previousButton = Bounce();
    _previousButton.attach(controlPreviousPin, INPUT_PULLUP);
    _previousButton.interval(DebounceInterval);

    _nextButton = Bounce();
    _nextButton.attach(controlNextPin, INPUT_PULLUP);
    _nextButton.interval(DebounceInterval);

    this->updateDisplay(true);

    initialized = true;

  }
};

void LoopLauncher::update() {
  if(initialized) {
    for(int i = 0; i < _launchButtonCount; i++) {
      Bounce *b = &_launchButtons[i];
      if(b->update()) {
        this->handleLaunchButton(b, i);
      }
    }

    if(_stopButton.update()) {
      handleStopButton();
    }

    if(_previousButton.update()) {
      handlePreviousButton();
    }

    if(_nextButton.update()) {
      handleNextButton();
    }

    this->updateDisplay(false);
  }
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

// return the note to be played according to currentBank and passed trigger
int LoopLauncher::findLaunchNoteByIndex(int index) {
  if(index == 127) {
    return 127;
  }
  else {
    return (_currentBank * _launchButtonCount) + index + LaunchButtonStartingNote;
  }
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
    _currentTrigger =127;
    this->midiOn(StopButtonNote);
  }
  if(_stopButton.rose()) {
    _currentTrigger =127;
    this->midiOff(StopButtonNote);
  }
}

void LoopLauncher::handlePreviousButton() {
  if(_previousButton.fell()) {
    this->clearTrigger();
    this->shiftCurrentBank(false);
  }
  if(_previousButton.rose()) {
  }
}

void LoopLauncher::handleNextButton() {
  if(_nextButton.fell()) {
    this->clearTrigger();
    this->shiftCurrentBank(true);
  }
  if(_nextButton.rose()) {
  }
}

void LoopLauncher::clearTrigger() {
  _currentTrigger = -1;
}
bool LoopLauncher::isTriggerActive() {
  return _currentTrigger != -1;
}

void LoopLauncher::updateDisplay(bool force) {
  if(force || _currentTrigger != _previousTrigger
    || _currentBank != _previousBank) {

      char buffer[16];
      if(this->isTriggerActive()) {
        sprintf(buffer, "Bank:%02d Patch:%c", _currentBank+1, this->findTriggerLabelByIndex(_currentTrigger));
      }
      else {
        sprintf(buffer, "Bank:%02d Patch:NA", _currentBank+1);
      }
      _lcd->clear();
      _lcd->print(buffer);

      _lcd->setCursor(0,1);
      _lcd->print("Note:");
      if(this->isTriggerActive()) {
        _lcd->print(this->findLaunchNoteByIndex(_currentTrigger));
      }
      else {
        _lcd->print("NA");
      }

      _previousTrigger = _currentTrigger;
      _previousBank = _currentBank;
  }
}

// void LoopLauncher::shiftCurrentOctave(bool forward) {
//    const int maxOctave = 10;
//
//    if(forward) {
//     if(_currentOctave == maxOctave) // 0 indexed bank numbers
//       _currentOctave = 0;
//     else
//       _currentOctave += 1;
//   }
//   else {
//     if(_currentOctave == 0)
//       _currentOctave = maxOctave;
//     else
//       _currentOctave -= 1;
//   }
// }
