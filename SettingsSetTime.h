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

#ifndef SettingsSetTime_H
#define SettingsSetTime_H

class SettingsSetTime : public Runnable {
  private:
    SetTimeState state = SetHour;
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

        state = SetTimeStates[currentStateIndex];
      }
    }

    void draw() {
      backgroundLayer.fillScreen(CRGB(CRGB::Black));

      int x = 0;
      int y = 13;

      scrollingLayer.setFont(font3x5);

      clockDisplay.readTime();

      sprintf(timeBuffer, "%02d-%02d-%02d", time.Hour, time.Minute, time.Second);

      scrollingLayer.setOffsetFromTop(MATRIX_HEIGHT);
      scrollingLayer.setColor(clockDisplay.color);
      indexedLayer.fillScreen(0);

      // draw the time
      indexedLayer.drawString(x, y, 1, timeBuffer);

      // draw instruction text
      switch (state) {
        case SetHour:
          indexedLayer.drawString(1, 0, 1, "24 Hour");
          x = 0;
          break;

        case SetMinute:
          indexedLayer.drawString(1, 0, 1, "Minute");
          x = 12;
          break;

        case SetSecond:
          indexedLayer.drawString(1, 0, 1, "Second");
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
      backgroundLayer.drawString(0, 27, { 255, 255, 255 }, versionText);
      return 0;
    }

    void adjust(int d) {
      switch (state) {
        case SetHour:
          if (d > 0 && time.Hour == 23)
            time.Hour = 0;
          else if (d < 1 && time.Hour == 0)
            time.Hour = 23;
          else
            time.Hour += d;
          break;

        case SetMinute:
          if (d > 0 && time.Minute == 59)
            time.Minute = 0;
          else if (d < 1 && time.Minute == 0)
            time.Minute = 59;
          else
            time.Minute += d;
          break;


        case SetSecond:
          if (d > 0 && time.Second == 59)
            time.Second = 0;
          else if (d < 1 && time.Second == 0)
            time.Second = 59;
          else
            time.Second += d;
          break;
      }

      setTimeHardware();
    }
};

#endif
