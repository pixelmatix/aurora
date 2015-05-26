/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from "Funky Clouds" by Stefan Petrick: https://gist.github.com/anonymous/876f908333cd95315c35
 * Portions of this code are adapted from "NoiseSmearing" by Stefan Petrick: https://gist.github.com/StefanPetrick/9ee2f677dbff64e3ba7a
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
extern uint16_t XY(uint8_t x, uint8_t y);

class Effects {
public:
    CRGB *leds;
    CRGB leds2[MATRIX_WIDTH * MATRIX_HEIGHT];

    // The coordinates for 3 16-bit noise spaces.
#define NOISE_NUM_LAYERS 1

    uint32_t noise_x[NOISE_NUM_LAYERS];
    uint32_t noise_y[NOISE_NUM_LAYERS];
    uint32_t noise_z[NOISE_NUM_LAYERS];
    uint32_t noise_scale_x[NOISE_NUM_LAYERS];
    uint32_t noise_scale_y[NOISE_NUM_LAYERS];

    uint8_t noise[NOISE_NUM_LAYERS][MATRIX_WIDTH][MATRIX_HEIGHT];

    uint8_t noisesmoothing;

    int ledCount = MATRIX_WIDTH * MATRIX_HEIGHT;

    // palettes
    static const int paletteCount = 11;
    int paletteIndex = -1;
    TBlendType currentBlendType = BLEND;
    CRGBPalette16 currentPalette;
    CRGBPalette16 targetPalette;
    char* currentPaletteName;

    static const int HeatColorsPaletteIndex = 7;

    void Setup() {
      currentPalette = RainbowColors_p;
      loadPalette(0);
      NoiseVariablesSetup();
    }

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
            targetPalette = RainbowColors_p;
            currentPaletteName = (char *)"Rainbow";
            break;
        case 1:
            targetPalette = RainbowStripeColors_p;
            currentPaletteName = (char *)"RainbowStripe";
            break;
        case 2:
            targetPalette = OceanColors_p;
            currentPaletteName = (char *)"Ocean";
            break;
        case 3:
            targetPalette = CloudColors_p;
            currentPaletteName = (char *)"Cloud";
            break;
        case 4:
            targetPalette = ForestColors_p;
            currentPaletteName = (char *)"Forest";
            break;
        case 5:
            targetPalette = PartyColors_p;
            currentPaletteName = (char *)"Party";
            break;
        case 6:
            setupGrayscalePalette();
            currentPaletteName = (char *)"Grey";
            break;
        case HeatColorsPaletteIndex:
            targetPalette = HeatColors_p;
            currentPaletteName = (char *)"Heat";
            break;
        case 8:
            targetPalette = LavaColors_p;
            currentPaletteName = (char *)"Lava";
            break;
        case 9:
            setupIcePalette();
            currentPaletteName = (char *)"Ice";
            break;
        case 10:
            setupRandomPalette3();
            currentPaletteName = (char *)"Random";
            break;
        }
    }

    void setPalette(String paletteName) {
        if (paletteName == "Rainbow")
            loadPalette(0);
        else if (paletteName == "RainbowStripe")
            loadPalette(1);
        else if (paletteName == "Ocean")
            loadPalette(2);
        else if (paletteName == "Cloud")
            loadPalette(3);
        else if (paletteName == "Forest")
            loadPalette(4);
        else if (paletteName == "Party")
            loadPalette(5);
        else if (paletteName == "Grayscale")
            loadPalette(6);
        else if (paletteName == "Heat")
            loadPalette(7);
        else if (paletteName == "Lava")
            loadPalette(8);
        else if (paletteName == "Ice")
            loadPalette(9);
        else if (paletteName == "Random")
            loadPalette(10);
    }

    void listPalettes() {
        Serial.println(F("{"));
        Serial.print(F("  \"count\": "));
        Serial.print(paletteCount);
        Serial.println(",");
        Serial.println(F("  \"results\": ["));

        String paletteNames[] = {
            "Rainbow",
            "RainbowStripe",
            "Ocean",
            "Cloud",
            "Forest",
            "Party",
            "Grayscale",
            "Heat",
            "Lava",
            "Ice",
            "Random"
        };

        for (int i = 0; i < paletteCount; i++) {
            Serial.print(F("    \""));
            Serial.print(paletteNames[i]);
            if (i == paletteCount - 1)
                Serial.println(F("\""));
            else
                Serial.println(F("\","));
        }

        Serial.println("  ]");
        Serial.println("}");
    }

    void setupGrayscalePalette() {
        targetPalette = CRGBPalette16(CRGB::Black, CRGB::White);
    }

    void setupIcePalette() {
        targetPalette = CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White);
    }

    void setupRandomPalette3() {
        targetPalette = CRGBPalette16(
            CHSV(random8(), 255, 0),
            CHSV(random8(), 255, 0),
            CHSV(random8(), 255, 255),
            CHSV(random8(), 255, 255),

            CHSV(random8(), 255, 0),
            CHSV(random8(), 255, 255),
            CHSV(random8(), 255, 255),
            CHSV(random8(), 255, 0),

            CHSV(random8(), 255, 0),
            CHSV(random8(), 255, 255),
            CHSV(random8(), 255, 255),
            CHSV(random8(), 255, 0),

            CHSV(random8(), 255, 255),
            CHSV(random8(), 255, 255),
            CHSV(random8(), 255, 0),
            CHSV(random8(), 255, 0));
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
#if (FASTLED_VERSION >= 3001000)
        nblendPaletteTowardPalette(currentPalette, targetPalette, 24);
#else
        currentPalette = targetPalette;
#endif

        matrix.swapBuffers();
        leds = (CRGB*) matrix.backBuffer();
        LEDS.countFPS();
    }

    // scale the brightness of the screenbuffer down
    void DimAll(byte value)
    {
        for (int i = 0; i < ledCount; i++)
        {
            leds[i].nscale8(value);
        }
    }


    // All the caleidoscope functions work directly within the screenbuffer (leds array).
    // Draw whatever you like in the area x(0-15) and y (0-15) and then copy it arround.

    // rotates the first 16x16 quadrant 3 times onto a 32x32 (+90 degrees rotation for each one)
    void Caleidoscope1() {
        for (int x = 0; x < MATRIX_CENTER_X; x++) {
            for (int y = 0; y < MATRIX_CENTER_Y; y++) {
                leds[XY(MATRIX_WIDTH - 1 - x, y)] = leds[XY(x, y)];
                leds[XY(MATRIX_WIDTH - 1 - x, MATRIX_HEIGHT - 1 - y)] = leds[XY(x, y)];
                leds[XY(x, MATRIX_HEIGHT - 1 - y)] = leds[XY(x, y)];
            }
        }
    }


    // mirror the first 16x16 quadrant 3 times onto a 32x32
    void Caleidoscope2() {
        for (int x = 0; x < MATRIX_CENTER_X; x++) {
            for (int y = 0; y < MATRIX_CENTER_Y; y++) {
                leds[XY(MATRIX_WIDTH - 1 - x, y)] = leds[XY(y, x)];
                leds[XY(x, MATRIX_HEIGHT - 1 - y)] = leds[XY(y, x)];
                leds[XY(MATRIX_WIDTH - 1 - x, MATRIX_HEIGHT - 1 - y)] = leds[XY(x, y)];
            }
        }
    }

    // copy one diagonal triangle into the other one within a 16x16
    void Caleidoscope3() {
        for (int x = 0; x <= MATRIX_CENTRE_X; x++) {
            for (int y = 0; y <= x; y++) {
                leds[XY(x, y)] = leds[XY(y, x)];
            }
        }
    }

    // copy one diagonal triangle into the other one within a 16x16 (90 degrees rotated compared to Caleidoscope3)
    void Caleidoscope4() {
        for (int x = 0; x <= MATRIX_CENTRE_X; x++) {
            for (int y = 0; y <= MATRIX_CENTRE_Y - x; y++) {
                leds[XY(MATRIX_CENTRE_Y - y, MATRIX_CENTRE_X - x)] = leds[XY(x, y)];
            }
        }
    }

    // copy one diagonal triangle into the other one within a 8x8
    void Caleidoscope5() {
        for (int x = 0; x < MATRIX_WIDTH / 4; x++) {
            for (int y = 0; y <= x; y++) {
                leds[XY(x, y)] = leds[XY(y, x)];
            }
        }

        for (int x = MATRIX_WIDTH / 4; x < MATRIX_WIDTH / 2; x++) {
            for (int y = MATRIX_HEIGHT / 4; y >= 0; y--) {
                leds[XY(x, y)] = leds[XY(y, x)];
            }
        }
    }

    void Caleidoscope6() {
        for (int x = 1; x < MATRIX_CENTER_X; x++) {
            leds[XY(7 - x, 7)] = leds[XY(x, 0)];
        } //a
        for (int x = 2; x < MATRIX_CENTER_X; x++) {
            leds[XY(7 - x, 6)] = leds[XY(x, 1)];
        } //b
        for (int x = 3; x < MATRIX_CENTER_X; x++) {
            leds[XY(7 - x, 5)] = leds[XY(x, 2)];
        } //c
        for (int x = 4; x < MATRIX_CENTER_X; x++) {
            leds[XY(7 - x, 4)] = leds[XY(x, 3)];
        } //d
        for (int x = 5; x < MATRIX_CENTER_X; x++) {
            leds[XY(7 - x, 3)] = leds[XY(x, 4)];
        } //e
        for (int x = 6; x < MATRIX_CENTER_X; x++) {
            leds[XY(7 - x, 2)] = leds[XY(x, 5)];
        } //f
        for (int x = 7; x < MATRIX_CENTER_X; x++) {
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

    // rotate + copy triangle (MATRIX_CENTER_X*MATRIX_CENTER_X)
    void RotateTriangle() {
        for (int x = 1; x < MATRIX_CENTER_X; x++) {
            for (int y = 0; y < x; y++) {
                leds[XY(x, 7 - y)] = leds[XY(7 - x, y)];
            }
        }
    }

    // mirror + copy triangle (MATRIX_CENTER_X*MATRIX_CENTER_X)
    void MirrorTriangle() {
        for (int x = 1; x < MATRIX_CENTER_X; x++) {
            for (int y = 0; y < x; y++) {
                leds[XY(7 - y, x)] = leds[XY(7 - x, y)];
            }
        }
    }

    // draw static rainbow triangle pattern (MATRIX_CENTER_XxWIDTH / 2)
    // (just for debugging)
    void RainbowTriangle() {
        for (int i = 0; i < MATRIX_CENTER_X; i++) {
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

    void NoiseVariablesSetup() {

        noisesmoothing = 200;
        for (int i = 0; i < NOISE_NUM_LAYERS; i++) {
            noise_x[i] = random16();
            noise_y[i] = random16();
            noise_z[i] = random16();
            noise_scale_x[i] = 6000;
            noise_scale_y[i] = 6000;
        }
    }

    void FillNoise(byte layer) {

        for (uint8_t i = 0; i < MATRIX_WIDTH; i++) {

            uint32_t ioffset = noise_scale_x[layer] * (i - MATRIX_CENTRE_X);

            for (uint8_t j = 0; j < MATRIX_HEIGHT; j++) {

                uint32_t joffset = noise_scale_y[layer] * (j - MATRIX_CENTRE_Y);

                byte data = inoise16(noise_x[layer] + ioffset, noise_y[layer] + joffset, noise_z[layer]) >> 8;

                uint8_t olddata = noise[layer][i][j];
                uint8_t newdata = scale8(olddata, noisesmoothing) + scale8(data, 256 - noisesmoothing);
                data = newdata;


                noise[layer][i][j] = data;
            }
        }
    }


#ifdef CONNECT_THE_DOTS
    byte point1_x = 0;
    byte point1_y = 0;
    byte point2_x = 0;
    byte point2_y = 0;

    int delta1_x = 0;
    int delta1_y = 0;
    int delta2_x = 0;
    int delta2_y = 0;
#endif

    void ShiftRow(byte row, int delta) {
        if (delta < 0)
            delta += MATRIX_WIDTH;

        for (int x = 0; x < MATRIX_WIDTH - delta; x++) {
            leds2[XY(x, row)] = leds[XY(x + delta, row)];
        }

        for (int x = MATRIX_WIDTH - delta; x < MATRIX_WIDTH; x++) {
            leds2[XY(x, row)] = leds[XY(x + delta - MATRIX_WIDTH, row)];
        }
    }

    void ShiftColumn(byte column, int delta) {
        if (delta < 0)
            delta += MATRIX_HEIGHT;

        for (int y = 0; y < MATRIX_HEIGHT - delta; y++) {
            leds2[XY(column, y)] = leds[XY(column, y + delta)];
        }
        for (int y = MATRIX_HEIGHT - delta; y < MATRIX_HEIGHT; y++) {
            leds2[XY(column, y)] = leds[XY(column, y + delta - MATRIX_HEIGHT)];
        }
    }

    void ShiftRowFractional(byte row, byte fractionalDelta) {
        CRGB PixelA;
        CRGB PixelB;

        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            PixelA = leds2[XY((x + 1) % MATRIX_WIDTH, row)];
            PixelB = leds2[XY(x, row)];

            // blend together
            PixelA %= 255 - fractionalDelta;
            PixelB %= fractionalDelta;

            leds[XY((x + 1) % MATRIX_WIDTH, row)] = PixelA + PixelB;
        }
    }

    void ShiftColumnFractional(byte column, byte fractionalDelta) {
        CRGB PixelA;
        CRGB PixelB;

        for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
            PixelA = leds2[XY(column, (y + 1) % MATRIX_HEIGHT)];
            PixelB = leds2[XY(column, y)];

            PixelA %= 255 - fractionalDelta;
            PixelB %= fractionalDelta;

            leds[XY(column, (y + 1) % MATRIX_HEIGHT)] = PixelA + PixelB;
        }
    }

    // radius is maximum number of pixels to shift rows +/- from the center based on the noise value for that row
    // offset is additional number of pixels to shift rows in the positive direction (left)
    void MoveFractionalNoiseX(byte radius, int offset = 0) {
        for (int y = 0; y < MATRIX_HEIGHT; y++) {

            int noiseX = noise[0][0][y];

            /* move bitmap to the right edge of the radius, or to the left of the radius
            minus one pixel (saving room for the fractional move later).
            movement off center is based on the high byte of noiseX */
            int delta = offset + (radius) -((radius * 2 * noiseX) / 256);

#ifdef CONNECT_THE_DOTS
            if (y == point1_y)
                delta1_x = -delta;

            if (y == point2_y)
                delta2_x = -delta;
#endif
            if (delta < 0)
                delta += MATRIX_WIDTH;

            ShiftRow(y, delta);
        }


        // blur pixels, shifting them between 0 and 1 pixels to the left then copy back to leds buffer
        for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
            byte fractions = noise[0][0][y] * radius * 2 % 256;

            ShiftRowFractional(y, fractions);
        }
    }

    // radius is maximum number of pixels to shift columns +/- from the center based on the noise value for that column
    // offset is additional number of pixels to shift columns in the positive direction (up)
    void MoveFractionalNoiseY(byte radius = 8, int offset = 0) {
        for (int x = 0; x < MATRIX_WIDTH; x++) {

            int noiseY = noise[0][x][0];

            int delta = offset + (radius) -((radius * 2 * noiseY) / 256);

#ifdef CONNECT_THE_DOTS
            if (x == point1_x)
                delta1_y = -delta;

            if (x == point2_x)
                delta2_y = -delta;
#endif
            ShiftColumn(x, delta);
        }

        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            byte fractions = noise[0][x][0] * radius * 2 % 256;

            ShiftColumnFractional(x, fractions);
        }
    }

    void NoiseSmearWithRadius(byte radius, int offset = 0) {
        MoveFractionalNoiseX(radius, offset);
        MoveFractionalNoiseY(radius, offset);
    }

    // this pattern draws two points to the screen based on sin/cos if a counter
    // (comment out NoiseSmearWithRadius to see pattern of pixels)
    // these pixels are smeared by a large radius, giving a lot of movement
    // the image is dimmed before each drawing to not saturate the screen with color
    // the smear has an offset so the pixels usually have a trail leading toward the upper left
    void MultipleStream() {
        static unsigned long counter = 0;
#if 1
        // this counter lets put delays between each frame and still get the same animation
        counter++;
#else
        // this counter updates in real time and can't be slowed down for debugging
        counter = millis() / 10;
#endif

        byte x1 = 4 + sin8(counter * 2) / 10;
        byte x2 = 8 + sin8(counter * 2) / 16;
        byte y2 = 8 + cos8((counter * 2) / 3) / 16;

#ifdef CONNECT_THE_DOTS
        byte y1 = 4 + cos8(counter * 2) / 10;
        
        point1_x = x1;
        point1_y = y1;

        // draw line from previous position of point to current posistion
        // TODO: draw line that wraps around edges
        pSmartMatrix->drawLine(point1_x, point1_y, point1_x + delta1_x, point1_y + delta1_y, { (0xff * 230) / 256, 0xff * 230 / 256, 0x00 });

        point2_x = x2;
        point2_y = y2;

        pSmartMatrix->drawLine(point2_x, point2_y, point2_x + delta2_x, point2_y + delta2_y, { (0xff * 230) / 256, 0x00, 0x00 });

        // dim the image more as lines add more light than just pixels
        DimAll(230);
#else
        DimAll(249);
#endif

        leds[XY(x1, x2)] = 0xFFFF00;
        leds[XY(x2, y2)] = 0xFF0000;

        // Noise
        noise_x[0] += 1000;
        noise_y[0] += 1000;
        noise_scale_x[0] = 4000;
        noise_scale_y[0] = 4000;
        FillNoise(0);

        // this pattern smears with an offset added so the pixels usually have a trail going to the upper left
        NoiseSmearWithRadius(8, 1);
    }
};

#endif
