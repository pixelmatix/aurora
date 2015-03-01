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

#ifndef ClockText_H
#define ClockText_H

#include "Aurora.h"
extern tmElements_t time;
extern bool isTimeAvailable;

class ClockText : public Drawable {
private:
    char* onesStrings[20] =
    {
        (char *)"twelve",
        (char *)"one",
        (char *)"two",
        (char *)"three",
        (char *)"four",
        (char *)"five",
        (char *)"six",
        (char *)"seven",
        (char *)"eight",
        (char *)"nine",
        (char *)"ten",
        (char *)"eleven",
        (char *)"twelve",
        (char *)"thirteen",
        (char *)"fourteen",
        (char *)"fifteen",
        (char *)"sixteen",
        (char *)"seventeen",
        (char *)"eighteen",
        (char *)"nineteen"
    };

    char* tensStrings[4] =
    {
        (char *)"twenty",
        (char *)"thirty",
        (char *)"forty",
        (char *)"fifty"
    };

public:
    rgb24 color = CRGB(CRGB::White);

    unsigned int drawFrame() {
        uint8_t y = clockDigitalShort.y;

        if (y < 0)
            y = 0;
        else if (y > 11)
            y = 11;

        return drawFrame(y);
    }

    unsigned int drawFrame(const int cy) {
        matrix.setScrollOffsetFromTop(MATRIX_HEIGHT);
        matrix.setScrollColor(color);
        matrix.setForegroundFont(font3x5);
        matrix.clearForeground();

        int x = 1;

        if (isTimeAvailable) {
            uint8_t hour = time.Hour;
            uint8_t minute = time.Minute;

            if (hour > 12)
                hour -= 12;

            matrix.drawForegroundString(x, cy, onesStrings[hour]);

            if (minute == 0) {
                matrix.drawForegroundString(x, cy + 7, "o'clock");
            }
            else if (minute < 10) {
                matrix.drawForegroundString(x, cy + 7, "oh");
                matrix.drawForegroundString(x, cy + 14, onesStrings[minute]);
            }
            else if (minute == 17) {
                matrix.drawForegroundString(x, cy + 7, onesStrings[7]);
                matrix.drawForegroundString(x, cy + 14, "teen");
            }
            else if (minute < 20) {
                matrix.drawForegroundString(x, cy + 7, onesStrings[minute]);
            }
            else {
                matrix.drawForegroundString(x, cy + 7, tensStrings[minute / 10 - 2]);

                if (minute % 10 > 0) {
                    matrix.drawForegroundString(x, cy + 14, onesStrings[minute % 10]);
                }
            }
        }
        else {
            matrix.drawForegroundString(x, cy, "No Clock", true);
        }

        return 0;
    }
};
extern ClockText clockText;

#endif
