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

// IR Raw Key Codes for SparkFun remote
#define IRCODE_POWER  0x10EFD827    
#define IRCODE_A      0x10EFF807
#define IRCODE_B      0x10EF7887
#define IRCODE_C      0x10EF58A7
#define IRCODE_UP     0x10EFA05F
#define IRCODE_LEFT   0x10EF10EF
#define IRCODE_SELECT 0x10EF20DF
#define IRCODE_RIGHT  0x10EF807F
#define IRCODE_DOWN   0x10EF00FF
#define IRCODE_HELD   0xFFFFFFFF

// IR Raw Key Codes for Adafruit remote
#define IRCODE_ADAFRUIT_HELD        4294967295
#define IRCODE_ADAFRUIT_VOLUME_UP   16597183
#define IRCODE_ADAFRUIT_PLAY_PAUSE  16613503
#define IRCODE_ADAFRUIT_VOLUME_DOWN 16580863
#define IRCODE_ADAFRUIT_SETUP       16589023
#define IRCODE_ADAFRUIT_UP          16621663
#define IRCODE_ADAFRUIT_STOP_MODE   16605343
#define IRCODE_ADAFRUIT_LEFT        16584943
#define IRCODE_ADAFRUIT_ENTER_SAVE  16617583
#define IRCODE_ADAFRUIT_RIGHT       16601263
#define IRCODE_ADAFRUIT_0_10_PLUS   16593103
#define IRCODE_ADAFRUIT_DOWN        16625743
#define IRCODE_ADAFRUIT_BACK        16609423
#define IRCODE_ADAFRUIT_1           16582903
#define IRCODE_ADAFRUIT_2           16615543
#define IRCODE_ADAFRUIT_3           16599223
#define IRCODE_ADAFRUIT_4           16591063
#define IRCODE_ADAFRUIT_5           16623703
#define IRCODE_ADAFRUIT_6           16607383
#define IRCODE_ADAFRUIT_7           16586983
#define IRCODE_ADAFRUIT_8           16619623
#define IRCODE_ADAFRUIT_9           16603303

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
unsigned int defaultHoldDelay = 250;
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

    if (irCode == IRCODE_HELD || irCode == IRCODE_ADAFRUIT_HELD) {
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
        // not zero, not IRCODE_HELD
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

InputCommand getCommand(unsigned long input) {
    switch (input) {
        case IRCODE_ADAFRUIT_UP:
        case IRCODE_UP:
            return InputCommand::Up;

        case IRCODE_ADAFRUIT_DOWN:
        case IRCODE_DOWN:
            return InputCommand::Down;

        case IRCODE_LEFT:
        case IRCODE_ADAFRUIT_LEFT:
            return InputCommand::Left;

        case IRCODE_RIGHT:
        case IRCODE_ADAFRUIT_RIGHT:
            return InputCommand::Right;

        case IRCODE_SELECT:
        case IRCODE_ADAFRUIT_ENTER_SAVE:
            return InputCommand::Select;

        case IRCODE_POWER:
            return InputCommand::Brightness;

        case IRCODE_A:
        case IRCODE_ADAFRUIT_STOP_MODE:
        case IRCODE_ADAFRUIT_1:
            return InputCommand::PlayMode;

        case IRCODE_B:
        case IRCODE_ADAFRUIT_2:
            return InputCommand::Palette;

        case IRCODE_C:
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
