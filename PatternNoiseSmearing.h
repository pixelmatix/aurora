/*
* Aurora: https://github.com/pixelmatix/aurora
* Copyright (c) 2014 Jason Coon
*
* Portions of this code are adapted from "Noise Smearing" by Stefan Petrick: https://gist.githubusercontent.com/embedded-creations/5cd47d83cb0e04f4574d/raw/ebf6a82b4755d55cfba3bf6598f7b19047f89daf/NoiseSmearing.ino
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

#ifndef PatternNoiseSmearing_H
#define PatternNoiseSmearing_H

class PatternMultipleStream : public Drawable {
public:
    PatternMultipleStream() {
        name = (char *)"MultipleStream";
    }

    void start() {
        effects.NoiseVariablesSetup();
    }

    // this pattern draws two points to the screen based on sin/cos if a counter
    // (comment out NoiseSmearWithRadius to see pattern of pixels)
    // these pixels are smeared by a large radius, giving a lot of movement
    // the image is dimmed before each drawing to not saturate the screen with color
    // the smear has an offset so the pixels usually have a trail leading toward the upper left
    unsigned int drawFrame() {
        static unsigned long counter = 0;
#if 0
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
        effects.DimAll(230);
#else
        effects.DimAll(249);
#endif

        effects.leds[XY(x1, x2)] = 0xFFFF00;
        effects.leds[XY(x2, y2)] = 0xFF0000;

        // Noise
        effects.noise_x[0] += 1000;
        effects.noise_y[0] += 1000;
        effects.noise_scale_x[0] = 4000;
        effects.noise_scale_y[0] = 4000;
        effects.FillNoise(0);

        // this pattern smears with an offset added so the pixels usually have a trail going to the upper left
        effects.NoiseSmearWithRadius(8, 1);

        return 0;
    }
};

class PatternMultipleStream2 : public Drawable {
public:
    PatternMultipleStream2() {
        name = (char *)"MultipleStream2";
    }

    void start() {
        effects.NoiseVariablesSetup();
    }

    unsigned int drawFrame() {
        effects.DimAll(230);

        byte xx = 4 + sin8(millis() / 9) / 10;
        byte yy = 4 + cos8(millis() / 10) / 10;
        effects.leds[XY(xx, yy)] += 0x0000FF;

        xx = 8 + sin8(millis() / 10) / 16;
        yy = 8 + cos8(millis() / 7) / 16;
        effects.leds[XY(xx, yy)] += 0xFF0000;

        effects.leds[XY(15, 15)] += 0xFFFF00;

        effects.noise_x[0] += 1000;
        effects.noise_y[0] += 1000;
        effects.noise_z[0] += 1000;
        effects.noise_scale_x[0] = 4000;
        effects.noise_scale_y[0] = 4000;
        effects.FillNoise(0);

        effects.NoiseSmearWithRadius(2);

        return 0;
    }
};

class PatternMultipleStream3 : public Drawable {
public:
    PatternMultipleStream3() {
        name = (char *)"MultipleStream3";
    }

    void start() {
        effects.NoiseVariablesSetup();
    }

    unsigned int drawFrame() {
        //CLS();
        effects.DimAll(235);

        for (uint8_t i = 3; i < 32; i = i + 4) {
            effects.leds[XY(i, 15)] += CHSV(i * 2, 255, 255);
        }

        // Noise
        effects.noise_x[0] += 1000;
        effects.noise_y[0] += 1000;
        effects.noise_z[0] += 1000;
        effects.noise_scale_x[0] = 4000;
        effects.noise_scale_y[0] = 4000;
        effects.FillNoise(0);

        effects.NoiseSmearWithRadius(2);

        return 1;
    }
};

class PatternMultipleStream4 : public Drawable {
public:
    PatternMultipleStream4() {
        name = (char *)"MultipleStream4";
    }

    void start() {
        effects.NoiseVariablesSetup();
    }

    unsigned int drawFrame() {

        //CLS();
        effects.DimAll(235);

        effects.leds[XY(15, 15)] += CHSV(millis(), 255, 255);


        // Noise
        effects.noise_x[0] += 1000;
        effects.noise_y[0] += 1000;
        effects.noise_scale_x[0] = 4000;
        effects.noise_scale_y[0] = 4000;
        effects.FillNoise(0);

        effects.NoiseSmearWithRadius(2);

        return 0;
    }
};

class PatternMultipleStream5 : public Drawable {
public:
    PatternMultipleStream5() {
        name = (char *)"MultipleStream5";
    }

    void start() {
        effects.NoiseVariablesSetup();
    }

    unsigned int drawFrame() {

        //CLS();
        effects.DimAll(235);


        for (uint8_t i = 3; i < 32; i = i + 4) {
            effects.leds[XY(i, 31)] += CHSV(i * 2, 255, 255);
        }

        // Noise
        effects.noise_x[0] += 1000;
        effects.noise_y[0] += 1000;
        effects.noise_z[0] += 1000;
        effects.noise_scale_x[0] = 4000;
        effects.noise_scale_y[0] = 4000;
        effects.FillNoise(0);

        // apply an offset to 1 to keep streams moving toward the top of the scren
        effects.MoveFractionalNoiseX(2);
        effects.MoveFractionalNoiseY(2, 1);

        return 0;
    }
};

class PatternMultipleStream8 : public Drawable {
public:
    PatternMultipleStream8() {
        name = (char *)"MultipleStream8";
    }

    void start() {
        effects.NoiseVariablesSetup();
    }

    unsigned int drawFrame() {
        effects.DimAll(230);

        // draw grid of rainbow dots on top of the dimmed image
        for (uint8_t y = 1; y < 32; y = y + 6) {
            for (uint8_t x = 1; x < 32; x = x + 6) {

                effects.leds[XY(x, y)] += CHSV((x*y) / 4, 255, 255);
            }
        }

        // Noise
        effects.noise_x[0] += 1000;
        effects.noise_y[0] += 1000;
        effects.noise_z[0] += 1000;
        effects.noise_scale_x[0] = 4000;
        effects.noise_scale_y[0] = 4000;
        effects.FillNoise(0);

        // move image (including newly drawn dot) within +/-2 pixels of original position
        effects.NoiseSmearWithRadius(2);

        return 0;
    }
};

class PatternPaletteSmear : public Drawable {
public:
    void start() {
        effects.NoiseVariablesSetup();
    }

    unsigned int drawFrame() {
        effects.DimAll(170);

        // draw a rainbow color palette
        for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
            for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
                effects.leds[XY(x, y)] += effects.ColorFromCurrentPalette(x * 8, y * 8 + 7);
            }
        }
        
        // Noise
        effects.noise_x[0] += 1000;
        effects.noise_y[0] += 1000;
        effects.noise_scale_x[0] = 4000;
        effects.noise_scale_y[0] = 4000;
        effects.FillNoise(0);

        // this pattern smears with an offset added so the pixels usually have a trail going to the upper left
        effects.NoiseSmearWithRadius(8, 1);

        return 0;
    }
};

#endif