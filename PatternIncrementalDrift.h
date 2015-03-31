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

#ifndef PatternIncrementalDrift_H
#define PatternIncrementalDrift_H

class PatternIncrementalDrift : public Drawable {
  private:

  public:
    PatternIncrementalDrift() {
      name = (char *)"Incremental Drift";
    }

    void start() {
    }

    uint8_t beatcos8( accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255)
    {
      uint8_t beat = beat8( beats_per_minute);
      uint8_t beatsin = cos8( beat);
      uint8_t rangewidth = highest - lowest;
      uint8_t scaledbeat = scale8( beatsin, rangewidth);
      uint8_t result = lowest + scaledbeat;
      return result;
    }

    unsigned int drawFrame() {
      effects.DimAll(170);

      for (int i = 0; i < MATRIX_WIDTH / 2; i++)
      {
        CRGB color = effects.ColorFromCurrentPalette(i * 15);

        uint8_t x = beatsin8(i, MATRIX_CENTER_X - i, MATRIX_CENTER_X + i);
        uint8_t y = beatcos8(i, MATRIX_CENTER_Y - i, MATRIX_CENTER_Y + i);

        matrix.drawPixel(x, y, color);
      }

      return 0;
    }
};

#endif
