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

#ifndef PatternAnalogClock_H

extern ClockDisplay clockDisplay;

class PatternAnalogClock : public Drawable {
private:
#define HOUR_HAND_RADIUS 15
#define MIN_HAND_RADIUS  15
#define SEC_IND_RADIUS   15

public:
    PatternAnalogClock() {
        name = (char *)"AnalogClock";
    }

    void start() {
    }

    unsigned int drawFrame() {
        effects.DimAll(250);

        clockDisplay.readTime();

        if (isTimeAvailable) {
            uint8_t hour = time.Hour;
            if (hour > 12)
                hour -= 12;

            double radians;
            int x;
            int y;
            // hour
            radians = (90.0F - ((hour * 30.0F) + ((time.Minute * 29.0F) / 60.0F))) * M_PI / 180.0F;
            x = round(MATRIX_CENTRE_X + HOUR_HAND_RADIUS * cos(radians));
            y = round(MATRIX_CENTRE_Y - HOUR_HAND_RADIUS * sin(radians));
            effects.leds[XY(x, y)] += {255, 0, 0};

            // minute
            radians = (90.0F - (time.Minute * 6.0F)) * M_PI / 180.0;
            x = round(MATRIX_CENTRE_X + MIN_HAND_RADIUS * cos(radians));
            y = round(MATRIX_CENTRE_Y - MIN_HAND_RADIUS * sin(radians));
            effects.leds[XY(x, y)] += {0, 255, 0};

            // second
            radians = (90.0F - ((float)time.Second * 6.0F)) * M_PI / 180.0F;
            x = round(MATRIX_CENTRE_X + SEC_IND_RADIUS * cos(radians));
            y = round(MATRIX_CENTRE_Y - SEC_IND_RADIUS * sin(radians));
            effects.leds[XY(x, y)] += {0, 0, 255};

            // millisecond
            uint8_t xm = beatcos8(60, 0, MATRIX_CENTER_X + SEC_IND_RADIUS);
            uint8_t ym = beatsin8(60, 0, MATRIX_CENTER_Y + SEC_IND_RADIUS);
            effects.leds[XY(xm, ym)] += effects.ColorFromCurrentPalette(beat8(1));
        }
        else {
            matrix.setFont(font3x5);
            matrix.drawString(0, 0, { 255, 255, 255 }, "No Clock");
        }

        return 0;
    }
};

#endif
