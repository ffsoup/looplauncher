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
    this->SetupButton(&b, launchPins[i]);
    _launchButtons[i] = b;
  }

  this->_stopButton = Bounce();
  this->SetupButton(&_stopButton, controlStopPin);

  this->_previousButton = Bounce();
  this->SetupButton(&_previousButton, controlPreviousPin);

  this->_nextButton = Bounce();
  this->SetupButton(&_nextButton, controlNextPin);
};

void LoopLauncher::Update() {

  this->UpdateButtons();

  this->HandleLaunchButtons();

  this->HandleStopButton();
  this->HandlePreviousButton();
  this->HandleNextButton();
}

void LoopLauncher::UpdateButtons()
{
  for(int i = 0; i < _launchButtonCount; i++) {
    _launchButtons[i].update();
  }

  _stopButton.update();
  _previousButton.update();
  _nextButton.update();
}


// handles shuffling the currentBank forward or backward
void LoopLauncher::ShiftCurrentBank(bool forward) {
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

void LoopLauncher::ShiftCurrentOctave(bool forward) {
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


int LoopLauncher::FindLaunchNoteByIndex(int index) {
  return (_currentBank * _launchButtonCount) + index + LaunchButtonStartingNote;
}

void LoopLauncher::MidiOn(int note) {
  usbMIDI.sendNoteOn(note, _midiVelocity, _midiChannel);
}

void LoopLauncher::MidiOff(int note) {
  usbMIDI.sendNoteOff(note, _midiVelocity, _midiChannel);
}

void LoopLauncher::HandleLaunchButtons(){
  for(int i = 0; i < _launchButtonCount; i++) {
      this->HandleLaunchButton(&_launchButtons[i], i);
    }
}

void LoopLauncher::HandleLaunchButton(Bounce *b, int index){
  if(b->fell()) {
    this->MidiOn(this->FindLaunchNoteByIndex(index));
  }

  if(b->rose()) {
    this->MidiOff(this->FindLaunchNoteByIndex(index));
  }
}

void LoopLauncher::HandleStopButton() {
  if(_stopButton.fell()) {
    this->MidiOn(StopButtonNote);
  }

  if(_stopButton.rose()) {
    this->MidiOff(StopButtonNote);
  }
}

void LoopLauncher::HandlePreviousButton() {
  if(_previousButton.fell()) {
    this->ShiftCurrentBank(false);
  }

  if(_previousButton.rose()) {
  }
}

void LoopLauncher::HandleNextButton() {
  if(_nextButton.fell()) {
   this->ShiftCurrentBank(true);
  }

  if(_nextButton.rose()) {
  }
}

void LoopLauncher::SetupButton(Bounce *b, int pin) {
  b->attach(pin);
  b->interval(DebounceInterval);
}
