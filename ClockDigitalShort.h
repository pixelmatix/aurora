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

#ifndef ClockDigitalShort_H
#define ClockDigitalShort_H

enum SetTimeState{
    SetHour,
    SetMinute,
};

static const unsigned int SetTimeStatesCount = 2;
SetTimeState SetTimeStates[SetTimeStatesCount] = {
    SetHour,
    SetMinute,
};

class ClockDigitalShort : public Drawable {
public:
    char timeBuffer[5];
    int16_t y = 11;
    rgb24 color = CRGB(CRGB::White);
    bool hasShownError = false;

    bool twentyFourHour = false;

    unsigned int drawFrame() {
        drawFrame(y);
    }

    unsigned int drawFrame(int y) {
        if (isTimeAvailable) {
            uint8_t hour = time.Hour;
            if (!twentyFourHour && hour > 12)
                hour -= 12;

            sprintf(timeBuffer, "%d:%02d", hour, time.Minute, time.Second);
            matrix.setScrollOffsetFromEdge(MATRIX_HEIGHT);
            matrix.setForegroundFont(gohufont11b);
            matrix.setScrollColor(color);
            matrix.clearForeground();

            int x = 1;
            if (hour < 10)
                x = 5;

            matrix.drawForegroundString(x, y, timeBuffer, true);

            // push change to the foreground, no need to wait for the copy to complete as we clear and redraw each time
            matrix.displayForegroundDrawing(false);
        }
        else if (!hasShownError) {
            hasShownError = true;
            matrix.setScrollMode(wrapForwardFromLeft);
            matrix.setScrollFont(gohufont11b);
            matrix.setScrollColor(color);
            matrix.scrollText("No Clock", 1);
        }

        return 0;
    }

    void drawSetTimeIndicator(SetTimeState state) {
        int16_t x = 0;

        uint8_t hour = time.Hour;
        if (!twentyFourHour && hour > 12)
            hour -= 12;

        switch (state) {
            case SetHour:
                if (hour > 9)
                    x = 2;
                else
                    x = 0;
                break;

            case SetMinute:
                if (hour > 9)
                    x = 20;
                else
                    x = 18;
                break;
        }

        // upper indicators (in case the clock's at the bottom)
        if (state != SetHour || hour > 9)
            matrix.drawTriangle(x + 0, y - 1, x + 1, y - 2, x + 2, y - 1, CRGB(CRGB::SlateGray));
        matrix.drawTriangle(x + 6, y - 1, x + 7, y - 2, x + 8, y - 1, CRGB(CRGB::SlateGray));

        // lower indicators (in case the clock's at the top)
        if (state != SetHour || hour > 9)
            matrix.drawTriangle(x + 0, y + 11, x + 1, y + 12, x + 2, y + 11, CRGB(CRGB::SlateGray));
        matrix.drawTriangle(x + 6, y + 11, x + 7, y + 12, x + 8, y + 11, CRGB(CRGB::SlateGray));
    }

    void drawMoveClockIndicator() {
        for (int16_t x = 2; x <= 26; x += 6) {
            // upper indicators (in case the clock's at the bottom)
            matrix.drawTriangle(x + 0, y - 1, x + 1, y - 2, x + 2, y - 1, CRGB(CRGB::SlateGray));

            // lower indicators (in case the clock's at the top)
            matrix.drawTriangle(x + 0, y + 11, x + 1, y + 12, x + 2, y + 11, CRGB(CRGB::SlateGray));
        }
    }

    void loadSettings() {
        y = loadIntSetting("/aurora/", "/aurora/clockY.txt", 2, 11);
        twentyFourHour = loadIntSetting("/aurora/", "/aurora/clock24h.txt", 2, 11);
        boundY();
    }

    void saveSettings() {
        saveClockYSetting();
    }

    void saveClockYSetting() {
        saveIntSetting("/aurora/", "/aurora/clockY.txt", y);
    }

    void saveTwentyFourHourSetting() {
        saveIntSetting("/aurora/", "/aurora/clock24h.txt", twentyFourHour);
    }

    void boundY() {
        if (y < 0)
            y = 0;
        else if (y > 23)
            y = 23;
    }
};

#endif