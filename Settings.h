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
#include "SettingsMoveClock.h"
#include "SettingsSetTime.h"
#include "SettingsClockColor.h"
#include "SettingsClock24Hour.h"
#include "SettingsMenuColor.h"
#include "SettingsAutoplayDuration.h"
#include "SettingsAudioCalibration.h"
#include "SettingsUpdateFiles.h"
#include "SettingsDemoMode.h"

extern GifPlayer gifPlayer;

class Settings : public Runnable {
  private:
    Menu settingsMenu;

    SettingsBrightness brightness;
    SettingsBackgroundBrightness backgroundBrightness;
    SettingsMenuColor menuColor;
    SettingsMoveClock moveClock;
    SettingsClockColor clockColor;
    SettingsSetTime setTime;
    SettingsClock24Hour set24Hour;
    SettingsAutoplayDuration setAutoplayDuration;
    SettingsAudioCalibration audioCalibration;
    SettingsUpdateFiles updateFiles;
    SettingsDemoMode demoMode;
    Drawable exit;

    static const int itemCount = 12;

    MenuItem menuItemBrightness = MenuItem((char *)"Brightness", &brightness);
    MenuItem menuItemBgBrightness = MenuItem((char *)"BG Brightness", &backgroundBrightness);
    MenuItem menuItemMenuColor = MenuItem((char *)"Menu Color", &menuColor);
    MenuItem menuItemAutoplayDuration = MenuItem((char *)"Autoplay Duration", &setAutoplayDuration);
    MenuItem menuItemMoveClock = MenuItem((char *)"Move Clock", &moveClock);
    MenuItem menuItemClockColor = MenuItem((char *)"Clock Color", &clockColor);
    MenuItem menuItemClock24Hour = MenuItem((char *)"12/24 Hour Clock", &set24Hour);
    MenuItem menuItemSetTime = MenuItem((char *)"Set Time", &setTime);
    MenuItem menuItemNoiseReduction = MenuItem((char *)"Audio Calibration", &audioCalibration);
    MenuItem menuItemUpdateFiles = MenuItem((char *)"Update Files", &updateFiles);
    MenuItem menuItemDemoMode = MenuItem((char *)"Demo Mode", &demoMode);
    MenuItem menuItemExit = MenuItem((char *)"Exit Settings", &exit, true);

    MenuItem* menuItems[itemCount] = {
      &menuItemBrightness,
      &menuItemBgBrightness,
      &menuItemMenuColor,
      &menuItemAutoplayDuration,
      &menuItemMoveClock,
      &menuItemClockColor,
      &menuItemClock24Hour,
      &menuItemSetTime,
      &menuItemNoiseReduction,
      &menuItemUpdateFiles,
      &menuItemDemoMode,
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

      if (imageFile.isDirectory()) {
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

    char* Drawable::name = (char *)"Settings";

    void load() {
      audioCalibration.load();
    }

    void run() {
      menuItemMoveClock.visible = isTimeAvailable;
      menuItemClockColor.visible = isTimeAvailable;
      menuItemClock24Hour.visible = isTimeAvailable;
      menuItemSetTime.visible = isTimeAvailable;

      menuItemUpdateFiles.visible = *(unsigned long *)0x38080 != 0xffffffff;

      settingsMenu.canMoveBack = true;
      settingsMenu.run(menuItems, itemCount);
    }

    unsigned int drawFrame() {
      unsigned long result = 30;

      if (imageFile && imageFile.available()) {
        unsigned long result = gifPlayer.drawFrame();
        if (result == ERROR_FINISHED) {
          openImageFile();
        }
      }

      // matrix.setFont(font3x5);
      // matrix.drawString(0, 27, { 255, 255, 255 }, "v1.5");

      if (result < 0) {
        result = 30;
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
