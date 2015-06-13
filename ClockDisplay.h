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

// Portions of this code are adapted from PJRC DS1307RTC Library: https://www.pjrc.com/teensy/td_libs_DS1307RTC.html
// Uses PJRC Time Library: https://www.pjrc.com/teensy/td_libs_Time.html

#ifndef ClockDisplay_H
#define ClockDisplay_H

#include "Externs.h"


class ClockDisplay : public Playlist {
  public:
    unsigned int lastRead = 0;
    const unsigned int readInterval = 1000; // ms

    int currentIndex = 0;

    static const int itemCount = 4;

    Drawable* items[itemCount] = {
      &clockDigitalShort,
      &clockText,
      &clockCountdown,
      &clockPong,
    };

    Drawable* currentItem = items[currentIndex];

    int getCurrentIndex() {
      return currentIndex;
    }

    rgb24 color = CRGB(CRGB::White);

    void setColor(rgb24 color) {
      ClockDisplay::color = color;
      clockDigitalShort.color = color;
      clockText.color = color;
    }

    virtual void move(int step) {
      moveTo(currentIndex + step);
    }

    virtual void moveRandom(int step) {
      moveTo(random(0, itemCount));
    }

    void moveTo(int index) {
      currentIndex = index;

      if (currentIndex >= itemCount)
        currentIndex = 0;
      else if (currentIndex < 0)
        currentIndex = itemCount - 1;

      if (currentItem)
        currentItem->stop();

      currentItem = items[currentIndex];

      if (currentItem)
        currentItem->start();
    }

    bool hasSetSyncProvider = false;

    bool readTimeHardware() {
      hasDS1307RTC = false;

      // try to read the DS1307RTC
      if (RTC.read(time)) {
        hasDS1307RTC = true;
        return true;
      }
      else {
        if (!hasSetSyncProvider) {
          // set the Time library to use Teensy 3.0's RTC to keep time
          setSyncProvider(getTeensy3Time);
          hasSetSyncProvider = true;
        }

        breakTime(now(), time);
        return true;
      }
    }

    void readTime() {
      // reading from the RTC only takes 1-2ms, but there's no point in reading it every frame
      // so we'll only check it every so often, and then cache the time text
      if (lastRead == 0 || millis() - lastRead > readInterval) {
        if (readTimeHardware()) {
          isTimeAvailable = true;
          lastRead = millis();
        }
        else {
          isTimeAvailable = false;
        }
      }
    }

    unsigned int drawFrame() {
      readTime();

      if (currentItem)
        currentItem->drawFrame();

      return 0;
    }

    void adjustY(int d) {
      clockDigitalShort.y += d;
      clockDigitalShort.boundY();
    }

    void drawMoveClockIndicator() {
      clockDigitalShort.drawMoveClockIndicator();
    }

    char* clockR = (char*) "clockR.txt";
    char* clockG = (char*) "clockG.txt";
    char* clockB = (char*) "clockB.txt";

    void loadSettings() {
      clockCountdown.loadSettings();
      clockDigitalShort.loadSettings();
      move(0);

      color.red = loadByteSetting(clockR, 255);
      color.green = loadByteSetting(clockG, 255);
      color.blue = loadByteSetting(clockB, 255);
      setColor(color);
    }

    void saveSettings() {
      clockDigitalShort.saveSettings();

      saveColor();
      setColor(color);
    }

    void saveClockYSetting() {
      clockDigitalShort.saveClockYSetting();
    }

    void saveTwentyFourHourSetting() {
      clockDigitalShort.saveTwentyFourHourSetting();
    }

    void saveColor() {
      saveIntSetting(clockR, color.red);
      saveIntSetting(clockG, color.green);
      saveIntSetting(clockB, color.blue);
    }
};

#endif
