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

#ifndef SettingsClockColor_H
#define SettingsClockColor_H

class SettingsClockColor : public Runnable {
private:
    int cursorX = 0;
    int cursorY = 0;

public:

    void start() {
        CHSV chsv;
        CRGB crgb;
        CRGB selectedColor;
        for (int x = 0; x < MATRIX_WIDTH - 1; x += 1) {
            uint8_t hue = x * 8;

            for (int y = 0; y < MATRIX_HEIGHT; y += 1) {
                if (y <= 16) {
                    chsv = CHSV(hue, y * 15 + 15, 255);
                }
                else {
                    chsv = CHSV(hue, 255, 255 - (y - 17) * 15);
                }

                hsv2rgb_rainbow(chsv, crgb);
                if (crgb.r == clockDisplay.color.red &&
                    crgb.g == clockDisplay.color.green &&
                    crgb.b == clockDisplay.color.blue) {
                    cursorX = x;
                    cursorY = y;
                    return;
                }
            }
        }
    }

    void run() {
        int lastStateIndex = -1;

        while (true) {
            drawFrame();

            // draw an RGB grid, Hue from 0 to 248 left to right (256 is the same as 0)
            // Saturation from 15 (mostly white) to 255 (fully-saturated color) top to middle (0 to 16), skipping 0 which is white and will be handled in the right-most column)
            // then Value from 255 (fully-saturated color) to 45 (mostly black) middle to bottom, skipping anything lower than 45 which is indistinguishable from black.
            CHSV chsv;
            CRGB crgb;
            CRGB selectedColor;
            for (int x = 0; x < MATRIX_WIDTH - 1; x += 1) {
                uint8_t hue = x * 8;

                for (int y = 0; y < MATRIX_HEIGHT; y += 1) {
                    if (y <= 16) {
                        chsv = CHSV(hue, y * 15 + 15, 255);
                    }
                    else {
                        chsv = CHSV(hue, 255, 255 - (y - 17) * 15);
                    }

                    hsv2rgb_rainbow(chsv, crgb);
                    matrix.drawPixel(x, y, crgb);

                    if (x == cursorX && y == cursorY)
                        selectedColor = crgb;
                }
            }

            // draw a grayscale strip down the right edge
            // from 0 (white) to 69 (mostly black), skipping anything lower than 69 which is almost indistinguishable from black.
            int x = MATRIX_WIDTH - 1;
            for (int y = 0; y < MATRIX_HEIGHT; y += 1) {
                chsv = CHSV(0, 0, 255 - y * 6);
                hsv2rgb_rainbow(chsv, crgb);
                matrix.drawPixel(x, y, crgb);

                if (x == cursorX && y == cursorY)
                    selectedColor = crgb;
            }

            // draw the cursor crosshairs
            uint8_t v = cursorY * 8;
            crgb = CRGB(v, v, v);
            // horizontal cursor lines
            matrix.drawLine(cursorX - 4, cursorY, cursorX - 2, cursorY, crgb);
            matrix.drawLine(cursorX + 4, cursorY, cursorX + 2, cursorY, crgb);
            // vertical cursor lines
            matrix.drawLine(cursorX, cursorY - 4, cursorX, cursorY - 2, crgb);
            matrix.drawLine(cursorX, cursorY + 4, cursorX, cursorY + 2, crgb);

            // draw the clock numbers at the bottom, so we can see the selected color better, without leaving the settings item
            clockDisplay.setColor(selectedColor);
            clockDisplay.readTime();

            if (cursorY >= 18) {
                clockDigitalShort.drawFrame(0);
            }
            else {
                clockDigitalShort.drawFrame(23);
            }

            matrix.swapBuffers();

            unsigned long irCode = readIRCode(defaultHoldDelay);

            switch (irCode) {
                case IRCODE_LEFT:
                    cursorX -= 1;
                    break;

                case IRCODE_RIGHT:
                    cursorX += 1;
                    break;

                case IRCODE_UP:
                    cursorY -= 1;
                    break;

                case IRCODE_DOWN:
                    cursorY += 1;
                    break;

                case IRCODE_A:
                case IRCODE_SELECT:
                case IRCODE_POWER:
                    save(selectedColor);
                    return;
            }

            if (cursorX < 0)
                cursorX = MATRIX_WIDTH - 1;
            else if (cursorX >= MATRIX_WIDTH)
                cursorX = 0;

            if (cursorY < 0)
                cursorY = MATRIX_HEIGHT - 1;
            else if (cursorY >= MATRIX_HEIGHT)
                cursorY = 0;
        }
    }

    unsigned int drawFrame() {
        matrix.fillScreen(CRGB(CRGB::Black));
        return 0;
    }

    void save(CRGB crgb) {
        clockDisplay.setColor(crgb);
        clockDisplay.saveColor();
    }
};

#endif