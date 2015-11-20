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

#ifndef PatternInfinity_H

class PatternInfinity : public Drawable {
public:
    PatternInfinity() {
        name = (char *)"Infinity";
    }

    unsigned int drawFrame() {
        // dim all pixels on the display slightly 
        // to 250/255 (98%) of their current brightness
        effects.DimAll(250);

        // the horizontal position of the head of the infinity sign
        // oscillates from 0 to the maximum horizontal and back
        int x = beatsin8(15, 1, MATRIX_WIDTH - 1);

        // the vertical position of the head oscillates
        int y = (MATRIX_HEIGHT - 1) - beatsin8(30, MATRIX_HEIGHT / 4, ((MATRIX_HEIGHT / 4) * 3) - 1);

        // the hue oscillates from 0 to 255, overflowing back to 0
        static byte hue = 0;
        hue++;

        CRGB color = effects.ColorFromCurrentPalette(hue);
        
        effects.leds[XY(x, y)] = color;
        effects.leds[XY(x + 1, y)] = color;
        effects.leds[XY(x, y + 1)] = color;
        effects.leds[XY(x - 1, y)] = color;
        effects.leds[XY(x, y - 1)] = color;

        return 0;
    }
};

#endif
