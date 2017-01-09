#ifndef LoopLauncher_h
#define LoopLauncher_h

#include "Arduino.h"
#include "Bounce2.h"
#include "LiquidCrystal.h"

enum LaunchMode { LaunchMode_Note, LaunchMode_PC, LaunchMode_Pad };

class LoopLauncher
{
public:
  LoopLauncher();
  void begin(int controlStopPin, int controlPreviousPin, int controlNextPin, int launchPins[], int launchButtonCount, LiquidCrystal *lcd);
  void update();

private:
  bool initialized = false;
  int _controlStopPin;
  int _controlPreviousPin;
  int _controlNextPin;
  int _launchButtonCount;
  int _launchPins[];
  LiquidCrystal *_lcd;

  Bounce *_launchButtons;
  Bounce _stopButton;
  Bounce _previousButton;
  Bounce _nextButton;

  int _midiChannel = 0;
  int _midiVelocity = 100;

  int _currentTrigger = -1;
  int _previousTrigger = -1;

  const int DebounceInterval = 5;
  const int LaunchButtonStartingNote = 1;
  const int StopButtonNote = 127;


  void updateDisplay(bool force);

  void handleLaunchButton(Bounce *b, int pin);
  void handleStopButton();
  void handlePreviousButton();
  void handleNextButton();

  void clearTrigger();
  bool isTriggerActive();

  int _currentBank = 0;
  int _previousBank = 0;
  void shiftCurrentBank(bool forward);

  // int _currentOctave = 5;
  // int _previousOctave = 5;
  // void shiftCurrentOctave(bool forward);

  int findLaunchNoteByIndex(int index);
  char findTriggerLabelByIndex(int index);

  void midiOn(int note);
  void midiOff(int note);

};

#endif
