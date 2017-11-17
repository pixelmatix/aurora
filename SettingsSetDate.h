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

#ifndef SettingsSetDate_H
#define SettingsSetDate_H

enum SetDateState {
  SetYear,
  SetMonth,
  SetDay,
};

static const unsigned int SetDateStatesCount = 3;
SetDateState SetDateStates[SetDateStatesCount] = {
  SetYear,
  SetMonth,
  SetDay,
};

class SettingsSetDate : public Runnable {
  private:
    SetDateState state = SetYear;
    unsigned int currentStateIndex = 0;

    char timeBuffer[9];

    void setTimeHardware() {
      if (hasDS1307RTC)
        RTC.write(time);
      else {
        time_t t = makeTime(time);
        Teensy3Clock.set(t);
        setTime(t);
      }
    }

  public:

    void run() {
      while (true) {
        draw();

        InputCommand command = readCommand(defaultHoldDelay);

        switch (command) {
          case InputCommand::Up:
            adjust(1);
            break;

          case InputCommand::Down:
            adjust(-1);
            break;

          case InputCommand::Left:
            currentStateIndex--;
            break;

          case InputCommand::Right:
            currentStateIndex++;
            break;

          case InputCommand::Select:
          case InputCommand::Back:
            return;

          default:
            break;
        }

        if (currentStateIndex > SetTimeStatesCount)
          currentStateIndex = 0;
        else if (currentStateIndex < 0)
          currentStateIndex = SetTimeStatesCount - 1;

        state = SetDateStates[currentStateIndex];
      }
    }

    void draw() {
      backgroundLayer.fillScreen(CRGB(CRGB::Black));

      int x = 0;
      int y = 13;

      indexedLayer.setFont(font3x5);

      if (isTimeAvailable) {
        uint16_t year = time.Year + 1970;
        while (year > 999)
          year -= 1000;
        while (year > 99)
          year -= 100;

        sprintf(timeBuffer, "%02d-%02d-%02d", year, time.Month, time.Day);
      }
      else {
        sprintf(timeBuffer, "No Clock");
      }

      indexedLayer.setIndexedColor(1, clockDisplay.color);
      indexedLayer.fillScreen(0);

      // draw the date
      indexedLayer.drawString(x, y, 1, timeBuffer);

      // draw instruction text
      switch (state) {
        case SetYear:
          indexedLayer.drawString(1, 0, 1, "Year");
          x = 0;
          break;

        case SetMonth:
          indexedLayer.drawString(1, 0, 1, "Month");
          x = 12;
          break;

        case SetDay:
          indexedLayer.drawString(1, 0, 1, "Day");
          x = 24;
          break;

        default:
          break;
      }

      // draw up arrows
      backgroundLayer.drawTriangle(x + 0, y - 2, x + 1, y - 3, x + 2, y - 2, CRGB(CRGB::SlateGray));
      backgroundLayer.drawTriangle(x + 4, y - 2, x + 5, y - 3, x + 6, y - 2, CRGB(CRGB::SlateGray));

      // draw down arrows
      backgroundLayer.drawTriangle(x + 0, y + 6, x + 1, y + 7, x + 2, y + 6, CRGB(CRGB::SlateGray));
      backgroundLayer.drawTriangle(x + 4, y + 6, x + 5, y + 7, x + 6, y + 6, CRGB(CRGB::SlateGray));

      backgroundLayer.swapBuffers();
      indexedLayer.swapBuffers();
    }

    unsigned int drawFrame() {
      backgroundLayer.fillScreen(CRGB(CRGB::Black));
      backgroundLayer.setFont(font3x5);
      backgroundLayer.drawString(0, MATRIX_HEIGHT - 5, { 255, 255, 255 }, versionText);
      return 0;
    }

    void adjust(int d) {
      switch (state) {
        case SetYear:
          if (d > 0 || time.Year > 0)
            time.Year += d;
          break;

        case SetMonth:
          if (d > 0 && time.Month == 12)
            time.Month = 1;
          else if (d < 1 && time.Month == 1)
            time.Month = 12;
          else
            time.Month += d;
          break;

        case SetDay:
          if (d > 0 && time.Day == 31)
            time.Day = 1;
          else if (d < 1 && time.Day == 1)
            time.Day = 31;
          else
            time.Day += d;
          break;

        default:
          break;
      }

      setTimeHardware();
    }
};

#endif
