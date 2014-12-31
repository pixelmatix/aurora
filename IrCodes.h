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
    switch (input) {
        case IRCODE_ADAFRUIT_UP:
        case IRCODE_SPARKFUN_UP:
            return InputCommand::Up;

        case IRCODE_ADAFRUIT_DOWN:
        case IRCODE_SPARKFUN_DOWN:
            return InputCommand::Down;

        case IRCODE_SPARKFUN_LEFT:
        case IRCODE_ADAFRUIT_LEFT:
            return InputCommand::Left;

        case IRCODE_SPARKFUN_RIGHT:
        case IRCODE_ADAFRUIT_RIGHT:
            return InputCommand::Right;

        case IRCODE_SPARKFUN_SELECT:
        case IRCODE_ADAFRUIT_ENTER_SAVE:
            return InputCommand::Select;

        case IRCODE_SPARKFUN_POWER:
            return InputCommand::Brightness;

        case IRCODE_SPARKFUN_A:
        case IRCODE_ADAFRUIT_STOP_MODE:
        case IRCODE_ADAFRUIT_1:
            return InputCommand::PlayMode;

        case IRCODE_SPARKFUN_B:
        case IRCODE_ADAFRUIT_2:
            return InputCommand::Palette;

        case IRCODE_SPARKFUN_C:
        case IRCODE_ADAFRUIT_3:
            return InputCommand::Clock;

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
