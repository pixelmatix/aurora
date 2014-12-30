/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from "Funky Clouds" by Stefan Petrick: https://gist.github.com/anonymous/876f908333cd95315c35
 * Copyright (c) 2014 Stefan Petrick
 * http://www.stefan-petrick.de/wordpress_beta
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

#ifndef Effects_H
#define Effects_H

#define MAX_COLOR_VALUE 255

extern SmartMatrix matrix;

class Effects {
public:
    CRGB *leds;
    int ledCount = MATRIX_WIDTH * MATRIX_HEIGHT;

    // palettes
    static const int paletteCount = 10;
    int paletteIndex = -1;
    TBlendType currentBlendType = BLEND;
    CRGBPalette16 currentPalette;

    static const int HeatColorsPaletteIndex = 7;

    void CyclePalette(int offset = 1) {
        loadPalette(paletteIndex + offset);
    }

    void RandomPalette() {
        loadPalette(random(0, paletteCount));
    }

    void loadPalette(int index) {
        paletteIndex = index;

        if (paletteIndex >= paletteCount)
            paletteIndex = 0;
        else if (paletteIndex < 0)
            paletteIndex = paletteCount - 1;

        switch (paletteIndex) {
            case 0:
                currentPalette = RainbowColors_p;
                break;
            case 1:
                currentPalette = RainbowStripeColors_p;
                break;
            case 2:
                currentPalette = OceanColors_p;
                break;
            case 3:
                currentPalette = CloudColors_p;
                break;
            case 4:
                currentPalette = ForestColors_p;
                break;
            case 5:
                currentPalette = PartyColors_p;
                break;
            case 6:
                setupGrayscalePalette();
                break;
            case HeatColorsPaletteIndex:
                currentPalette = HeatColors_p;
                break;
            case 8:
                currentPalette = LavaColors_p;
                break;
            case 9:
                setupIcePalette();
                break;
        }
    }

    void setupGrayscalePalette() {
        currentPalette = CRGBPalette16(CRGB::Black, CRGB::White);
    }

    void setupIcePalette() {
        currentPalette = CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White);
    }

    // Oscillators and Emitters

    // the oscillators: linear ramps 0-255
    byte osci[6];

    // sin8(osci) swinging between 0 to MATRIX_WIDTH - 1
    byte p[6];

// set the speeds (and by that ratios) of the oscillators here
    void MoveOscillators() {
    osci[0] = osci[0] + 5;
    osci[1] = osci[1] + 2;
    osci[2] = osci[2] + 3;
    osci[3] = osci[3] + 4;
    osci[4] = osci[4] + 1;
    if (osci[4] % 2 == 0)
        osci[5] = osci[5] + 1; // .5
    for (int i = 0; i < 4; i++) {
        p[i] = map8(sin8(osci[i]), 0, MATRIX_WIDTH - 1); //why? to keep the result in the range of 0-MATRIX_WIDTH (matrix size)
    }
}

    void PrepareFrame() {
    leds = (CRGB*) matrix.backBuffer();
}

    void ShowFrame() {
    matrix.swapBuffers();
    leds = (CRGB*) matrix.backBuffer();
    LEDS.countFPS();
}

// translates from x, y into an index into the LED array
    int XY(int x, int y) {
    if (y >= MATRIX_HEIGHT) { y = MATRIX_HEIGHT - 1; }
    if (y < 0) { y = 0; }
    if (x >= MATRIX_WIDTH) { x = MATRIX_WIDTH - 1; }
    if (x < 0) { x = 0; }

    return (y * MATRIX_WIDTH) + x;
}

// scale the brightness of the screenbuffer down
    void DimAll(byte value)
{
    for (int i = 0; i < ledCount; i++)
    {
        leds[i].nscale8(value);
    }
}

/*
Caleidoscope1 mirrors from source to A, B and C

y

| |
| B | C
|_______________
| |
|source | A
|_______________ x

*/
    void Caleidoscope1() {
    for (int x = 0; x < MATRIX_WIDTH / 2; x++) {
        for (int y = 0; y < MATRIX_HEIGHT / 2; y++) {
            leds[XY(MATRIX_WIDTH - 1 - x, y)] = leds[XY(x, y)]; // copy to A
            leds[XY(x, MATRIX_HEIGHT - 1 - y)] = leds[XY(x, y)]; // copy to B
            leds[XY(MATRIX_WIDTH - 1 - x, MATRIX_HEIGHT - 1 - y)] = leds[XY(x, y)]; // copy to C
        }
    }
}

/*
Caleidoscope2 rotates from source to A, B and C

y

| |
| C | B
|_______________
| |
|source | A
|_______________ x

*/
    void Caleidoscope2() {
    for (int x = 0; x < MATRIX_WIDTH / 2; x++) {
        for (int y = 0; y < MATRIX_HEIGHT / 2; y++) {
            leds[XY(MATRIX_WIDTH - 1 - x, y)] = leds[XY(y, x)]; // rotate to A
            leds[XY(MATRIX_WIDTH - 1 - x, MATRIX_HEIGHT - 1 - y)] = leds[XY(x, y)]; // rotate to B
            leds[XY(x, MATRIX_HEIGHT - 1 - y)] = leds[XY(y, x)]; // rotate to C
        }
    }
}

// adds the color of one quarter to the other 3
    void Caleidoscope3() {
    for (int x = 0; x < MATRIX_WIDTH / 2; x++) {
        for (int y = 0; y < MATRIX_HEIGHT / 2; y++) {
            leds[XY(MATRIX_WIDTH - 1 - x, y)] += leds[XY(y, x)]; // rotate to A
            leds[XY(MATRIX_WIDTH - 1 - x, MATRIX_HEIGHT - 1 - y)] += leds[XY(x, y)]; // rotate to B
            leds[XY(x, MATRIX_HEIGHT - 1 - y)] += leds[XY(y, x)]; // rotate to C
        }
    }
}

// add the complete screenbuffer 3 times while rotating
    // rotate and add the complete screenbuffer 3 times
    void Caleidoscope4() {
    for (int x = 0; x < MATRIX_WIDTH; x++) {
        for (int y = 0; y < MATRIX_HEIGHT; y++) {
            leds[XY(MATRIX_WIDTH - 1 - x, y)] += leds[XY(y, x)]; // rotate to A
            leds[XY(MATRIX_WIDTH - 1 - x, MATRIX_HEIGHT - 1 - y)] += leds[XY(x, y)]; // rotate to B
            leds[XY(x, MATRIX_HEIGHT - 1 - y)] += leds[XY(y, x)]; // rotate to C
        }
    }
}

// rotate, duplicate and copy over a triangle from first sector into the other half
// (crappy code)
    void Caleidoscope5() {
    int halfWidth = MATRIX_WIDTH / 2;
    int halfWidthMinus1 = halfWidth - 1;

    int j = halfWidthMinus1;
    int k = 0;

    for (int i = 1; i < halfWidth; i++) {
        for (int x = i; x < halfWidth; x++) {
            leds[XY(halfWidthMinus1 - x, j)] += leds[XY(x, k)];
        }

        j--;
        k++;
    }
}

    void Caleidoscope6() {
    for (int x = 1; x < MATRIX_WIDTH / 2; x++) {
        leds[XY(7 - x, 7)] = leds[XY(x, 0)];
    } //a
    for (int x = 2; x < MATRIX_WIDTH / 2; x++) {
        leds[XY(7 - x, 6)] = leds[XY(x, 1)];
    } //b
    for (int x = 3; x < MATRIX_WIDTH / 2; x++) {
        leds[XY(7 - x, 5)] = leds[XY(x, 2)];
    } //c
    for (int x = 4; x < MATRIX_WIDTH / 2; x++) {
        leds[XY(7 - x, 4)] = leds[XY(x, 3)];
    } //d
    for (int x = 5; x < MATRIX_WIDTH / 2; x++) {
        leds[XY(7 - x, 3)] = leds[XY(x, 4)];
    } //e
    for (int x = 6; x < MATRIX_WIDTH / 2; x++) {
        leds[XY(7 - x, 2)] = leds[XY(x, 5)];
    } //f
    for (int x = 7; x < MATRIX_WIDTH / 2; x++) {
        leds[XY(7 - x, 1)] = leds[XY(x, 6)];
    } //g
}

// create a square twister to the left or counter-clockwise
// x and y for center, r for radius
    void SpiralStream(int x, int y, int r, byte dimm) {
    for (int d = r; d >= 0; d--) { // from the outside to the inside
        for (int i = x - d; i <= x + d; i++) {
            leds[XY(i, y - d)] += leds[XY(i + 1, y - d)]; // lowest row to the right
            leds[XY(i, y - d)].nscale8(dimm);
        }
        for (int i = y - d; i <= y + d; i++) {
            leds[XY(x + d, i)] += leds[XY(x + d, i + 1)]; // right colum up
            leds[XY(x + d, i)].nscale8(dimm);
        }
        for (int i = x + d; i >= x - d; i--) {
            leds[XY(i, y + d)] += leds[XY(i - 1, y + d)]; // upper row to the left
            leds[XY(i, y + d)].nscale8(dimm);
        }
        for (int i = y + d; i >= y - d; i--) {
            leds[XY(x - d, i)] += leds[XY(x - d, i - 1)]; // left colum down
            leds[XY(x - d, i)].nscale8(dimm);
        }
    }
}

// expand everything within a circle
    void Expand(int centerX, int centerY, int radius, byte dimm) {
    if (radius == 0)
        return;

    int currentRadius = radius;

    while (currentRadius > 0) {
        int a = radius, b = 0;
        int radiusError = 1 - a;

        int nextRadius = currentRadius - 1;
        int nextA = nextRadius - 1, nextB = 0;
        int nextRadiusError = 1 - nextA;

        while (a >= b)
        {
            // move them out one pixel on the radius
            leds[XY(a + centerX, b + centerY)] = leds[XY(nextA + centerX, nextB + centerY)];
            leds[XY(b + centerX, a + centerY)] = leds[XY(nextB + centerX, nextA + centerY)];
            leds[XY(-a + centerX, b + centerY)] = leds[XY(-nextA + centerX, nextB + centerY)];
            leds[XY(-b + centerX, a + centerY)] = leds[XY(-nextB + centerX, nextA + centerY)];
            leds[XY(-a + centerX, -b + centerY)] = leds[XY(-nextA + centerX, -nextB + centerY)];
            leds[XY(-b + centerX, -a + centerY)] = leds[XY(-nextB + centerX, -nextA + centerY)];
            leds[XY(a + centerX, -b + centerY)] = leds[XY(nextA + centerX, -nextB + centerY)];
            leds[XY(b + centerX, -a + centerY)] = leds[XY(nextB + centerX, -nextA + centerY)];

            // dim them
            leds[XY(a + centerX, b + centerY)].nscale8(dimm);
            leds[XY(b + centerX, a + centerY)].nscale8(dimm);
            leds[XY(-a + centerX, b + centerY)].nscale8(dimm);
            leds[XY(-b + centerX, a + centerY)].nscale8(dimm);
            leds[XY(-a + centerX, -b + centerY)].nscale8(dimm);
            leds[XY(-b + centerX, -a + centerY)].nscale8(dimm);
            leds[XY(a + centerX, -b + centerY)].nscale8(dimm);
            leds[XY(b + centerX, -a + centerY)].nscale8(dimm);

            b++;
            if (radiusError < 0)
                radiusError += 2 * b + 1;
            else
            {
                a--;
                radiusError += 2 * (b - a + 1);
            }

            nextB++;
            if (nextRadiusError < 0)
                nextRadiusError += 2 * nextB + 1;
            else
            {
                nextA--;
                nextRadiusError += 2 * (nextB - nextA + 1);
            }
        }

        currentRadius--;
    }
}

// give it a linear tail to the right
    void StreamRight(byte scale, int fromX = 0, int toX = MATRIX_WIDTH, int fromY = 0, int toY = MATRIX_HEIGHT)
{
    for (int x = fromX + 1; x < toX; x++) {
        for (int y = fromY; y < toY; y++) {
            leds[XY(x, y)] += leds[XY(x - 1, y)];
            leds[XY(x, y)].nscale8(scale);
        }
    }
    for (int y = fromY; y < toY; y++)
        leds[XY(0, y)].nscale8(scale);
}

// give it a linear tail to the left
    void StreamLeft(byte scale, int fromX = MATRIX_WIDTH, int toX = 0, int fromY = 0, int toY = MATRIX_HEIGHT)
{
    for (int x = toX; x < fromX; x++) {
        for (int y = fromY; y < toY; y++) {
            leds[XY(x, y)] += leds[XY(x + 1, y)];
            leds[XY(x, y)].nscale8(scale);
        }
    }
    for (int y = fromY; y < toY; y++)
        leds[XY(0, y)].nscale8(scale);
}

// give it a linear tail downwards
    void StreamDown(byte scale)
{
    for (int x = 0; x < MATRIX_WIDTH; x++) {
        for (int y = 1; y < MATRIX_HEIGHT; y++) {
            leds[XY(x, y)] += leds[XY(x, y - 1)];
            leds[XY(x, y)].nscale8(scale);
        }
    }
    for (int x = 0; x < MATRIX_WIDTH; x++)
        leds[XY(x, 0)].nscale8(scale);
}

    // give it a linear tail upwards
    void StreamUp(byte scale)
    {
        for (int x = 0; x < MATRIX_WIDTH; x++) {
            for (int y = MATRIX_HEIGHT - 2; y >= 0; y--) {
                leds[XY(x, y)] += leds[XY(x, y + 1)];
                leds[XY(x, y)].nscale8(scale);
            }
        }
        for (int x = 0; x < MATRIX_WIDTH; x++)
            leds[XY(x, MATRIX_HEIGHT - 1)].nscale8(scale);
    }

    // give it a linear tail up and to the left
    void StreamUpAndLeft(byte scale)
    {
        for (int x = 0; x < MATRIX_WIDTH - 1; x++) {
            for (int y = MATRIX_HEIGHT - 2; y >= 0; y--) {
                leds[XY(x, y)] += leds[XY(x + 1, y + 1)];
                leds[XY(x, y)].nscale8(scale);
            }
        }
        for (int x = 0; x < MATRIX_WIDTH; x++)
            leds[XY(x, MATRIX_HEIGHT - 1)].nscale8(scale);
        for (int y = 0; y < MATRIX_HEIGHT; y++)
            leds[XY(MATRIX_WIDTH - 1, y)].nscale8(scale);
    }

    // give it a linear tail up and to the right
    void StreamUpAndRight(byte scale)
    {
        for (int x = 0; x < MATRIX_WIDTH - 1; x++) {
            for (int y = MATRIX_HEIGHT - 2; y >= 0; y--) {
                leds[XY(x + 1, y)] += leds[XY(x, y + 1)];
                leds[XY(x, y)].nscale8(scale);
            }
        }
        // fade the bottom row
        for (int x = 0; x < MATRIX_WIDTH; x++)
            leds[XY(x, MATRIX_HEIGHT - 1)].nscale8(scale);

        // fade the right column
        for (int y = 0; y < MATRIX_HEIGHT; y++)
            leds[XY(MATRIX_WIDTH - 1, y)].nscale8(scale);
    }

// just move everything one line down
    void MoveDown() {
    for (int y = MATRIX_HEIGHT - 1; y > 0; y--) {
        for (int x = 0; x < MATRIX_WIDTH; x++) {
            leds[XY(x, y)] = leds[XY(x, y - 1)];
        }
    }
}

// just move everything one line down
    void VerticalMoveFrom(int start, int end) {
    for (int y = end; y > start; y--) {
        for (int x = 0; x < MATRIX_WIDTH; x++) {
            leds[XY(x, y)] = leds[XY(x, y - 1)];
        }
    }
}

// copy the rectangle defined with 2 points x0, y0, x1, y1
// to the rectangle beginning at x2, x3
    void Copy(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2) {
    for (int y = y0; y < y1 + 1; y++) {
        for (int x = x0; x < x1 + 1; x++) {
            leds[XY(x + x2 - x0, y + y2 - y0)] = leds[XY(x, y)];
        }
    }
}

// rotate + copy triangle (MATRIX_WIDTH / 2*MATRIX_WIDTH / 2)
    void RotateTriangle() {
    for (int x = 1; x < MATRIX_WIDTH / 2; x++) {
        for (int y = 0; y < x; y++) {
            leds[XY(x, 7 - y)] = leds[XY(7 - x, y)];
        }
    }
}

// mirror + copy triangle (MATRIX_WIDTH / 2*MATRIX_WIDTH / 2)
    void MirrorTriangle() {
    for (int x = 1; x < MATRIX_WIDTH / 2; x++) {
        for (int y = 0; y < x; y++) {
            leds[XY(7 - y, x)] = leds[XY(7 - x, y)];
        }
    }
}

// draw static rainbow triangle pattern (MATRIX_WIDTH / 2xWIDTH / 2)
// (just for debugging)
    void RainbowTriangle() {
    for (int i = 0; i < MATRIX_WIDTH / 2; i++) {
        for (int j = 0; j <= i; j++) {
            Pixel(7 - i, j, i*j * 4);
        }
    }
}

    void BresenhamLine(int x0, int y0, int x1, int y1, byte colorIndex)
{
    BresenhamLine(x0, y0, x1, y1, ColorFromCurrentPalette(colorIndex));
}

    void BresenhamLine(int x0, int y0, int x1, int y1, CRGB color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    for (;;){
        leds[XY(x0, y0)] += color;
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 > dy) { err += dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

// write one pixel with the specified color from the current palette to coordinates
    void Pixel(int x, int y, uint8_t colorIndex) {
    leds[XY(x, y)] = ColorFromCurrentPalette(colorIndex);
}

    CRGB ColorFromCurrentPalette(uint8_t index = 0, uint8_t brightness = 255, TBlendType blendType = BLEND) {
    return ColorFromPalette(currentPalette, index, brightness, currentBlendType);
}

    CRGB HsvToRgb(uint8_t h, uint8_t s, uint8_t v) {
    CHSV hsv = CHSV(h, s, v);
    CRGB rgb;
    hsv2rgb_spectrum(hsv, rgb);
    return rgb;
}
};

#endif
