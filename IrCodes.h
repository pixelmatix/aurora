/*
 * Aurora: https://github.com/pixelmatix/aurora
 *
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from Craig Lindley's LightAppliance:
 * https://github.com/CraigLindley/LightAppliance
 * Copyright (c) 2014 Craig A. Lindley
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef IrCodes_H
#define IrCodes_H

extern IRrecv irReceiver;

enum class InputCommand {
  None,
  Update,
  Up,
  Down,
  Left,
  Right,
  Select,
  CycleBrightness,
  PlayMode,
  Palette,
  CycleClockAndMessageFiles,
  ShowClock,
  HideClockOrMessage,
  Power,
  Back,
  BrightnessUp,
  BrightnessDown,
  Menu,
  ShowCurrentMessage,
  AudioScaleUp,
  AudioScaleDown,
  ToggleSettingsMenuVisibility,
  ShowPatternName,
  FreezeDisplay,
};

bool sparkfunRemoteEnabled = true;
bool adafruitRemoteEnabled = true;
bool smartMatrixRemoteEnabled = true;
bool ronixSixButtonRemoteEnabled = true;
bool gameARemoteEnabled = true;
bool gameBRemoteEnabled = true;

// IR Raw Key Codes for SmartMatrix remote
#define IRCODE_SMARTMATRIX_HELD            0xFFFFFFFF // 4294967295
#define IRCODE_SMARTMATRIX_POWER           0x1689D02F // 378130479
#define IRCODE_SMARTMATRIX_BRIGHTNESS_DOWN 0x1689E01F // 378134559
#define IRCODE_SMARTMATRIX_BRIGHTNESS_UP   0x1689D827 // 378132519
#define IRCODE_SMARTMATRIX_PLAY            0x168900FF // 378077439
#define IRCODE_SMARTMATRIX_PALETTE         0x1689C03F // 378126399
#define IRCODE_SMARTMATRIX_OVERLAY         0x1689807F // 378110079
#define IRCODE_SMARTMATRIX_UP              0x1689609F // 378101919
#define IRCODE_SMARTMATRIX_LEFT            0x168910EF // 378081519
#define IRCODE_SMARTMATRIX_SELECT          0x168938C7 // 378091719
#define IRCODE_SMARTMATRIX_RIGHT           0x16899867 // 378116199
#define IRCODE_SMARTMATRIX_BACK            0x168918E7 // 378083559
#define IRCODE_SMARTMATRIX_DOWN            0x1689B847 // 378124359

// IR Raw Key Codes for SparkFun remote
#define IRCODE_SPARKFUN_HELD               0xFFFFFFFF // 4294967295
#define IRCODE_SPARKFUN_POWER              0x10EFD827 // 284153895
#define IRCODE_SPARKFUN_A                  0x10EFF807 // 284162055
#define IRCODE_SPARKFUN_B                  0x10EF7887 // 284129415
#define IRCODE_SPARKFUN_C                  0x10EF58A7 // 284121255
#define IRCODE_SPARKFUN_UP                 0x10EFA05F // 284139615
#define IRCODE_SPARKFUN_LEFT               0x10EF10EF // 284102895
#define IRCODE_SPARKFUN_SELECT             0x10EF20DF // 284106975
#define IRCODE_SPARKFUN_RIGHT              0x10EF807F // 284131455
#define IRCODE_SPARKFUN_DOWN               0x10EF00FF // 284098815

// IR Raw Key Codes for Adafruit remote
#define IRCODE_ADAFRUIT_HELD               0x7FFFFFFF // 2147483647
#define IRCODE_ADAFRUIT_VOLUME_UP          0x00FD40BF // 16597183
#define IRCODE_ADAFRUIT_PLAY_PAUSE         0x00FD807F // 16613503
#define IRCODE_ADAFRUIT_VOLUME_DOWN        0x00FD00FF // 16580863
#define IRCODE_ADAFRUIT_SETUP              0x00FD20DF // 16589023
#define IRCODE_ADAFRUIT_UP                 0x00FDA05F // 16621663
#define IRCODE_ADAFRUIT_STOP_MODE          0x00FD609F // 16605343
#define IRCODE_ADAFRUIT_LEFT               0x00FD10EF // 16584943
#define IRCODE_ADAFRUIT_ENTER_SAVE         0x00FD906F // 16617583
#define IRCODE_ADAFRUIT_RIGHT              0x00FD50AF // 16601263
#define IRCODE_ADAFRUIT_0_10_PLUS          0x00FD30CF // 16593103
#define IRCODE_ADAFRUIT_DOWN               0x00FDB04F // 16625743
#define IRCODE_ADAFRUIT_BACK               0x00FD708F // 16609423
#define IRCODE_ADAFRUIT_1                  0x00FD08F7 // 16582903
#define IRCODE_ADAFRUIT_2                  0x00FD8877 // 16615543
#define IRCODE_ADAFRUIT_3                  0x00FD48B7 // 16599223
#define IRCODE_ADAFRUIT_4                  0x00FD28D7 // 16591063
#define IRCODE_ADAFRUIT_5                  0x00FDA857 // 16623703
#define IRCODE_ADAFRUIT_6                  0x00FD6897 // 16607383
#define IRCODE_ADAFRUIT_7                  0x00FD18E7 // 16586983
#define IRCODE_ADAFRUIT_8                  0x00FD9867 // 16619623
#define IRCODE_ADAFRUIT_9                  0x00FD58A7 // 16603303

#define IRCODE_RONIX_SIX_BUTTON_HELD   4294967295
#define IRCODE_RONIX_SIX_BUTTON_M      16728255
#define IRCODE_RONIX_SIX_BUTTON_UP     16752735
#define IRCODE_RONIX_SIX_BUTTON_LEFT   16716015
#define IRCODE_RONIX_SIX_BUTTON_SELECT 16748655
#define IRCODE_RONIX_SIX_BUTTON_RIGHT  16732335
#define IRCODE_RONIX_SIX_BUTTON_DOWN   16756815

#define IRCODE_GAME_A_UP     2814785559
#define IRCODE_GAME_A_DOWN   3881522439
#define IRCODE_GAME_A_LEFT   3183975767
#define IRCODE_GAME_A_RIGHT  2718800199
#define IRCODE_GAME_A_A      2714386827
#define IRCODE_GAME_A_B      1156176007
#define IRCODE_GAME_A_TA     365726790
#define IRCODE_GAME_A_TB     1741811250
#define IRCODE_GAME_A_SELECT 3726419671
#define IRCODE_GAME_A_START  2229327047
#define IRCODE_GAME_A_RESET  4294967295

#define IRCODE_GAME_B_UP     3428550935
#define IRCODE_GAME_B_DOWN   200320519
#define IRCODE_GAME_B_LEFT   3797741143
#define IRCODE_GAME_B_RIGHT  3332565575
#define IRCODE_GAME_B_A      3328152203
#define IRCODE_GAME_B_B      1769941383
#define IRCODE_GAME_B_TA     979492166
#define IRCODE_GAME_B_TB     2355576626
#define IRCODE_GAME_B_SELECT 45217751
#define IRCODE_GAME_B_START  2843092423
#define IRCODE_GAME_B_RESET  4294967295

// Low level IR code reading function
// Function will return 0 if no IR code available
unsigned long decodeIRCode() {

  decode_results results;

  results.value = 0;

  // Attempt to read an IR code ?
  if (irReceiver.decode(&results)) {
    delay(20);

    // Prepare to receive the next IR code
    irReceiver.resume();
  }

  return results.value;
}

// Read an IR code
// Function will return 0 if no IR code available
unsigned long readIRCode() {

  // Is there an IR code to read ?
  unsigned long code = decodeIRCode();
  if (code == 0) {
    // No code so return 0
    return 0;
  }
  // Keep reading until code changes
  while (decodeIRCode() == code) {
    ;
  }
  // Serial.println(code);
  return code;
}

unsigned long lastIrCode = 0;

unsigned int holdStartTime = 0;
unsigned int defaultHoldDelay = 500;
bool isHolding = false;

unsigned int zeroStartTime = 0;
unsigned int zeroDelay = 120;

unsigned long readIRCode(unsigned int holdDelay) {
  // read the raw code from the sensor
  unsigned long irCode = readIRCode();

  //Serial.print(millis());
  //Serial.print("\t");
  //Serial.println(irCode);

  // don't return a short click until we know it's not a long hold
  // we'll have to wait for holdDelay ms to pass before returning a non-zero IR code
  // then, after that delay, as long as the button is held, we can keep returning the code
  // every time until it's released

  // the ir remote only sends codes every 107 ms or so (avg 106.875, max 111, min 102),
  // so the ir sensor will return 0 even if a button is held
  // so we have to wait longer than that before returning a non-zero code
  // in order to detect that a button has been released and is no longer held

  // only reset after we've gotten 0 back for more than the ir remote send interval
  unsigned int zeroTime = 0;

  if (irCode == 0) {
    zeroTime = millis() - zeroStartTime;
    if (zeroTime >= zeroDelay && lastIrCode != 0) {
      //Serial.println(F("zero delay has elapsed, returning last ir code"));
      // the button has been released for longer than the zero delay
      // start over delays over and return the last code
      irCode = lastIrCode;
      lastIrCode = 0;
      return irCode;
    }

    return 0;
  }

  // reset the zero timer every time a non-zero code is read
  zeroStartTime = millis();

  unsigned int heldTime = 0;

  if (irCode == IRCODE_SPARKFUN_HELD || irCode == IRCODE_ADAFRUIT_HELD) {
    // has the hold delay passed?
    heldTime = millis() - holdStartTime;
    if (heldTime >= holdDelay) {
      isHolding = true;
      //Serial.println(F("hold delay has elapsed, returning last ir code"));
      return lastIrCode;
    }
    else if (holdStartTime == 0) {
      isHolding = false;
      holdStartTime = millis();
    }
  }
  else {
    // not zero, not IRCODE_SPARKFUN_HELD
    // store it for use later, until the hold and zero delays have elapsed
    holdStartTime = millis();
    isHolding = false;
    lastIrCode = irCode;
    return 0;
  }

  return 0;
}

void heldButtonHasBeenHandled() {
  lastIrCode = 0;
  isHolding = false;
  holdStartTime = 0;
}

unsigned long waitForIRCode() {

  unsigned long irCode = readIRCode();
  while ((irCode == 0) || (irCode == 0xFFFFFFFF)) {
    delay(200);
    irCode = readIRCode();
  }
  return irCode;
}

InputCommand getCommand(unsigned long input) {
  if (adafruitRemoteEnabled) {
    switch (input) {
      case IRCODE_ADAFRUIT_UP:
        return InputCommand::Up;

      case IRCODE_ADAFRUIT_DOWN:
        return InputCommand::Down;

      case IRCODE_ADAFRUIT_LEFT:
        return InputCommand::Left;

      case IRCODE_ADAFRUIT_RIGHT:
        return InputCommand::Right;

      case IRCODE_ADAFRUIT_ENTER_SAVE:
        return InputCommand::Select;

      case IRCODE_ADAFRUIT_STOP_MODE:
      case IRCODE_ADAFRUIT_1:
        return InputCommand::PlayMode;

      case IRCODE_ADAFRUIT_2:
        return InputCommand::Palette;

      case IRCODE_ADAFRUIT_3:
        return InputCommand::CycleClockAndMessageFiles;

      case IRCODE_ADAFRUIT_PLAY_PAUSE:
        return InputCommand::Power;

      case IRCODE_ADAFRUIT_BACK:
        return InputCommand::Back;

      case IRCODE_ADAFRUIT_VOLUME_UP:
        return InputCommand::BrightnessUp;

      case IRCODE_ADAFRUIT_VOLUME_DOWN:
        return InputCommand::BrightnessDown;

      case IRCODE_ADAFRUIT_SETUP:
        return InputCommand::Menu;

      case IRCODE_ADAFRUIT_5:
        return InputCommand::ToggleSettingsMenuVisibility;

      case IRCODE_ADAFRUIT_6:
        return InputCommand::AudioScaleUp;

      case IRCODE_ADAFRUIT_7:
        return InputCommand::FreezeDisplay;

      case IRCODE_ADAFRUIT_9:
        return InputCommand::AudioScaleDown;

      case IRCODE_ADAFRUIT_0_10_PLUS:
        return InputCommand::ShowPatternName;
    }
  }

  if (sparkfunRemoteEnabled) {
    switch (input) {
      case IRCODE_SPARKFUN_UP:
        return InputCommand::Up;

      case IRCODE_SPARKFUN_DOWN:
        return InputCommand::Down;

      case IRCODE_SPARKFUN_LEFT:
        return InputCommand::Left;

      case IRCODE_SPARKFUN_RIGHT:
        return InputCommand::Right;

      case IRCODE_SPARKFUN_SELECT:
        return InputCommand::Select;

      case IRCODE_SPARKFUN_POWER:
        return InputCommand::CycleBrightness;

      case IRCODE_SPARKFUN_A:
        return InputCommand::PlayMode;

      case IRCODE_SPARKFUN_B:
        return InputCommand::Palette;

      case IRCODE_SPARKFUN_C:
        return InputCommand::CycleClockAndMessageFiles;
    }
  }

  if (smartMatrixRemoteEnabled) {
    switch (input) {
      case IRCODE_SMARTMATRIX_UP:
        return InputCommand::Up;

      case IRCODE_SMARTMATRIX_DOWN:
        return InputCommand::Down;

      case IRCODE_SMARTMATRIX_LEFT:
        return InputCommand::Left;

      case IRCODE_SMARTMATRIX_RIGHT:
        return InputCommand::Right;

      case IRCODE_SMARTMATRIX_SELECT:
        return InputCommand::Select;

      case IRCODE_SMARTMATRIX_PLAY:
        return InputCommand::PlayMode;

      case IRCODE_SMARTMATRIX_PALETTE:
        return InputCommand::Palette;

      case IRCODE_SMARTMATRIX_OVERLAY:
        return InputCommand::CycleClockAndMessageFiles;

      case IRCODE_SMARTMATRIX_POWER:
        return InputCommand::Power;

      case IRCODE_SMARTMATRIX_BACK:
        return InputCommand::Back;

      case IRCODE_SMARTMATRIX_BRIGHTNESS_UP:
        return InputCommand::BrightnessUp;

      case IRCODE_SMARTMATRIX_BRIGHTNESS_DOWN:
        return InputCommand::BrightnessDown;
    }
  }

  if (ronixSixButtonRemoteEnabled) {
    switch (input) {
      case IRCODE_RONIX_SIX_BUTTON_UP:
        return InputCommand::Up;

      case IRCODE_RONIX_SIX_BUTTON_DOWN:
        return InputCommand::Down;

      case IRCODE_RONIX_SIX_BUTTON_LEFT:
        return InputCommand::Left;

      case IRCODE_RONIX_SIX_BUTTON_RIGHT:
        return InputCommand::Right;

      case IRCODE_RONIX_SIX_BUTTON_SELECT:
        return InputCommand::Select;

      case IRCODE_RONIX_SIX_BUTTON_M:
        return InputCommand::CycleBrightness;
    }
  }

  if (gameARemoteEnabled) {
    switch (input) {
      case IRCODE_GAME_A_UP:
        return InputCommand::Up;

      case IRCODE_GAME_A_DOWN:
        return InputCommand::Down;

      case IRCODE_GAME_A_LEFT:
        return InputCommand::Left;

      case IRCODE_GAME_A_RIGHT:
        return InputCommand::Right;

      case IRCODE_GAME_A_SELECT:
        return InputCommand::Back;

      case IRCODE_GAME_A_START:
        return InputCommand::Select;

      case IRCODE_GAME_A_A:
        return InputCommand::PlayMode;

      case IRCODE_GAME_A_B:
        return InputCommand::Palette;

      case IRCODE_GAME_A_TA:
        return InputCommand::CycleClockAndMessageFiles;
    }
  }

  return InputCommand::None;
}

InputCommand getCommand(String input) {
  if (input == "Up")
    return InputCommand::Up;
  else if (input == "Down")
    return InputCommand::Down;
  else if (input == "Left")
    return InputCommand::Left;
  else if (input == "Right")
    return InputCommand::Right;
  else if (input == "Select")
    return InputCommand::Select;
  else if (input == "CycleBrightness")
    return InputCommand::CycleBrightness;
  else if (input == "PlayMode")
    return InputCommand::PlayMode;
  else if (input == "Palette")
    return InputCommand::Palette;
  else if (input == "CycleClockAndMessageFiles")
    return InputCommand::CycleClockAndMessageFiles;
  else if (input == "Power")
    return InputCommand::Power;
  else if (input == "Back")
    return InputCommand::Back;
  else if (input == "BrightnessUp")
    return InputCommand::BrightnessUp;
  else if (input == "BrightnessDown")
    return InputCommand::BrightnessDown;
  else if (input == "Menu")
    return InputCommand::Menu;
  else if (input == "ShowCurrentMessage")
    return InputCommand::ShowCurrentMessage;
  else if (input == "ShowClock")
    return InputCommand::ShowClock;
  else if (input == "HideClockOrMessage")
    return InputCommand::HideClockOrMessage;

  return InputCommand::None;
}

InputCommand readSerialCommand() {
  if (Serial.available() < 1)
    return InputCommand::None;

  if (Serial.peek() != '{')
    return InputCommand::None;

  aJsonStream stream(&Serial);

  //Serial.print(F("Parsing json..."));
  aJsonObject* root = aJson.parse(&stream);
  if (!root) {
    //Serial.println(F(" failed"));
    return InputCommand::None;
  }
  //Serial.println(F(" done"));

  // message
  aJsonObject* item = aJson.getObjectItem(root, "message");
  if (item && messagePlayer.readJsonObject(item)) {
    return InputCommand::ShowCurrentMessage;
  }

  // brightness
  item = aJson.getObjectItem(root, "brightness");
  if (item && item->type == aJson_Int) {
    brightness = item->valueint;
    boundBrightness();
    matrix.setBrightness(brightness);
    saveBrightnessSetting();
    return InputCommand::None;
  }

  // background brightness
  item = aJson.getObjectItem(root, "backgroundBrightness");
  if (item && item->type == aJson_Int) {
    backgroundBrightness = item->valueint;
    boundBackgroundBrightness();
    matrix.setBackgroundBrightness(backgroundBrightness);
    saveBackgroundBrightnessSetting();
    return InputCommand::None;
  }

  // palette
  item = aJson.getObjectItem(root, "palette");
  if (item && item->type == aJson_String) {
    effects.setPalette(item->valuestring);
    return InputCommand::None;
  }

  // pattern
  item = aJson.getObjectItem(root, "pattern");
  if (item && item->type == aJson_String) {
    //Serial.print(F("Loading pattern "));
    //Serial.println(item->valuestring);
    if (setPattern(item->valuestring))
      return InputCommand::Update;
    else
      return InputCommand::None;
  }
  else if (item && item->type == aJson_Int) {
    //Serial.print(F("Loading pattern "));
    //Serial.println(item->valueint);
    if (setPattern(item->valueint))
      return InputCommand::Update;
    else
      return InputCommand::None;
  }

  // animation
  item = aJson.getObjectItem(root, "animation");
  if (item && item->type == aJson_String) {
    //Serial.print(F("Loading animation "));
    //Serial.println(item->valuestring);
    if (setAnimation(item->valuestring))
      return InputCommand::Update;
    else
      return InputCommand::None;
  }
  else if (item && item->type == aJson_Int) {
    //Serial.print(F("Loading animation "));
    //Serial.println(item->valueint);
    if (setAnimation(item->valueint))
      return InputCommand::Update;
    else
      return InputCommand::None;
  }

  // add support for more specialized items here...

  // fall back on basic command support
  item = aJson.getObjectItem(root, "command");
  if (item && item->type == aJson_String) {
    // custom commands
    if ((String) item->valuestring == "ListAnimations") {
      listAnimations();
      return InputCommand::None;
    }
    else if ((String) item->valuestring == "ListPatterns") {
      listPatterns();
      return InputCommand::None;
    }
    else if ((String) item->valuestring == "ListPalettes") {
      effects.listPalettes();
      return InputCommand::None;
    }

    InputCommand command = getCommand(item->valuestring);
    if (command != InputCommand::None)
      return command;
  }

  return InputCommand::None;
}

InputCommand readCommand() {
  InputCommand command = getCommand(readIRCode());

  if (command == InputCommand::None)
    command = readSerialCommand();

  return command;
}

InputCommand readCommand(unsigned int holdDelay) {
  InputCommand command = getCommand(readIRCode(holdDelay));

  if (command == InputCommand::None)
    command = readSerialCommand();

  return command;
}

#endif
