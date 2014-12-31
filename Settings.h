/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
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

#ifndef Settings_H
#define Settings_H

#include "SettingsBrightness.h"
#include "SettingsBackgroundBrightness.h"
//#include "SettingsChooseClock.h"
#include "SettingsMoveClock.h"
#include "SettingsSetTime.h"
#include "SettingsClockColor.h"
#include "SettingsClock24Hour.h"

extern GifPlayer gifPlayer;

class Settings : public Runnable {
private:
    Menu settingsMenu;

    SettingsBrightness brightness;
    SettingsBackgroundBrightness backgroundBrightness;
    //SettingsChooseClock chooseClock;
    SettingsMoveClock moveClock;
    SettingsClockColor clockColor;
    SettingsSetTime setTime;
    SettingsClock24Hour set24Hour;
    Drawable exit;

    static const int itemCount = 7;

    MenuItem menuItemBrightness = MenuItem("Brightness", &brightness);
    MenuItem menuItemBgBrightness = MenuItem("BG Brightness", &backgroundBrightness);
    //MenuItem menuItemClockFace = MenuItem("Clock Face", &chooseClock);
    MenuItem menuItemMoveClock = MenuItem("Move Clock", &moveClock);
    MenuItem menuItemClockColor = MenuItem("Clock Color", &clockColor);
    MenuItem menuItemClock24Hour = MenuItem("12/24 Hour Clock", &set24Hour);
    MenuItem menuItemSetTime = MenuItem("Set Time", &setTime);
    MenuItem menuItemExit = MenuItem("Exit Settings", &exit, true);

    MenuItem* menuItems[itemCount] = {
        &menuItemBrightness,
        &menuItemBgBrightness,
        // &menuItemClockFace,
        &menuItemMoveClock,
        &menuItemClockColor,
        &menuItemClock24Hour,
        &menuItemSetTime,
        &menuItemExit,
    };

    File imageFile;

    void openImageFile() {
        if (!sdAvailable)
            return;

        if (imageFile)
            imageFile.close();

        char filepath [] = "/aurora/gearblue.gif"; // gearblu2.gif

        if (!SD.exists(filepath))
            return;

        imageFile = SD.open(filepath, FILE_READ);
        if (!imageFile)
            return;

        if (imageFile.isDirectory()){
            imageFile.close();
            return;
        }

        gifPlayer.setFile(imageFile);

        if (!gifPlayer.parseGifHeader()) {
            imageFile.close();
            return;
        }

        gifPlayer.parseLogicalScreenDescriptor();
        gifPlayer.parseGlobalColorTable();
    }

public:
    Settings() {
    }

    char* Drawable::name = "Settings";

    void run() {
        menuItemMoveClock.visible = isTimeAvailable;
        menuItemClockColor.visible = isTimeAvailable;
        menuItemClock24Hour.visible = isTimeAvailable;
        menuItemSetTime.visible = isTimeAvailable;

        settingsMenu.canMoveBack = true;
        settingsMenu.run(menuItems, itemCount);
    }

    unsigned int drawFrame() {
        if (!imageFile && !imageFile.available())
            return 30;

        unsigned long result = gifPlayer.drawFrame();
        if (result == ERROR_FINISHED) {
            openImageFile();
        }
        else if (result < 0) {
            return 30;
        }

        return result;
    }

    void start() {
        matrix.fillScreen(CRGB(CRGB::Black));
        openImageFile();
    }

    void stop() {
        if (imageFile)
            imageFile.close();
    }
};

#endif
