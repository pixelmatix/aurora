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



class ClockCountdown : public Drawable {
public:
  char timeBuffer[9];

  tmElements_t target;
  tmElements_t remaining;

  time_t targetTime;

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

    indexedLayer.fillScreen(0);
    clockDigitalShort.drawFrame(0);

    indexedLayer.setFont(font3x5);
    indexedLayer.setIndexedColor(1, clockDigitalShort.color);
    indexedLayer.drawString(1, 26, 1, timeBuffer);

    return 0;
  }
  
  tmElements_t loadDateTimeSetting(const char* name) {
    tmElements_t value;
  
    if (!sdAvailable)
      return value;
  
    if (!SD.exists(auroraPath)) {
      SD.mkdir(auroraPath);
    }
  
    char filepath[20];
    strcpy(filepath, auroraPath);
    strcat(filepath, name);
  
    File file = SD.open(filepath, FILE_READ);
    if (file) {
      char c;
      value.Year = CalendarYrToTm(readIntB(file, 4));
      if (c >= 0) value.Month = readIntB(file, 2);
      if (c >= 0) value.Day = readIntB(file, 2);
      if (c >= 0) value.Hour = readIntB(file, 2);
      if (c >= 0) value.Minute = readIntB(file, 2);
      if (c >= 0) value.Second = readIntB(file, 2);
  
      file.close();
    }
  
    return value;
  }
  
  int readIntB(File &file, uint8_t maxLength) {
    String text;
    char c = file.read();
    int length = 1;
    while (c >= 0 && length <= maxLength) {
      text.append(c);
      c = file.read();
      length++;
    }
    return text.toInt();
  }

  void loadSettings() {
    target = loadDateTimeSetting("countdwn.txt");
    targetTime = makeTime(target);
  }
};
extern ClockCountdown clockCountdown;

#endif
