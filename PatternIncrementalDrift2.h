/*
*
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

#ifndef PatternIncrementalDrift2_H
#define PatternIncrementalDrift2_H

class PatternIncrementalDrift2 : public Drawable {
  private:

  public:
    PatternIncrementalDrift2() {
      name = (char *)"Incremental Drift Rose";
    }

    void start() {
    }

    uint8_t beatcos8( accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255)
    {
      if (lowest > highest) {
        uint8_t temp = lowest;
        lowest = highest;
        highest = temp;
      }

      uint8_t beat = beat8( beats_per_minute);
      uint8_t beatcos = cos8( beat);
      uint8_t rangewidth = highest - lowest;
      uint8_t scaledbeat = scale8( beatcos, rangewidth);
      uint8_t result = lowest + scaledbeat;
      return result;
    }

    unsigned int drawFrame() {
      uint8_t dim = beatsin8(2, 170, 250);
      blur2d(effects.leds, MATRIX_WIDTH, MATRIX_HEIGHT, dim);

      for (uint8_t i = 0; i < 32; i++)
      {
        CRGB color;

        uint8_t x = 0;
        uint8_t y = 0;

        if (i < 16) {
          x = beatcos8((i + 1) * 2, i, (MATRIX_WIDTH - 1) - i);
          y = beatsin8((i + 1) * 2, i, (MATRIX_HEIGHT - 1) - i);
          color = effects.ColorFromCurrentPalette(i * 16);
        }
        else
        {
          x = beatsin8((32 - i) * 2, (MATRIX_WIDTH - 1) - i, i);
          y = beatcos8((32 - i) * 2, (MATRIX_HEIGHT - 1) - i, i);
          color = effects.ColorFromCurrentPalette((31 - i) * 16);
        }

        effects.leds[XY(x, y)] += color;
      }

      return 0;
    }
};

#endif
