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

#ifndef PatternEffects_H
#define PatternEffects_H

class PatternGhost : public Drawable {
public:
    PatternGhost() {
        name = (char *)"Ghost";
    }

    unsigned int drawFrame() {
        effects.StreamRight(125);

        effects.MoveOscillators();

        //if (random(255) < 120)
        effects.Pixel((effects.p[2] + effects.p[0] + effects.p[1]) / 3, (effects.p[1] + effects.p[3] + effects.p[2]) / 3, effects.osci[3]);

        effects.SpiralStream(MATRIX_CENTER_X - 5, MATRIX_CENTER_Y - 5, 5, 140);
        effects.SpiralStream(MATRIX_CENTER_X + 5, MATRIX_CENTER_Y + 5, 5, 140);

        return 15;
    }
};

// 2 oscillators flying arround one ;)
class PatternDots1 : public Drawable {
private:
    CRGB color1 = effects.HsvToRgb(1, 255, 255);
    CRGB color2 = effects.HsvToRgb(1, 255, 150);

public:
    PatternDots1() {
        name = (char *)"Dots1";
    }

    unsigned int drawFrame() {
        effects.StreamRight(125);
        effects.MoveOscillators();
        
        //2 lissajous dots
        effects.Pixel(effects.p[0], effects.p[1], beatsin8(3, 0, 255));
        effects.Pixel(effects.p[2], effects.p[3], beatsin8(3, 0, 255));
        
        //average of the coordinates
        effects.Pixel((effects.p[2] + effects.p[0]) / 2, (effects.p[1] + effects.p[3]) / 2, beatsin8(6, 0, 255));

        return 20;
    }
};

// x and y based on 3 sine waves
class PatternDots2 : public Drawable {
public:
    PatternDots2() {
        name = (char *)"Dots2";
    }

    unsigned int drawFrame() {

        effects.StreamRight(125);
        effects.MoveOscillators();
        effects.Pixel((effects.p[2] + effects.p[0] + effects.p[1]) / 3, (effects.p[1] + effects.p[3] + effects.p[2]) / 3, effects.osci[3]);

        return 20;
    }
};

// red, 4 spirals, one dot emitter
// demonstrates SpiralStream and Caleidoscope
// (psychedelic)
class PatternSlowMandala : public Drawable {
private:
    int i = 0;
    int j = 0;
public:
    PatternSlowMandala() {
        name = (char *)"SlowMandala";
    }

    unsigned int drawFrame() {
        effects.leds[XY(i, j)] = effects.ColorFromCurrentPalette(255); // COLOR_RED;
        effects.SpiralStream(8, 8, 8, 127);
        effects.Caleidoscope1();

        i++;
        if (i >= MATRIX_CENTER_X) {
            i = 0;
            j++;
            if (j >= MATRIX_CENTER_Y) {
                j = 0;
            }
        }

        return 50;
    }
};

class PatternMandala8 : public Drawable {
public:
    PatternMandala8() {
        name = (char *)"Mandala8";
    }

    // 2 lissajou dots *2 *4
    unsigned int drawFrame() {
        effects.MoveOscillators();
        effects.Pixel(effects.p[0], effects.p[1], effects.osci[2]);
        effects.Pixel(effects.p[2], effects.p[3], effects.osci[3]);
        effects.Caleidoscope5();
        effects.Caleidoscope2();
        effects.StreamRight(110);

        return 0;
    }
};

#endif