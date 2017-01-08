#ifndef LoopLauncher_h
#define LoopLauncher_h

#include "Arduino.h"
#include "Bounce2.h"
#include "LiquidCrystal.h"

enum LaunchMode { LaunchMode_Note, LaunchMode_PC, LaunchMode_Pad };

class LoopLauncher
{
public:
  LoopLauncher(int controlStopPin, int controlPreviousPin, int controlNextPin, int launchPins[], int launchButtonCount, LiquidCrystal *lcd);
  void Update();

private:
  int _controlStopPin;
  int _controlPreviousPin;
  int _controlNextPin;
  int _launchButtonCount;
  int _launchPins[];
  LiquidCrystal *_lcd;

  Bounce _launchButtons[];
  Bounce _stopButton;
  Bounce _previousButton;
  Bounce _nextButton;

  int _midiChannel = 0;
  int _midiVelocity = 100;

  int _currentOctave = 5;
  int _currentBank = 1;

  const int DebounceInterval = 5;
  const int LaunchButtonStartingNote = 1;
  const int StopButtonNote = 127;

  void UpdateButtons();

  void HandleLaunchButtons();
  void HandleLaunchButton(Bounce *b, int pin);
  void HandleStopButton();
  void HandlePreviousButton();
  void HandleNextButton();

  void ShiftCurrentBank(bool forward);
  void ShiftCurrentOctave(bool forward);

  int FindLaunchNoteByIndex(int index);

  void MidiOn(int note);
  void MidiOff(int note);

  void SetupButton(Bounce *b, int pin);
};

#endif
