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

#include "MessagePlayer.h"
MessagePlayer messagePlayer;

#include "Effects.h"
Effects effects;

#include "IrCodes.h"

#include "GifPlayer.h"
GifPlayer gifPlayer;

#include "BitmapPlayer.h"
BitmapPlayer bitmapPlayer;

#include "Logo.h"

#include "Drawable.h"
#include "Playlist.h"
#include "Runnable.h"

tmElements_t time;
bool isTimeAvailable = false;

#include "ClockDigitalShort.h"
ClockDigitalShort clockDigitalShort;

#include "ClockText.h"
ClockText clockText;

#include "ClockDisplay.h"
ClockDisplay clockDisplay;

#include "Patterns.h"
Patterns patterns;

#include "Animations.h"
Animations animations;

#include "Bitmaps.h"

rgb24 menuColor = CRGB(CRGB::Blue);
int autoPlayDurationSeconds = 10;

#include "StreamingMode.h"
StreamingMode streamingMode;

#include "MenuItem.h"
#include "Menu.h"
Menu menu;

#include "Settings.h"
Settings settings;

#include "SettingsSetTime.h"
#include "SettingsMoveClock.h"

MenuItem menuItemPatterns = MenuItem((char*)"Patterns", &patterns);
MenuItem menuItemAnimations = MenuItem((char*)"Animations", &animations);
MenuItem menuItemSettings = MenuItem((char*)"Settings", &settings);

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
    Serial.begin(115200);

    // delay(3000);
    // Serial.println(F("starting..."));

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
        animations.setup((char *)"/gifs/");
        messagePlayer.setup((char *)"/messages/");
    }

    // setup the effects generator
    effects.CyclePalette();
    effects.NoiseVariablesSetup();

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

    if (!HAS_IR) {
        menu.playbackState = Menu::PlaybackState::Autoplay;
        menu.visible = false;
    }
}

void loop()
{
    menu.run(mainMenuItems, mainMenuItemCount);
}

void listPatterns() {
    patterns.listPatterns();
}

bool setPattern(String name) {
    if (patterns.setPattern(name)) {
        menu.currentIndex = 1;
        menu.visible = false;
        return true;
    }
    return false;
}

bool setPattern(int index) {
    if(patterns.setPattern(index)) {
        menu.currentIndex = 1;
        menu.visible = false;
        return true;
    }
    return false;
}

void listAnimations() {
    animations.listFiles();
}

bool setAnimation(String name) {
    if(animations.setAnimation(name)) {
        menu.currentIndex = 2;
        menu.visible = false;
        return true;
    }
    return false;
}

bool setAnimation(int index) {
    if (animations.setAnimation(index)) {
        menu.currentIndex = 2;
        menu.visible = false;
        return true;
    }

    return false;
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
            command == InputCommand::CycleBrightness)
            return;

        // go idle for a while, converve power
        delay(500);
    }
}

void loadSettings() {
    brightness = loadIntSetting("brghtnss.txt", 3, 255);
    boundBrightness();
    matrix.setBrightness(brightness);

    backgroundBrightness = loadIntSetting("bckbrght.txt", 3, 63);
    boundBackgroundBrightness();
    matrix.setBackgroundBrightness(backgroundBrightness);

    menuColor.red = loadIntSetting("menuR.txt", 3, 0);
    menuColor.green = loadIntSetting("menuG.txt", 3, 0);
    menuColor.blue = loadIntSetting("menuB.txt", 3, 255);

    autoPlayDurationSeconds = loadIntSetting("autoplyd.txt", 3, 10);

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
}

uint8_t cycleBrightness() {
    adjustBrightness(1);
    saveBrightnessSetting();

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
    saveIntSetting("brghtnss.txt", brightness);
}

void saveBackgroundBrightnessSetting() {
    saveIntSetting("bckbrght.txt", backgroundBrightness);
}

void saveMenuColor() {
    saveMenuR();
    saveMenuG();
    saveMenuB();
}

void saveMenuR() {
    saveIntSetting("menuR.txt", menuColor.red);
}

void saveMenuG() {
    saveIntSetting("menuG.txt", menuColor.green);
}

void saveMenuB() {
    saveIntSetting("menuB.txt", menuColor.blue);
}

void saveAutoPlayDurationSeconds() {
    saveIntSetting("autoplyd.txt", autoPlayDurationSeconds);
}

int loadIntSetting(const char* name, int maxLength, int defaultValue) {
    if (!sdAvailable)
        return defaultValue;

    int intValue = defaultValue;

    char* path = (char *) "/aurora/";
    
    if (!SD.exists(path)) {
        SD.mkdir(path);
    }

    char filepath[255];
    strcpy(filepath, path);
    strcat(filepath, name);

//    Serial.print("loading ");
//    Serial.println(filepath);

    File file = SD.open(filepath, FILE_READ);
    if (file) {
        String value;
        char c = file.read();
        int length = 1;
        while (c >= 0 && length <= maxLength) {
            value.append(c);
            c = file.read();
            length++;
        }
        file.close();
        intValue = value.toInt();
    }

    return intValue;
}

void saveIntSetting(const char* name, int value) {
    if (!sdAvailable)
        return;

    char* path = (char *) "/aurora/";
    
    if (!SD.exists(path)) {
        SD.mkdir(path);
    }

    char filepath[255];
    strcpy(filepath, path);
    strcat(filepath, name);

//    Serial.print("saving ");
//    Serial.println(filepath);

    File file = SD.open(filepath, O_CREAT | O_TRUNC | O_WRITE);
    if (file) {
        file.print(value, 10);
        file.close();
    }
}

// translates from x, y into an index into the LED array
uint16_t XY(uint8_t x, uint8_t y) {
    if (y >= MATRIX_HEIGHT) { y = MATRIX_HEIGHT - 1; }
    if (y < 0) { y = 0; }
    if (x >= MATRIX_WIDTH) { x = MATRIX_WIDTH - 1; }
    if (x < 0) { x = 0; }

    return (y * MATRIX_WIDTH) + x;
}
