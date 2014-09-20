/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from FastLED Fire2012 example by Mark Kriegsman: https://github.com/FastLED/FastLED/blob/FastLED2.1/examples/Noise/Noise.ino
 * Copyright (c) 2013 FastLED
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

#ifndef PatternSimplexNoise_H
#define PatternSimplexNoise_H

class PatternSimplexNoise : public Drawable {
private:
    // The 32bit version of our coordinates
    uint16_t x;
    uint16_t y;
    uint16_t z;

    // This is the array that we keep our computed noise values in
    uint8_t noise[MATRIX_WIDTH][MATRIX_HEIGHT];

public:
    PatternSimplexNoise() {
        // Initialize our coordinates to some random values
        x = random16();
        y = random16();
        z = random16();
    }

    unsigned int drawFrame() {
        loop();
        return 0;
    }

    // speed - how fast time moves forward
    // scale - how far apart the pixels in our noise matrix are
    void loop(uint16_t speed = 4, uint16_t scale = 10, bool interactive = false) {
        // We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
        // use the z-axis for "time".  speed determines how fast time moves forward.  Try
        // 1 for a very slow moving effect, or 60 for something that ends up looking like
        // water.
        // uint16_t speed = 1; // almost looks like a painting, moves very slowly
        // uint16_t speed = 4; // nice and slow plasma
        // uint16_t speed = 20; // 20 is a nice starting speed, mixes well with a scale of 100
        // uint16_t speed = 33;
        // uint16_t speed = 100; // wicked fast!

        // Scale determines how far apart the pixels in our noise matrix are.  Try
        // changing these values around to see how it affects the motion of the display.  The
        // higher the value of scale, the more "zoomed out" the noise iwll be.  A value
        // of 1 will be so zoomed in, you'll mostly see solid colors.

        // uint16_t scale = 1; // mostly just solid colors
        // uint16_t scale = 10; // nice and big plasma
        // uint16_t scale = 4011; // very zoomed out and shimmery
        // uint16_t scale = 10; // 311

        // Fill the x/y array of 8-bit noise values using the inoise8 function.
        for (int i = 0; i < MATRIX_WIDTH; i++) {
            int ioffset = scale * i;
            for (int j = 0; j < MATRIX_HEIGHT; j++) {
                int joffset = scale * j;
                noise[i][j] = inoise8(x + ioffset, y + joffset, z);
                effects.leds[effects.XY(i, j)] = effects.ColorFromCurrentPalette(noise[j][i], noise[i][j]);
            }
        }
        z += speed;
    }
};

#endif