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

#ifndef SettingsClock24Hour_H
#define SettingsClock24Hour_H

class SettingsClock24Hour : public Runnable {
private:
public:
    void run() {
        while (true) {
            drawFrame();
            clockDisplay.drawFrame();
            clockDisplay.drawSetTimeIndicator(SetHour);

            matrix.swapBuffers();

            unsigned long irCode = readIRCode(defaultHoldDelay);

            switch (irCode) {
                case IRCODE_UP:
                case IRCODE_DOWN:
                    clockDigitalShort.twentyFourHour = !clockDigitalShort.twentyFourHour;
                    clockDigitalShort.saveTwentyFourHourSetting();
                    break;

                case IRCODE_A:
                case IRCODE_SELECT:
                case IRCODE_POWER:
                    return;
            }
        }
    }

    unsigned int drawFrame() {
        matrix.fillScreen(CRGB(CRGB::Black));
        return 0;
    }
};

#endif
