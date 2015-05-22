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

#ifndef ClockCountdown_H
#define ClockCountdown_H

#include "Aurora.h"
extern tmElements_t time;
extern bool isTimeAvailable;

class ClockCountdown : public Drawable {
  public:
    char timeBuffer[9];

    tmElements_t target;
    tmElements_t remaining;

    ClockCountdown() {
    }

    void printTime(tmElements_t t) {
      Serial.print(t.Year);
      Serial.print("-");
      Serial.print(t.Month);
      Serial.print("-");
      Serial.print(t.Day);

      Serial.print(" ");

      Serial.print(t.Hour);
      Serial.print(":");
      Serial.print(t.Minute);
      Serial.print(":");
      Serial.println(t.Second);
    }

    unsigned int drawFrame() {
      if (isTimeAvailable) {
        time_t targetTime = makeTime(target);
        time_t currentTime = makeTime(time);

        //        Serial.print("target: ");
        //        printTime(target);
        //        Serial.print("current: ");
        //        printTime(time);

        if (targetTime > currentTime) {
          unsigned long timeRemaining = targetTime - currentTime;

          uint8_t hoursRemaining = timeRemaining / SECS_PER_HOUR;
          uint8_t minutesRemaining = (timeRemaining - (hoursRemaining * SECS_PER_HOUR)) / 60;
          uint8_t secondsRemaining = timeRemaining - ((hoursRemaining * SECS_PER_HOUR) + (minutesRemaining * 60));

          sprintf(timeBuffer, "%02d:%02d:%02d", hoursRemaining, minutesRemaining, secondsRemaining);
        }
        else {
          sprintf(timeBuffer, "%02d:%02d:%02d", 0, 0, 0);
        }
      }
      else {
        sprintf(timeBuffer, "No Clock");
      }

      matrix.clearForeground();
      clockDigitalShort.drawFrame(0);

      matrix.setForegroundFont(font3x5);
      matrix.setScrollOffsetFromTop(MATRIX_HEIGHT);
      matrix.setScrollColor(clockDigitalShort.color);
      matrix.drawForegroundString(1, 26, timeBuffer, true);

      return 0;
    }

    void loadSettings() {
      target = loadDateTimeSetting("countdwn.txt");
    }
};
extern ClockCountdown clockCountdown;

#endif
