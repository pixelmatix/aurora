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
    Up,
    Down,
    Left,
    Right,
    Select,
    Brightness,
    PlayMode,
    Palette,
    Clock,
    Power,
    Back,
    BrightnessUp,
    BrightnessDown,
    Menu,
};

// IR Raw Key Codes for SparkFun remote
#define IRCODE_SPARKFUN_POWER  0x10EFD827    
#define IRCODE_SPARKFUN_A      0x10EFF807
#define IRCODE_SPARKFUN_B      0x10EF7887
#define IRCODE_SPARKFUN_C      0x10EF58A7
#define IRCODE_SPARKFUN_UP     0x10EFA05F
#define IRCODE_SPARKFUN_LEFT   0x10EF10EF
#define IRCODE_SPARKFUN_SELECT 0x10EF20DF
#define IRCODE_SPARKFUN_RIGHT  0x10EF807F
#define IRCODE_SPARKFUN_DOWN   0x10EF00FF
#define IRCODE_SPARKFUN_HELD   0xFFFFFFFF

// IR Raw Key Codes for Adafruit remote
#define IRCODE_ADAFRUIT_HELD        0x7FFFFFFF // 4294967295
#define IRCODE_ADAFRUIT_VOLUME_UP   0x00FD40BF // 16597183
#define IRCODE_ADAFRUIT_PLAY_PAUSE  0x00FD807F // 16613503
#define IRCODE_ADAFRUIT_VOLUME_DOWN 0x00FD00FF // 16580863
#define IRCODE_ADAFRUIT_SETUP       0x00FD20DF // 16589023
#define IRCODE_ADAFRUIT_UP          0x00FDA05F // 16621663
#define IRCODE_ADAFRUIT_STOP_MODE   0x00FD609F // 16605343
#define IRCODE_ADAFRUIT_LEFT        0x00FD10EF // 16584943
#define IRCODE_ADAFRUIT_ENTER_SAVE  0x00FD906F // 16617583
#define IRCODE_ADAFRUIT_RIGHT       0x00FD50AF // 16601263
#define IRCODE_ADAFRUIT_0_10_PLUS   0x00FD30CF // 16593103
#define IRCODE_ADAFRUIT_DOWN        0x00FDB04F // 16625743
#define IRCODE_ADAFRUIT_BACK        0x00FD708F // 16609423
#define IRCODE_ADAFRUIT_1           0x00FD08F7 // 16582903
#define IRCODE_ADAFRUIT_2           0x00FD8877 // 16615543
#define IRCODE_ADAFRUIT_3           0x00FD48B7 // 16599223
#define IRCODE_ADAFRUIT_4           0x00FD28D7 // 16591063
#define IRCODE_ADAFRUIT_5           0x00FDA857 // 16623703
#define IRCODE_ADAFRUIT_6           0x00FD6897 // 16607383
#define IRCODE_ADAFRUIT_7           0x00FD18E7 // 16586983
#define IRCODE_ADAFRUIT_8           0x00FD9867 // 16619623
#define IRCODE_ADAFRUIT_9           0x00FD58A7 // 16603303

// IR Raw Key Codes for WRT8621 remote
#define IRCODE_WRT8621_HELD         0x7FFFFFFF // 4294967295
#define IRCODE_WRT8621_OFF          0x00FFB04F // 16756815
#define IRCODE_WRT8621_MODE         0x00FF708F // 16740495
#define IRCODE_WRT8621_ON           0x00FFF00F // 16773135
#define IRCODE_WRT8621_UP1          0x00FF906F // 16748655
#define IRCODE_WRT8621_IN           0x00FF50AF // 16732335
#define IRCODE_WRT8621_DOWN1        0x00FFD02F // 16764975
#define IRCODE_WRT8621_UP2          0x00FF8877 // 16746615
#define IRCODE_WRT8621_OUT          0x00FF48B7 // 16730295
#define IRCODE_WRT8621_DOWN2        0x00FFC837 // 16762935
#define IRCODE_WRT8621_UP3          0x00FFA857 // 16754775
#define IRCODE_WRT8621_ATT          0x00FF6897 // 16738455
#define IRCODE_WRT8621_DOWN3        0x00FFE817 // 16771095
#define IRCODE_WRT8621_UP4          0x00FF9867 // 16750695
#define IRCODE_WRT8621_QA           0x00FF58A7 // 16734375
#define IRCODE_WRT8621_DOWN4        0x00FFD827 // 16767015
#define IRCODE_WRT8621_V            0x00FFB847 // 16758855
#define IRCODE_WRT8621_T            0x00FFF807 // 16775175

// IR Raw Key Codes for WRT8621-05A remote
#define IRCODE_WRT8621_05A_HELD     0x7FFFFFFF // 4294967295
#define IRCODE_WRT8621_05A_M        0x00FF40BF // 16728255
#define IRCODE_WRT8621_05A_UP       0x00FFA05F // 16752735
#define IRCODE_WRT8621_05A_LEFT     0x00FF10EF // 16716015
#define IRCODE_WRT8621_05A_OK       0x00FF906F // 16748655
#define IRCODE_WRT8621_05A_RIGHT    0x00FF50AF // 16732335
#define IRCODE_WRT8621_05A_DOWN     0x00FFB04F // 16756815

// IR Raw Key Codes for WRT8521-01 remote
#define IRCODE_WRT8521_01_HELD        0x7FFFFFFF // 4294967295
#define IRCODE_WRT8521_01_VOLUME_UP   0x00FFE21D // 16769565
#define IRCODE_WRT8521_01_PLAY_PAUSE  0x00FF629D // 16736925
#define IRCODE_WRT8521_01_VOLUME_DOWN 0x00FFA25D // 16753245
#define IRCODE_WRT8521_01_SETUP       0x00FF22DD // 16720605
#define IRCODE_WRT8521_01_UP          0x00FF02FD // 16712445
#define IRCODE_WRT8521_01_STOP_MODE   0x00FFC23D // 16761405
#define IRCODE_WRT8521_01_LEFT        0x00FFE01F // 16769055
#define IRCODE_WRT8521_01_ENTER_SAVE  0x00FFA857 // 16754775
#define IRCODE_WRT8521_01_RIGHT       0x00FF906F // 16748655
#define IRCODE_WRT8521_01_0_10_PLUS   0x00FF6897 // 16738455
#define IRCODE_WRT8521_01_DOWN        0x00FF9867 // 16750695
#define IRCODE_WRT8521_01_BACK        0x00FFB04F // 16756815     
#define IRCODE_WRT8521_01_1           0x00FF30CF // 16724175
#define IRCODE_WRT8521_01_2           0x00FF18E7 // 16718055
#define IRCODE_WRT8521_01_3           0x00FF7A85 // 16743045
#define IRCODE_WRT8521_01_4           0x00FF10EF // 16716015
#define IRCODE_WRT8521_01_5           0x00FF38C7 // 16726215
#define IRCODE_WRT8521_01_6           0x00FF5AA5 // 16734885
#define IRCODE_WRT8521_01_7           0x00FF42BD // 16728765
#define IRCODE_WRT8521_01_8           0x00FF4AB5 // 16730805
#define IRCODE_WRT8521_01_9           0x00FF52AD // 16732845

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
    Serial.println(code);
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
    switch (input) {
        case IRCODE_ADAFRUIT_UP:
        case IRCODE_WRT8521_01_UP:
        case IRCODE_SPARKFUN_UP:
        case IRCODE_WRT8621_IN:
        case IRCODE_WRT8621_05A_UP:
            return InputCommand::Up;

        case IRCODE_ADAFRUIT_DOWN:
        case IRCODE_WRT8521_01_DOWN:
        case IRCODE_SPARKFUN_DOWN:
        case IRCODE_WRT8621_ATT:
        //case IRCODE_WRT8621_05A_DOWN:
            return InputCommand::Down;

        case IRCODE_SPARKFUN_LEFT:
        case IRCODE_ADAFRUIT_LEFT:
        case IRCODE_WRT8521_01_LEFT:
        case IRCODE_WRT8621_UP2:
        case IRCODE_WRT8621_05A_LEFT:
            return InputCommand::Left;

        case IRCODE_SPARKFUN_RIGHT:
        case IRCODE_ADAFRUIT_RIGHT:
        //case IRCODE_WRT8521_01_RIGHT:
        case IRCODE_WRT8621_DOWN2:
        //case IRCODE_WRT8621_05A_RIGHT:
            return InputCommand::Right;

        case IRCODE_SPARKFUN_SELECT:
        case IRCODE_ADAFRUIT_ENTER_SAVE:
        case IRCODE_WRT8521_01_ENTER_SAVE:
        case IRCODE_WRT8621_OUT:
        //case IRCODE_WRT8621_05A_OK:
            return InputCommand::Select;

        case IRCODE_SPARKFUN_POWER:
        case IRCODE_WRT8621_05A_M:
            return InputCommand::Brightness;

        case IRCODE_SPARKFUN_A:
        case IRCODE_ADAFRUIT_STOP_MODE:
        case IRCODE_ADAFRUIT_1:
        case IRCODE_WRT8521_01_STOP_MODE:
        case IRCODE_WRT8521_01_1:
        case IRCODE_WRT8621_DOWN1:
            return InputCommand::PlayMode;

        case IRCODE_SPARKFUN_B:
        case IRCODE_ADAFRUIT_2:
        case IRCODE_WRT8521_01_2:
        case IRCODE_WRT8621_QA:
            return InputCommand::Palette;

        case IRCODE_SPARKFUN_C:
        case IRCODE_ADAFRUIT_3:
        case IRCODE_WRT8521_01_3:
        case IRCODE_WRT8621_DOWN4:
            return InputCommand::Clock;

        case IRCODE_ADAFRUIT_PLAY_PAUSE:
        case IRCODE_WRT8521_01_PLAY_PAUSE:
        case IRCODE_WRT8621_MODE:
            return InputCommand::Power;

        case IRCODE_ADAFRUIT_BACK:
        //case IRCODE_WRT8521_01_BACK:
        case IRCODE_WRT8621_DOWN3:
            return InputCommand::Back;

        case IRCODE_ADAFRUIT_VOLUME_UP:
        case IRCODE_WRT8521_01_VOLUME_UP:
        case IRCODE_WRT8621_ON:
            return InputCommand::BrightnessUp;

        case IRCODE_ADAFRUIT_VOLUME_DOWN:
        case IRCODE_WRT8521_01_VOLUME_DOWN:
        case IRCODE_WRT8621_OFF:
            return InputCommand::BrightnessDown;

        case IRCODE_ADAFRUIT_SETUP:
        case IRCODE_WRT8521_01_SETUP:
        case IRCODE_WRT8621_UP1:
            return InputCommand::Menu;
    }

    return InputCommand::None;
}

InputCommand readCommand() {
    return getCommand(readIRCode());
}

InputCommand readCommand(unsigned int holdDelay) {
    return getCommand(readIRCode(holdDelay));
}

#endif
