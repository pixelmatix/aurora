/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Designed in collaboration with Pixelmatix using the SmartMatrix Library: http://pixelmatix.com
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

#include "Hardware.h"
#include "Aurora.h"
uint8_t brightness = 255;
uint8_t backgroundBrightness = 63;

const uint8_t brightnessCount = 5;
uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
uint8_t backgroundBrightnessMap[brightnessCount] = { 8, 16, 32, 64, 128 };

#include <SmartMatrix_32x32.h>
#include <FastLED.h>
#include <IRremote.h>
#include <SPI.h>
#include <SD.h>

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

bool sdAvailable = false;
SmartMatrix matrix;
IRrecv irReceiver(IR_RECV_PIN);

#include "aJSON.h"

#include "IrCodes.h"

#include "Effects.h"
Effects effects;

#include "GifPlayer.h"
GifPlayer gifPlayer;

#include "BitmapPlayer.h"
BitmapPlayer bitmapPlayer;

#include "MessagePlayer.h"
MessagePlayer messagePlayer;

#include "Logo.h"

#include "Drawable.h"
#include "Playlist.h"
#include "Runnable.h"

tmElements_t time;
bool isTimeAvailable = false;

#include "ClockDigitalShort.h"
ClockDigitalShort clockDigitalShort;

#include "ClockDisplay.h"
ClockDisplay clockDisplay;

#include "Patterns.h"
Patterns patterns;

#include "Animations.h"
Animations animations;

#include "Bitmaps.h"

#include "MenuItem.h"
#include "Menu.h"
Menu menu;

#include "Settings.h"
Settings settings;

#include "SettingsSetTime.h"
#include "SettingsMoveClock.h"

MenuItem menuItemPatterns = MenuItem("Patterns", &patterns);
MenuItem menuItemAnimations = MenuItem("Animations", &animations);
MenuItem menuItemSettings = MenuItem("Settings", &settings);

// Main Menu
MenuItem* mainMenuItems [] = {
    &menuItemPatterns,
    &menuItemAnimations,
    &menuItemSettings,
};

int mainMenuItemCount;

bool enableStartupSplash = false;

void setup()
{
    // Setup serial interface
    Serial.begin(9600);

    //delay(3000);
    //Serial.println(F("starting..."));

    // Initialize the IR receiver
    irReceiver.enableIRIn();

    // Initialize 32x32 LED Matrix
    matrix.begin();
    matrix.setBrightness(brightness);
    matrix.setColorCorrection(cc24);
    matrix.setFont(gohufont11b);
    matrix.setScrollStartOffsetFromLeft(8);
    matrix.setScrollOffsetFromTop(25);
    matrix.setScrollSpeed(80);
    matrix.setScrollMode(wrapForward);

    if (enableStartupSplash) {
        drawLogo();
        matrix.scrollText("Aurora by Pixelmatix", 1);
    }
    else {
        matrix.fillScreen(rgb24{ 0, 0, 0 });
    }

    matrix.swapBuffers();

    pinMode(SD_CARD_CS, OUTPUT);
    sdAvailable = SD.begin(SD_CARD_CS);
    if (sdAvailable) {
        animations.setup("/gifs/");
        messagePlayer.setup("/messages/");
    }

    // setup the effects generator
    effects.CyclePalette();

    mainMenuItemCount = sizeof(mainMenuItems) / sizeof(MenuItem*);

    // initialize realtime clock
    // switch pins to use 16/17 for I2C instead of 18/19
    pinMode(18, INPUT);
    pinMode(19, INPUT);
    CORE_PIN16_CONFIG = (PORT_PCR_MUX(2) | PORT_PCR_PE | PORT_PCR_PS);
    CORE_PIN17_CONFIG = (PORT_PCR_MUX(2) | PORT_PCR_PE | PORT_PCR_PS);

    clockDisplay.readTime();

    if (sdAvailable)
        loadSettings();

    if (enableStartupSplash) {
        while (matrix.getScrollStatus() != 0) {}
    }
}

void loop()
{
    menu.run(mainMenuItems, mainMenuItemCount);
}

void powerOff()
{
    // clear the display
    matrix.clearForeground();
    matrix.scrollText("", 1);
    matrix.fillScreen(CRGB(CRGB::Black));
    matrix.swapBuffers();
    matrix.clearForeground();
    matrix.displayForegroundDrawing(false);

    while (true) {
        InputCommand command = readCommand();
        if (command == InputCommand::Power ||
            command == InputCommand::Brightness)
            return;

        // go idle for a while, converve power
        delay(500);
    }
}

void loadSettings() {
    brightness = loadIntSetting("/aurora/", "/aurora/brghtnss.txt", 3, 255);
    boundBrightness();
    matrix.setBrightness(brightness);

    backgroundBrightness = loadIntSetting("/aurora/", "/aurora/bckbrght.txt", 3, 63);
    boundBackgroundBrightness();
    matrix.setBackgroundBrightness(backgroundBrightness);

    clockDisplay.loadSettings();
}

int getBrightnessLevel() {
    int level = 0;
    for (int i = 0; i < brightnessCount; i++) {
        if (brightnessMap[i] >= brightness) {
            level = i;
            break;
        }
    }
    return level;
}

int getBackgroundBrightnessLevel() {
    int level = 0;
    for (int i = 0; i < brightnessCount; i++) {
        if (backgroundBrightnessMap[i] >= backgroundBrightness) {
            level = i;
            break;
        }
    }
    return level;
}

void adjustBrightness(int delta) {
    int level = getBrightnessLevel();

    level += delta;
    if (level < 0)
        level = brightnessCount - 1;
    if (level >= brightnessCount)
        level = 0;

    brightness = brightnessMap[level];
    boundBrightness();
    matrix.setBrightness(brightness);
    saveBrightnessSetting();
}

uint8_t cycleBrightness() {
    adjustBrightness(1);

    if (brightness == brightnessMap[0])
        return 0;

    return brightness;
}

void adjustBackgroundBrightness(int d) {
    int level = 0;
    for (int i = 0; i < brightnessCount; i++) {
        if (backgroundBrightnessMap[i] >= backgroundBrightness) {
            level = i;
            break;
        }
    }

    level += d;
    if (level < 0)
        level = brightnessCount - 1;
    if (level >= brightnessCount)
        level = 0;

    backgroundBrightness = backgroundBrightnessMap[level];
    boundBackgroundBrightness();
    matrix.setBackgroundBrightness(backgroundBrightness);
    saveBackgroundBrightnessSetting();
}

void boundBrightness() {
    if (brightness < brightnessMap[0])
        brightness = brightnessMap[0];
    else if (brightness > brightnessMap[brightnessCount - 1])
        brightness = brightnessMap[brightnessCount - 1];
}

void boundBackgroundBrightness() {
    if (backgroundBrightness < backgroundBrightnessMap[0])
        backgroundBrightness = backgroundBrightnessMap[0];
    else if (backgroundBrightness > backgroundBrightnessMap[brightnessCount - 1])
        backgroundBrightness = backgroundBrightnessMap[brightnessCount - 1];
}

void saveBrightnessSetting() {
    saveIntSetting("/aurora/", "/aurora/brghtnss.txt", brightness);
}

void saveBackgroundBrightnessSetting() {
    saveIntSetting("/aurora/", "/aurora/bckbrght.txt", backgroundBrightness);
}

int loadIntSetting(char* dir, const char* settingPath, int maxLength, int defaultValue) {
    if (!sdAvailable)
        return defaultValue;

    int intValue = defaultValue;

    if (!SD.exists(dir)) {
        //Serial.println(F("aurora dir doesn't exist"));
        SD.mkdir(dir);
    }
    //Serial.print(dir);
    //Serial.print(" exists: ");
    //Serial.println(sd.exists(dir));

    File file = SD.open(settingPath, FILE_READ);
    //Serial.println(settingPath);
    if (file) {
        //Serial.println(F(" exists, reading..."));
        String value;
        char c = file.read();
        int length = 1;
        while (c >= 0 && length <= maxLength) {
            //Serial.println(c);
            value.append(c);
            c = file.read();
            length++;
        }
        file.close();
        //Serial.println(value);
        intValue = value.toInt();
        //Serial.println(intValue);
    }

    return intValue;
}

void saveIntSetting(char* dir, const char* settingPath, int value) {
    if (!sdAvailable)
        return;

    if (!SD.exists(dir)) {
        //Serial.print(dir);
        //Serial.println(F(" dir doesn't exist"));
        SD.mkdir(dir);
    }
    //Serial.print("exists: ");
    //Serial.println(sd.exists(dir));

    File file = SD.open(settingPath, O_CREAT | O_TRUNC | O_WRITE);
    Serial.println(settingPath);
    if (file) {
        //Serial.println(value);
        file.print(value, 10);
        file.close();
    }
}
