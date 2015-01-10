/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from Andrew: http://pastebin.com/f22bfe94d
 * which, in turn, was "Adapted from the Life example on the Processing.org site"
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

#ifndef PatternLife_H
#define PatternLife_H

class Cell {
public:
  byte alive : 1;
  byte hue: 7;  
  byte brightness;
};

class PatternLife : public Drawable {
private:
    Cell world[MATRIX_WIDTH][MATRIX_HEIGHT][2];
    long density = 50;
    int generation = 0;

    void randomFillWorld() {
        for (int i = 0; i < MATRIX_WIDTH; i++) {
            for (int j = 0; j < MATRIX_HEIGHT; j++) {
                if (random(100) < density) {
                    world[i][j][0].alive = 1;
                }
                else {
                    world[i][j][0].alive = 0;
                }
                world[i][j][0].brightness = 255;
                world[i][j][0].hue = 0;
                world[i][j][1].brightness = 255;
                world[i][j][1].hue = 0;
                world[i][j][1].alive = 0;
            }
        }
    }

    int neighbours(int x, int y) {
        return (world[(x + 1) % MATRIX_WIDTH][y][0].alive) +
            (world[x][(y + 1) % MATRIX_HEIGHT][0].alive) +
            (world[(x + MATRIX_WIDTH - 1) % MATRIX_WIDTH][y][0].alive) +
            (world[x][(y + MATRIX_HEIGHT - 1) % MATRIX_HEIGHT][0].alive) +
            (world[(x + 1) % MATRIX_WIDTH][(y + 1) % MATRIX_HEIGHT][0].alive) +
            (world[(x + MATRIX_WIDTH - 1) % MATRIX_WIDTH][(y + 1) % MATRIX_HEIGHT][0].alive) +
            (world[(x + MATRIX_WIDTH - 1) % MATRIX_WIDTH][(y + MATRIX_HEIGHT - 1) % MATRIX_HEIGHT][0].alive) +
            (world[(x + 1) % MATRIX_WIDTH][(y + MATRIX_HEIGHT - 1) % MATRIX_HEIGHT][0].alive);
    }

public:
    unsigned int drawFrame() {
        if (generation == 0) {
            matrix.fillScreen(CRGB(CRGB::Black));

            randomFillWorld();
        }

        // Display current generation
        for (int i = 0; i < MATRIX_WIDTH; i++) {
            for (int j = 0; j < MATRIX_HEIGHT; j++) {
                effects.leds[effects.XY(i, j)] = effects.ColorFromCurrentPalette(world[i][j][0].hue * 2, world[i][j][0].brightness);
            }
        }

        // Birth and death cycle
        for (int x = 0; x < MATRIX_WIDTH; x++) {
            for (int y = 0; y < MATRIX_HEIGHT; y++) {
                // Default is for cell to stay the same
                world[x][y][1] = world[x][y][0];
                if (world[x][y][1].brightness > 0 && world[x][y][1].alive == 0)
                  world[x][y][1].brightness *= 0.9;
                int count = neighbours(x, y);
                if (count == 3 && world[x][y][0].alive == 0) {
                    // A new cell is born
                    world[x][y][1].alive = 1;
                    world[x][y][1].hue+=4;
                    world[x][y][1].brightness = 255;
                }
                if ((count < 2 || count > 3) && world[x][y][0].alive == 1) {
                    // Cell dies
                    world[x][y][1].alive = 0;
                }
            }
        }

        // Copy next generation into place
        for (int x = 0; x < MATRIX_WIDTH; x++) {
            for (int y = 0; y < MATRIX_HEIGHT; y++) {
                world[x][y][0] = world[x][y][1];
            }
        }

        generation++;
        if (generation >= 256)
            generation = 0;

        return 60;
    }
};

#endif
