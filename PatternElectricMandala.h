/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from "Funky Noise" by Stefan Petrick: https://github.com/StefanPetrick/FunkyNoise
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

#ifndef PatternElectricMandala_H

class PatternElectricMandala : public Drawable {
private:

    // can be used for palette rotation
    // "colorshift"
    byte colorshift;

    // The coordinates for 16-bit noise spaces.
#define NUM_LAYERS 1

    uint32_t x[NUM_LAYERS];
    uint32_t y[NUM_LAYERS];
    uint32_t z[NUM_LAYERS];
    uint32_t scale_x[NUM_LAYERS];
    uint32_t scale_y[NUM_LAYERS];

    // used for the random based animations
    int16_t dx;
    int16_t dy;
    int16_t dz;
    int16_t dsx;
    int16_t dsy;

    // a 3dimensional array used to store the calculated 
    // values of the different noise planes
    uint8_t noise[NUM_LAYERS][MATRIX_WIDTH][MATRIX_HEIGHT];

    uint8_t noisesmoothing;

    // Fill the x/y array with 16-bit noise values 
    void FillNoise(byte layer) {

        for (uint8_t i = 0; i < MATRIX_WIDTH; i++) {

            uint32_t ioffset = scale_x[layer] * (i - MATRIX_CENTRE_X);

            for (uint8_t j = 0; j < MATRIX_HEIGHT; j++) {

                uint32_t joffset = scale_y[layer] * (j - MATRIX_CENTRE_Y);

                byte data = inoise16(x[layer] + ioffset, y[layer] + joffset, z[layer]) >> 8;

                uint8_t olddata = noise[layer][i][j];
                uint8_t newdata = scale8(olddata, noisesmoothing) + scale8(data, 256 - noisesmoothing);
                data = newdata;


                noise[layer][i][j] = data;
            }
        }
    }

    // show just one layer
    void ShowLayer(byte layer, byte colorrepeat) {
        for (uint8_t i = 0; i < MATRIX_WIDTH; i++) {
            for (uint8_t j = 0; j < MATRIX_HEIGHT; j++) {

                uint8_t color = noise[layer][i][j];

                uint8_t   bri = color;

                // assign a color depending on the actual palette
                CRGB pixel = ColorFromPalette(effects.currentPalette, colorrepeat * (color + colorshift), bri);

                effects.leds[XY(i, j)] = pixel;
            }
        }
    }

public:
    PatternElectricMandala() {
        name = (char *)"ElectricMandala";
    }

    void start() {
        // set to reasonable values to avoid a black out
        colorshift = 0;
        noisesmoothing = 200;

        // just any free input pin
        //random16_add_entropy(analogRead(18));

        // fill coordinates with random values
        // set zoom levels
        for (int i = 0; i < NUM_LAYERS; i++) {
            x[i] = random16();
            y[i] = random16();
            z[i] = random16();
            scale_x[i] = 6000;
            scale_y[i] = 6000;
        }
        // for the random movement
        dx = random8();
        dy = random8();
        dz = random8();
        dsx = random8();
        dsy = random8();
    }

    unsigned int drawFrame() {
#if FASTLED_VERSION >= 3001000
        // a new parameter set every 15 seconds
        EVERY_N_SECONDS(15) {
            //SetupRandomPalette3();
            dy = random16(500) - 250; // random16(2000) - 1000 is pretty fast but works fine, too
            dx = random16(500) - 250;
            dz = random16(500) - 250;
            scale_x[0] = random16(10000) + 2000;
            scale_y[0] = random16(10000) + 2000;
        }
#endif

        y[0] += dy;
        x[0] += dx;
        z[0] += dz;

        FillNoise(0);
        ShowLayer(0, 1);

        effects.Caleidoscope3();
        effects.Caleidoscope1();

        return 0;
    }
};

#endif
