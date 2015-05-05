/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from "Funky Clouds" by Stefan Petrick: https://gist.github.com/anonymous/876f908333cd95315c35
 * Portions of this code are adapted from "MSGEQ7 controlled Noise" by Stefan Petrick: https://gist.github.com/anonymous/449974b8515df3e1aecc
 * Portions of this code are adapted from "FunkyNoise1-5" by Stefan Petrick: https://gist.github.com/anonymous/29388692c2a030cb8738
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

#ifndef AudioPatterns_H
#define AudioPatterns_H

class AudioPatterns : public Playlist {
  public:
    bool paused = true;

    bool drawPeaks = true;

    bool drawMiniAnalyzer = true;

    byte theta = 0;
    byte hueoffset = 0;

    typedef void (AudioPatterns::*AudioPatternFunctionPointer)(void);

    AudioPatternFunctionPointer currentItem;

    static const int PATTERN_COUNT = 48;

    AudioPatternFunctionPointer shuffledItems[PATTERN_COUNT];

    AudioPatternFunctionPointer items[PATTERN_COUNT] = {
      &AudioPatterns::analyzerColumns,
      &AudioPatterns::analyzerPixels,
      &AudioPatterns::lineChart,
      &AudioPatterns::viz2,
      &AudioPatterns::viz10,
      &AudioPatterns::radialPixels,
      &AudioPatterns::radialCircles,
      &AudioPatterns::radar,
      &AudioPatterns::gauge,
      &AudioPatterns::gauge2,
      &AudioPatterns::paletteSmear,
      &AudioPatterns::bounce,
      &AudioPatterns::incrementalDrift,
      &AudioPatterns::incrementalDrift2,
      &AudioPatterns::pendulumWave,
      &AudioPatterns::fire,
      &AudioPatterns::staticMiddlePixelsWithNoiseSmearing,
      &AudioPatterns::analyzerPixelsWithNoiseSmearing,
      &AudioPatterns::analyzerColumnsWithNoiseSmearing,
      &AudioPatterns::randomEmittersWithNoiseSmearing,
      &AudioPatterns::lineChartWithNoiseSmearing,
      &AudioPatterns::analyzerPixelsWithOneLargeSpiral,
      &AudioPatterns::analyzerPixelsWithFiveSpirals,
      &AudioPatterns::analyzerPixelsWithStreamDown120,
      &AudioPatterns::analyzerPixelsWithStreamUpAndLeft120,
      &AudioPatterns::analyzerPixelsWithStreamUp120,
      &AudioPatterns::analyzerPixelsWithStreamUpAndRight120,
      &AudioPatterns::analyzerPixelsMirroredVerticallyWithFiveSpirals,
      &AudioPatterns::analyzerColumnsWithOneLargeSpiralStream,
      &AudioPatterns::analyzerColumnsWithVerticalStream170,
      &AudioPatterns::fallingSpectrogramPaletteSpectrum,
      &AudioPatterns::fallingWavySpectrogram,
      &AudioPatterns::fallingSpectrogram,
      &AudioPatterns::randomEmitters,
      &AudioPatterns::linesFromCenterToOutside,
      &AudioPatterns::linesFromOutsideToCenter,
      &AudioPatterns::analyzerPixelsX4,
      &AudioPatterns::areaChart,
      &AudioPatterns::lineChartWithHorizontalStream,
      &AudioPatterns::lineChartWithSpiral,
      &AudioPatterns::lineChartWithSpirals,
      &AudioPatterns::FunkyNoise1,
      &AudioPatterns::FunkyNoise2,
      &AudioPatterns::FunkyNoise3,
      &AudioPatterns::FunkyNoise4,
      &AudioPatterns::FunkyNoise5,
      &AudioPatterns::circles,
      &AudioPatterns::boxes,
    };

    void circles() {
      effects.SpiralStream(15, 15, 16, 120);

      for (int i = 0; i < bandCount; i++) {
        int level = levels[i];
        if (drawPeaks)
          level = peaks[i];

        if (level / 32 < 2)
          continue;

        CRGB color = effects.ColorFromCurrentPalette(level / 4);
        //            matrix.drawCircle(15, 15, i * 2 + 0, color);
        matrix.drawCircle(15, 15, i * 2 + 1, color);
      }
    }

    void boxes() {
      effects.SpiralStream(7, 7, 8, 120);
      effects.SpiralStream(7, 23, 8, 120);
      effects.SpiralStream(15, 15, 8, 120);
      effects.SpiralStream(23, 7, 8, 120);
      effects.SpiralStream(23, 23, 8, 120);

      for (int i = 0; i < bandCount; i++) {
        int level = levels[i];
        if (drawPeaks)
          level = peaks[i];

        if (level / 32 < 2)
          continue;

        CRGB color = effects.ColorFromCurrentPalette(level / 4);
        //matrix.drawRectangle(i, i, (MATRIX_WIDTH - 1) - i, (MATRIX_HEIGHT - 1) - i, color);

        matrix.drawRectangle(
          15 - i * 2, 15 - i * 2, // upper left
          16 + i * 2, 16 + i * 2, // lower right
          color);
      }
    }

    // have an eye on the 7 frequency bands
    void miniAnalyzer() {
      // dim area
      for (int i = 0; i < bandCount; i++) {
        for (int j = 0; j < 4; j++) {
          effects.leds[XY((MATRIX_WIDTH - bandCount) + i, (MATRIX_HEIGHT - 1) - j)].nscale8(127);
        }
      }
      // draw lines according to the band levels
      for (int i = 0; i < bandCount; i++) {
        matrix.drawLine((MATRIX_WIDTH - bandCount) + i, MATRIX_HEIGHT - 1, (MATRIX_WIDTH - bandCount) + i, (MATRIX_HEIGHT - 1) - levels[i] / 256, effects.ColorFromCurrentPalette((-i) * 15));
      }
    }

    void analyzerPixels() {
      matrix.fillScreen({ 0, 0, 0 });
      drawAnalyzerPixels();
    }

    void analyzerPixelsWithStreamDown120() {
      effects.StreamDown(120);
      drawAnalyzerPixels();
    }

    void analyzerPixelsWithStreamUpAndLeft120() {
      effects.StreamUpAndLeft(120);
      drawAnalyzerPixels();
    }

    void analyzerPixelsWithStreamUp120() {
      effects.StreamUp(120);
      drawAnalyzerPixels();
    }

    void analyzerPixelsWithStreamUpAndRight120() {
      effects.StreamUpAndRight(120);
      drawAnalyzerPixels();
    }

    void analyzerPixelsWithOneLargeSpiral() {
      effects.SpiralStream(15, 15, 16, 120);
      drawAnalyzerPixels();
    }

    void analyzerPixelsWithFiveSpirals() {
      effects.SpiralStream(7, 7, 8, 120);
      effects.SpiralStream(7, 23, 8, 120);
      effects.SpiralStream(15, 15, 8, 120);
      effects.SpiralStream(23, 7, 8, 120);
      effects.SpiralStream(23, 23, 8, 120);
      drawAnalyzerPixels();
    }

    void analyzerPixelsWithNoiseSmearing() {
      effects.DimAll(235);

      drawAnalyzerPixels();

      effects.standardNoiseSmearing();
    }

    void drawAnalyzerPixels() {
      for (int x = 0; x < MATRIX_WIDTH; x++) {
        int level = interpolatedLevels[x / horizontalPixelsPerBand];

        uint8_t colorIndex = level / 4;
        uint8_t y = MATRIX_HEIGHT - level / MATRIX_HEIGHT;
        effects.Pixel(x, y, colorIndex);
      }
    }

    void staticMiddlePixelsWithNoiseSmearing() {
      effects.DimAll(235);

      for (int i = 0; i < bandCount; i++) {
        int level = levels[i];
        if (drawPeaks)
          level = peaks[i];

        effects.Pixel(i * 4 + 3, MATRIX_CENTRE_Y, level / 4);
      }

      effects.standardNoiseSmearing();
    }

    void areaChart() {
      effects.StreamDown(145);
      analyzerLinesByIntensity();
    }

    void lineChart() {
      matrix.fillScreen({ 0, 0, 0 });
      analyzerLinesByIntensity();
    }

    void lineChartWithNoiseSmearing() {
      effects.DimAll(235);

      analyzerLinesByIntensity();

      effects.standardNoiseSmearing();
    }

    void lineChartWithSpiral() {
      effects.SpiralStream(15, 15, 16, 120);
      analyzerLinesByIntensity();
    }

    void lineChartWithSpirals() {
      effects.SpiralStream(7, 7, 8, 120);
      effects.SpiralStream(7, 23, 8, 120);
      effects.SpiralStream(15, 15, 8, 120);
      effects.SpiralStream(23, 7, 8, 120);
      effects.SpiralStream(23, 23, 8, 120);
      analyzerLinesByIntensity();
    }

    void lineChartWithHorizontalStream() {
      effects.StreamRight(120);
      analyzerLinesByIntensity();
    }

    void analyzerColumns() {
      matrix.fillScreen({ 0, 0, 0 });
      analyzerColumnsByIntensity();
    }

    void analyzerColumnsWithNoiseSmearing() {
      effects.DimAll(235);

      analyzerColumnsByIntensity();

      effects.standardNoiseSmearing();
    }

    void analyzerColumnsWithOneLargeSpiralStream() {
      effects.SpiralStream(15, 15, 16, 120);
      analyzerColumnsByIntensity();
    }

    void analyzerColumnsWithVerticalStream170() {
      effects.StreamDown(170);
      analyzerColumnsByIntensity();
    }

    void analyzerColumnsByBand() {
      for (int x = 0; x < MATRIX_WIDTH; x++) {
        int level = interpolatedLevels[x / horizontalPixelsPerBand];

        CRGB color = effects.ColorFromCurrentPalette(x * 40);
        matrix.drawLine(x, MATRIX_HEIGHT - level / MATRIX_HEIGHT, x, 31, color);
      }
    }

    void analyzerColumnsByIntensity() {
      for (int x = 0; x < MATRIX_WIDTH; x++) {
        int level = interpolatedLevels[x / horizontalPixelsPerBand];

        CRGB color = effects.ColorFromCurrentPalette(level / 4);

        matrix.drawLine(x, MATRIX_HEIGHT - level / MATRIX_HEIGHT, x, 31, color);
      }
    }

    void analyzerLinesByIntensity() {
      for (int i = 0; i < interpolatedBandCount - 1; i++) {
        int level = interpolatedLevels[i];
        int nextLevel = interpolatedLevels[i + 1];

        CRGB color = effects.ColorFromCurrentPalette(level / 4);

        uint8_t y = MATRIX_HEIGHT - level / MATRIX_HEIGHT;
        uint8_t nextY = MATRIX_HEIGHT - nextLevel / MATRIX_HEIGHT;

        y = y >= MATRIX_HEIGHT ? MATRIX_HEIGHT - 1 : y;
        nextY = nextY >= MATRIX_HEIGHT ? MATRIX_HEIGHT - 1 : nextY;

        matrix.drawLine(2 * i, y, 2 + 2 * i, nextY, color);
      }
    }

    void analyzerPixelsMirroredVerticallyWithFiveSpirals() {
      effects.SpiralStream(7, 7, 8, 120);
      effects.SpiralStream(7, 23, 8, 120);
      effects.SpiralStream(15, 15, 8, 120);
      effects.SpiralStream(23, 7, 8, 120);
      effects.SpiralStream(23, 23, 8, 120);

      for (int x = 0; x < MATRIX_WIDTH; x++) {
        int level = interpolatedLevels[x / horizontalPixelsPerBand];

        uint8_t colorIndex = level / 4;

        uint8_t y = (MATRIX_HEIGHT / 2) - level / (MATRIX_HEIGHT * 2);
        effects.Pixel(x, y, colorIndex);

        y = (MATRIX_HEIGHT / 2) + level / (MATRIX_HEIGHT * 2);
        effects.Pixel(x, y, colorIndex);
      }
    }

    void fallingSpectrogramPaletteSpectrum() {
      effects.MoveDown();

      for (int x = 0; x < MATRIX_WIDTH; x++) {
        int level = interpolatedLevels[x / horizontalPixelsPerBand];

        if (level <= 8)
          level = 0;

        CRGB color = effects.ColorFromCurrentPalette(x * (240 / MATRIX_WIDTH), level / 4); // CHSV(i * 36, 255, levels[i] / 4); // brightness should be divided by 4
        effects.leds[XY(x, 0)] = color;
      }
    }

    void fallingSpectrogram() {
      effects.MoveDown();

      for (int x = 0; x < MATRIX_WIDTH; x++) {
        int level = interpolatedLevels[x / horizontalPixelsPerBand];

        if (level <= 8)
          level = 0;

        CRGB color;
        if (effects.paletteIndex < 2) // invert the first two palettes
          color = effects.ColorFromCurrentPalette(205 - (level / 4 - 205));
        else
          color = effects.ColorFromCurrentPalette(level / 4);

        effects.leds[XY(x, 0)] = color;
      }
    }

    void fallingWavySpectrogram() {
      effects.MoveDown();
      for (int i = 0; i < 4; i++) {
        if (i % 2 == 0) {
          effects.StreamRight(130, 0, MATRIX_WIDTH, i * 8, i * 8 + 7);
        }
        else {
          effects.StreamLeft(130, MATRIX_WIDTH, 0, i * 8, i * 8 + 7);
        }
      }

      for (int x = 0; x < MATRIX_WIDTH; x++) {
        int level = interpolatedLevels[x / horizontalPixelsPerBand];

        if (level <= 8)
          level = 0;

        CRGB color = effects.ColorFromCurrentPalette(x * (240 / MATRIX_WIDTH), level / 4); // CHSV(i * 36, 255, levels[i] / 4);
        effects.leds[XY(x, 0)] = color;
      }
    }

    // 2 frequencies linked to dot emitters in a spiral mandala (MSGEQtest2)
    void viz2() {
      if (levels[0] > 500) {
        effects.Pixel(0, 0, 255 + hueoffset);
        effects.Pixel(1, 1, 255 + hueoffset);
      }
      if (levels[2] > 200) {
        effects.Pixel(3, 3, 85 + hueoffset);
      }
      if (levels[6] > 200) {
        effects.Pixel(5, 5, 170 + hueoffset);
      }
      effects.SpiralStream(8, 8, 8, 128);
      effects.Caleidoscope1();

      EVERY_N_MILLIS(100) {
        hueoffset++;
      }
    }

    // basedrum/snare linked to emitters (AudioSpiral)
    void viz10() {
      if (levels[1] > 500) {
        effects.Pixel(0, 0, hueoffset);
        effects.Pixel(1, 1, hueoffset);
      }
      if (levels[4] > 500) {
        effects.Pixel(random8(16), random8(16), 128 + hueoffset);
      }

      effects.DimAll(254);
      effects.SpiralStream(8, 8, 8, 128);
      effects.Caleidoscope1();

      EVERY_N_MILLIS(100) {
        hueoffset++;
      }
    }

    // random emitters with spirals
    void randomEmitters() {
      effects.DimAll(250);
      effects.SpiralStream(15, 15, 15, 130);
      effects.SpiralStream(8, 8, 8, 122);
      effects.SpiralStream(23, 23, 6, 122);

      drawRandomEmitters();
    }

    void randomEmittersWithNoiseSmearing() {
      effects.DimAll(235);

      drawRandomEmitters();

      effects.standardNoiseSmearing();
    }

    void drawRandomEmitters() {
      for (int i = 0; i < bandCount; i++) {
        int level = levels[i];
        if (drawPeaks)
          level = peaks[i];

        if (level / 32 <= 2)
          continue;

        uint8_t colorIndex = i * 40;

        effects.leds[XY(random(0, MATRIX_WIDTH - 1), random(0, MATRIX_HEIGHT - 1))] = effects.ColorFromCurrentPalette(colorIndex, level / 4);
      }
    }

    // single channel (levels), one line per band starting from the center
    // radiating outwards at equal angles with length and color based on level
    void linesFromCenterToOutside() {
      effects.DimAll(235);

      drawLinesFromCenterToOutside(2);

      effects.standardNoiseSmearing();
    }

    // single channel (levels), one line per band starting from the outside
    // radiating inwards at equal angles with length and color based on level
    void linesFromOutsideToCenter() {
      effects.DimAll(235);

      drawLinesFromOutsideToCenter(2);

      effects.standardNoiseSmearing();
    }

    // single channel (levels), X lines per channel starting from the center
    // radiating outwards at equal angles with length and color based on channel level
    void drawLinesFromCenterToOutside(int linesPerBand) {
      int x0 = 15;
      int y0 = 15;
      if (linesPerBand * bandCount > 32)
        linesPerBand = bandCount / 32;
      float degreesPerLine = 360.0 / (bandCount * linesPerBand);
      float angle = 0.0;
      for (int band = 0; band < bandCount; band++) {
        int level = levels[band];
        if (drawPeaks)
          level = peaks[band];

        for (int i = 0; i < linesPerBand; i++) {
          CRGB color = effects.ColorFromCurrentPalette(level / 4);
          float length = level / 64.0 - 1.0;
          if (length > 0.0) {
            float radians = radians(angle);
            int x1 = x0 + length * sin(radians);
            int y1 = y0 + length * cos(radians);
            matrix.drawLine(x0, y0, x1, y1, color);
          }
          angle += degreesPerLine;
        }
      }
    }

    // single channel (levels), X lines per channel starting from the outside
    // radiating inwards at equal angles with length and color based on channel level
    void drawLinesFromOutsideToCenter(int linesPerBand) {
      int x0 = 15;
      int y0 = 15;
      if (linesPerBand * bandCount > 32)
        linesPerBand = bandCount / 32;
      float degreesPerLine = 360.0 / (bandCount * linesPerBand);
      float angle = 0.0;
      for (int band = 0; band < bandCount; band++) {
        int level = levels[band];
        if (drawPeaks)
          level = peaks[band];

        for (int i = 0; i < linesPerBand; i++) {
          CRGB color = effects.ColorFromCurrentPalette(level / 4);
          float length = level / 64.0 - 1.0;
          if (length > 0.0) {
            float radians = radians(angle);
            int x1 = x0 + 16 * sin(radians);
            int y1 = y0 + 16 * cos(radians);
            int x2 = x1 - length * sin(radians);
            int y2 = y1 - length * cos(radians);
            matrix.drawLine(x1, y1, x2, y2, color);
          }
          angle += degreesPerLine;
        }
      }
    }

    // analyzer x 4 (as showed on youtube) (MSGEQtest4)
    void analyzerPixelsX4() {
      effects.Caleidoscope2();
      effects.DimAll(240);
      for (int x = 0; x < MATRIX_WIDTH / 2; x++) {
        int level = interpolatedLevels[x / (horizontalPixelsPerBand / 2)];
        effects.Pixel(x, (MATRIX_HEIGHT / 2) - level / (MATRIX_HEIGHT * 2), x * (240 / MATRIX_WIDTH));
      }
    }

    //// spectrum mandala, color linked to 160Hz band (MSGEQtest7)
    //void viz7() {
    //    for (int i = 0; i < bandCount; i++) {
    //        effects.Pixel(14 - i * 2, 16 - levels[i] / 64, i * 10 + levels[1] / 8);
    //        effects.Pixel(15 - i * 2, 16 - levels[i] / 64, i * 10 + levels[1] / 8);
    //    }
    //    effects.Caleidoscope5();
    //    effects.Caleidoscope1();
    //    effects.DimAll(240);
    //}

    //// spectrum mandala, color linked to osci (MSGEQtest8)
    //void viz8() {
    //    effects.MoveOscillators();
    //    for (int i = 0; i < bandCount; i++) {
    //        effects.Pixel(14 - i * 2, 16 - levels[i] / 64, i * 10 + effects.osci[1]);
    //        effects.Pixel(15 - i * 2, 16 - levels[i] / 64, i * 10 + effects.osci[1]);
    //    }
    //    effects.Caleidoscope5();
    //    effects.Caleidoscope2();
    //    effects.DimAll(240);
    //}


    // The 32bit version of our coordinates
    uint16_t x;
    uint16_t y;
    uint16_t z;

    // We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
    // use the z-axis for "time".  speed determines how fast time moves forward.  Try
    // 1 for a very slow moving effect, or 60 for something that ends up looking like
    // water.
    // uint16_t speed = 1; // almost looks like a painting, moves very slowly
    //uint16_t speed = 1; // a nice starting speed, mixes well with a scale of 100
    // uint16_t speed = 33;
    // uint16_t speed = 100; // wicked fast!

    // Scale determines how far apart the pixels in our noise matrix are.  Try
    // changing these values around to see how it affects the motion of the display.  The
    // higher the value of scale, the more "zoomed out" the noise iwll be.  A value
    // of 1 will be so zoomed in, you'll mostly see solid colors.

    // uint16_t scale = 1; // mostly just solid colors
    // uint16_t scale = 4011; // very zoomed out and shimmery
    uint16_t scale = 50;

    // calculate noise matrix
    // x and y define the lower left point
    void FillNoise() {
      for (int i = 0; i < MATRIX_WIDTH; i++) {
        int ioffset = scale * i;
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
          int joffset = scale * j;
          noise[0][i][j] = inoise8(x + ioffset, y + joffset, z);
        }
      }
    }

    // calculate noise matrix
    // x and y define the center
    void FillNoiseCentral() {
      for (int i = 0; i < MATRIX_WIDTH; i++) {
        int ioffset = scale * (i - 8);
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
          int joffset = scale * (j - 8);
          noise[0][i][j] = inoise8(x + ioffset, y + joffset, z);
        }
      }
    }

    int band5 = (int) ((float) bandCount * 0.75);

    void FunkyNoise1() {
      int level0 = levels[0] / 4;
      if (drawPeaks)
        level0 = peaks[0] / 4;

      int level1 = levels[1] / 4;
      if (drawPeaks)
        level1 = peaks[1] / 4;

      int level5 = levels[band5] / 4;
      if (drawPeaks)
        level5 = peaks[5] / 4;

      scale = level5 / 4;
      FillNoiseCentral();
      for (int i = 0; i < MATRIX_WIDTH; i++) {
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
          effects.leds[XY(i, j)] =
            effects.ColorFromCurrentPalette(
              // color controlled by lowest basedrum + noisevalue
              level0 + noise[0][i][j],
              // brightness controlled by kickdrum
              level1);
        }
      }

      if (drawMiniAnalyzer)
        miniAnalyzer();
    }

    void FunkyNoise2() {
      int level0 = levels[0] / 4;
      if (drawPeaks)
        level0 = peaks[0] / 4;

      int level1 = levels[1] / 4;
      if (drawPeaks)
        level1 = peaks[1] / 4;

      // zoom factor set by inversed band 0 (63-0)
      // 3 added to avoid maximal zoom = plain color
      scale = 3 + 63 - level0 / 4;
      // move one step in the noise space when basedrum is present
      // = slowly change the pattern while the beat goes
      if (level0 > 128) z++;
      // calculate the noise array
      FillNoiseCentral();
      // map the noise
      for (int i = 0; i < MATRIX_WIDTH; i++) {
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
          effects.leds[XY(i, j)] =
            effects.ColorFromCurrentPalette(
              // hue controlled by noise and shifted by band 0
              // 40 added to end redish
              40 + noise[0][i][j] + level0 / 4,
              // brightness controlled by the average of band 0 and 1
              // 20 to ensure a minimum brigness
              // = limiting contrast
              20 + (level0 + level1) / 2);
        }
      }

      if (drawMiniAnalyzer)
        miniAnalyzer();
    }

    void FunkyNoise3() {
      int level0 = levels[0] / 4;
      if (drawPeaks)
        level0 = peaks[0] / 4;

      int level3 = levels[3] / 4;
      if (drawPeaks)
        level3 = peaks[3] / 4;

      // fix zoom factor
      scale = 50;
      // move one step in the noise space when basedrum is present
      // = slowly change the pattern while the beat goes
      if (level0 > 128) z = z + 10;
      // x any y is defining the position in the noise space
      x = level3 / 3;
      y = level0 / 3;
      // calculate the noise array
      FillNoiseCentral();
      // map the noise
      for (int i = 0; i < MATRIX_WIDTH; i++) {
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
          effects.leds[XY(i, j)] =
            effects.ColorFromCurrentPalette(
              // hue controlled by noise and shifted by band 0
              // 40 added to end redish
              40 + noise[0][i][j] + level0 / 4,
              // brightness controlled by noise
              noise[0][i][j]);
        }
      }

      if (drawMiniAnalyzer)
        miniAnalyzer();
    }

    void FunkyNoise4() {
      int level0 = levels[0] / 4;
      if (drawPeaks)
        level0 = peaks[0] / 4;

      int level5 = levels[band5] / 4;
      if (drawPeaks)
        level5 = peaks[band5] / 4;

      // fix zoom factor
      scale = 50;
      // position in the noise space depending on band 5
      // = change of the pattern
      z = level5 / 2;
      // x scrolling through
      // = horizontal movement
      x = x + 50;
      // y controlled by lowest band
      // = jumping of the pattern
      y = level0;
      // calculate the noise array
      FillNoiseCentral();
      // map the noise
      for (int i = 0; i < MATRIX_WIDTH; i++) {
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
          effects.leds[XY(i, j)] =
            effects.ColorFromCurrentPalette(
              // hue controlled by noise and shifted by band 0
              // 40 added to end redish
              40 + noise[0][i][j] + level0 / 4,
              // brightness controlled by noise
              noise[0][i][j]);
        }
      }

      if (drawMiniAnalyzer)
        miniAnalyzer();
    }

    void FunkyNoise5() {
      int level0 = levels[0] / 4;
      if (drawPeaks)
        level0 = peaks[0] / 4;

      int level1 = levels[1] / 4;
      if (drawPeaks)
        level1 = peaks[1] / 4;

      // dynamic zoom: average of band 0 and 1
      scale = 128 - (level0 + level1) / 4;
      // position in the noise space depending on x, y and z
      // z slowly scrolling
      z++;
      // x static
      // y scrolling
      y = y + 20;
      // calculate the noise array
      // x any y are this time defining THE CENTER
      FillNoiseCentral();
      // map the noise
      for (int i = 0; i < MATRIX_WIDTH; i++) {
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
          effects.leds[XY(i, j)] =
            effects.ColorFromCurrentPalette(
              // hue controlled by noise and shifted by band 0
              // 40 added to end redish
              120 + noise[0][i][j] * 2,
              // brightness controlled by noise
              noise[0][i][j]);
        }
      }

      if (drawMiniAnalyzer)
        miniAnalyzer();
    }

    void fire() {
      // cooling: How much does the air cool as it rises?
      // Less cooling = taller flames.  More cooling = shorter flames.
      // Default 55, suggested range 20-100
      int cooling = 100;

      for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
        uint8_t bandIndex = x / 5;

        if (bandIndex >= bandCount)
          bandIndex = bandCount - 1;

        int level = levels[bandIndex];
        if (drawPeaks)
          level = peaks[bandIndex];

        // Step 1.  Cool down every cell a little
        for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
          uint16_t xy = XY(x, y);
          heat[xy] = qsub8(heat[xy], random(0, ((cooling * 10) / MATRIX_HEIGHT) + 2));
        }

        // Step 2.  Heat from each cell drifts 'up' and diffuses a little
        for (int y = 0; y < MATRIX_HEIGHT; y++) {
          heat[XY(x, y)] = (heat[XY(x, y + 1)] + heat[XY(x, y + 2)] + heat[XY(x, y + 2)]) / 3;
        }

        // Step 2.  ignite new 'sparks' of heat based on audio level
        if (random(255) < (uint8_t)(level / 5)) {
          // int x = (p[0] + p[1] + p[2]) / 3;

          int xy = XY(x, MATRIX_HEIGHT - 1);
          heat[xy] = qadd8(heat[xy], level / 32);
        }

        // Step 4.  Map from heat cells to LED colors
        for (int y = 0; y < MATRIX_HEIGHT; y++) {
          int xy = XY(x, y);
          byte colorIndex = heat[xy];

          // Recommend that you use values 0-240 rather than
          // the usual 0-255, as the last 15 colors will be
          // 'wrapping around' from the hot end to the cold end,
          // which looks wrong.
          colorIndex = scale8(colorIndex, 240);

          // override color 0 to ensure a black background?
          if (colorIndex == 0)
            effects.leds[xy] = CRGB::Black;
          else
            effects.leds[xy] = effects.ColorFromCurrentPalette(colorIndex);
        }
      }
    }

    void pendulumWave() {
      effects.DimAll(170);

      for (int x = 0; x < MATRIX_WIDTH; x++)
      {
        uint8_t bandIndex = x / 4;

        if (bandIndex >= bandCount)
          bandIndex = bandCount - 1;

        int level = levels[bandIndex];
        if (drawPeaks)
          level = peaks[bandIndex];

        uint8_t y = beatsin8(x + MATRIX_WIDTH, 0, MATRIX_HEIGHT);
        matrix.drawPixel(x, y, effects.ColorFromCurrentPalette(level / 4));
      }
    }

    void incrementalDrift() {
      uint8_t dim = beatsin8(2, 230, 250);
      effects.DimAll(dim);

      for (int i = 0; i < MATRIX_WIDTH / 2; i++)
      {
        uint8_t bandIndex = i / 2;

        if (bandIndex >= bandCount)
          bandIndex = bandCount - 1;

        int level = levels[bandIndex];
        if (drawPeaks)
          level = peaks[bandIndex];

        uint8_t x = beatcos8((16 - i) * 2, MATRIX_CENTER_X - i, MATRIX_CENTER_X + i);
        uint8_t y = beatsin8((16 - i) * 2, MATRIX_CENTER_Y - i, MATRIX_CENTER_Y + i);

        CRGB color = effects.ColorFromCurrentPalette(level / 4);

        matrix.drawPixel(x, y, color);
      }
    }

    void incrementalDrift2() {
      uint8_t dim = beatsin8(2, 170, 250);
      effects.DimAll(dim);

      for (uint8_t i = 0; i < 32; i++)
      {
        uint8_t x = 0;
        uint8_t y = 0;

        uint8_t bandIndex = 0;

        if (i < 16) {
          x = beatcos8((i + 1) * 2, i, (MATRIX_WIDTH - 1) - i);
          y = beatsin8((i + 1) * 2, i, (MATRIX_HEIGHT - 1) - i);
          bandIndex = i / 2;
        }
        else
        {
          x = beatsin8((32 - i) * 2, (MATRIX_WIDTH - 1) - i, i);
          y = beatcos8((32 - i) * 2, (MATRIX_HEIGHT - 1) - i, i);
          bandIndex = (31 - i) / 2;
        }

        if (bandIndex >= bandCount)
          bandIndex = bandCount - 1;

        int level = levels[bandIndex];
        if (drawPeaks)
          level = peaks[bandIndex];

        CRGB color = effects.ColorFromCurrentPalette(level / 4);

        matrix.drawPixel(x, y, color);
      }
    }

    PVector gravity = PVector(0, 0.0250);
    PVector impulse = PVector(0, 0);
    boolean startedAttractor = false;
    void bounce() {
      const int count = 32;

      if (!startedAttractor) {
        startedAttractor = true;

        int direction = random(0, 2);
        if (direction == 0)
          direction = -1;

        for (int i = 0; i < count; i++) {
          Boid boid = Boid(i, MATRIX_HEIGHT - 1);
          boid.velocity.x = 0;
          boid.velocity.y = 0;
          boid.maxforce = 10;
          boid.maxspeed = 10;
          boids[i] = boid;
        }
      }

      // dim all pixels on the display
      effects.DimAll(170);

      for (int i = 0; i < count; i++) {
        Boid boid = boids[i];
        boid.velocity.x = 0;
        boid.location.x = i;
        boid.applyForce(gravity);

        uint8_t bandIndex = i / 5;

        int level = levels[bandIndex];
        if (drawPeaks)
          level = peaks[bandIndex];

        if (boid.location.y == MATRIX_HEIGHT - 1) {
          impulse.y = - (float)random(level * .75, level) / 960.0F;
          boid.applyForce(impulse);
        }

        boid.update();

        boid.bounceOffBorders(0.2);

        boids[i] = boid;

        matrix.drawPixel(boid.location.x, boid.location.y, effects.ColorFromCurrentPalette(((MATRIX_HEIGHT - 1) - boid.location.y) * 7));
      }
    }

    void paletteSmear() {
      effects.DimAll(170);

      // draw a rainbow color palette
      for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
          int level = interpolatedLevels[x / horizontalPixelsPerBand];

          effects.leds[XY(x, y)] += effects.ColorFromCurrentPalette(level / 5, y * 8 + 7);
        }
      }

      effects.standardNoiseSmearing();
    }

    void gauge() {
      blur2d( effects.leds, MATRIX_WIDTH, MATRIX_HEIGHT, 10);
      // effects.DimAll(250);

      for (int i = 0; i < bandCount; i++) {
        int level = levels[(bandCount - 1) - i];
        if (drawPeaks)
          level = peaks[(bandCount - 1) - i];

        uint8_t offset = i * 2;

        uint8_t x = mapsin8(256 - level / 4, offset, (MATRIX_WIDTH - 1) - offset);
        uint8_t y = mapcos8(256 - level / 4, offset, (MATRIX_HEIGHT - 1) - offset);
        effects.leds[XY(x, y)] += effects.ColorFromCurrentPalette(level / 5, 64);

        offset = i * 2 + 1;

        x = mapsin8(256 - level / 4, offset, (MATRIX_WIDTH - 1) - offset);
        y = mapcos8(256 - level / 4, offset, (MATRIX_HEIGHT - 1) - offset);
        effects.leds[XY(x, y)] += effects.ColorFromCurrentPalette(level / 5, 64);
      }
    }

    void gauge2() {
      blur2d( effects.leds, MATRIX_WIDTH, MATRIX_HEIGHT, 10);
      // effects.DimAll(250);

      for (int i = 0; i < bandCount; i++) {
        int level = levels[(bandCount - 1) - i];
        if (drawPeaks)
          level = peaks[(bandCount - 1) - i];

        uint8_t offset = i * 2;

        uint8_t x = mapsin8(256 - level / 4, offset, (MATRIX_WIDTH - 1) - offset);
        uint8_t y = mapcos8(256 - level / 4, offset, (MATRIX_HEIGHT - 1) - offset);
        effects.leds[XY(x, y)] += effects.ColorFromCurrentPalette(offset * 18, level / 4);

        offset = i * 2 + 1;

        x = mapsin8(256 - level / 4, offset, (MATRIX_WIDTH - 1) - offset);
        y = mapcos8(256 - level / 4, offset, (MATRIX_HEIGHT - 1) - offset);
        effects.leds[XY(x, y)] += effects.ColorFromCurrentPalette(offset * 18, level / 4);
      }
    }

    void radialTest() {
      effects.DimAll(254);

      for (int offset = 0; offset < MATRIX_CENTER_X; offset++) {
        byte hue = 255 - (offset * 16 + hueoffset);
        CRGB color = effects.ColorFromCurrentPalette(hue);
        uint8_t x = mapcos8(theta, offset, (MATRIX_WIDTH - 1) - offset);
        uint8_t y = mapsin8(theta, offset, (MATRIX_HEIGHT - 1) - offset);
        uint16_t xy = XY(x, y);
        effects.leds[xy] = color;

        EVERY_N_MILLIS(25) {
          theta += 2;
          hueoffset += 1;
        }
      }
    }

    void radar() {
      effects.DimAll(254);

      for (int i = 0; i < bandCount; i++) {
        int level = levels[(bandCount - 1) - i];
        if (drawPeaks)
          level = peaks[i];

        uint8_t brightness = level / 4;
        if (brightness < 32) brightness = 32;

        uint8_t offset = i * 2;
        CRGB color = effects.ColorFromCurrentPalette(offset * 18, brightness);
        uint8_t x = mapcos8(theta, offset, (MATRIX_WIDTH - 1) - offset);
        uint8_t y = mapsin8(theta, offset, (MATRIX_HEIGHT - 1) - offset);
        uint16_t xy = XY(x, y);
        effects.leds[xy] = color;

        offset = i * 2 + 1;
        color = effects.ColorFromCurrentPalette(offset * 18, brightness);
        x = mapcos8(theta, offset, (MATRIX_WIDTH - 1) - offset);
        y = mapsin8(theta, offset, (MATRIX_HEIGHT - 1) - offset);
        xy = XY(x, y);
        effects.leds[xy] = color;
      }

      EVERY_N_MILLIS(50) {
        theta += 2;
      }
    }

    void radialCircles() {
      effects.DimAll(230);

      uint8_t x = 0;
      uint8_t y = 0;
      //      uint8_t brightness = 0;
      int level = 0;
      CRGB color;
      byte localtheta = 0;

      for (int i = 0; i < bandCount; i++) {
        level = levels[i];
        if (drawPeaks)
          level = peaks[i];

        //        brightness = level / 4;
        //        if (brightness < 32) brightness = 32;

        // offset = i * 2 + 1;
        // color = effects.ColorFromCurrentPalette(offset * 18, brightness);
        color = effects.ColorFromCurrentPalette(level / 5);
        localtheta = theta + (i * 2) * (256 / 14);
        x = mapcos8(localtheta, 2, MATRIX_WIDTH - 2);
        y = mapsin8(localtheta, 2, MATRIX_HEIGHT - 2);
        matrix.fillCircle(x, y, 3, color);

        // offset = i * 2 + 3;
        // color = effects.ColorFromCurrentPalette(offset * 18, brightness);
        color = effects.ColorFromCurrentPalette(level / 5);
        localtheta = theta + (i * 2 + 1) * (256 / 14);
        x = mapcos8(localtheta, 2, MATRIX_WIDTH - 2);
        y = mapsin8(localtheta, 2, MATRIX_HEIGHT - 2);
        matrix.fillCircle(x, y, 3, color);
      }

      EVERY_N_MILLIS(1) {
        theta += 1;
      }

      effects.standardNoiseSmearing();
    }

    boolean peakhandled = false;

    void radialPixels() {
      effects.DimAll(230);

      uint8_t x = 0;
      uint8_t y = 0;
      uint8_t brightness = 0;
      int level = 0;
      CRGB color;
      byte localtheta = 0;

      for (int i = 0; i < bandCount; i++) {
        level = levels[i];
        if (drawPeaks)
          level = peaks[i];

        if (i == 0 && level > 768) {
          theta -= 1;
        }

        brightness = level / 16;
        if (brightness < 16) brightness = 16;

        color = effects.ColorFromCurrentPalette(level / 5);
        localtheta = theta - (i * 2) * (256 / 14);
        x = mapcos8(localtheta, 4, MATRIX_WIDTH - 4);
        y = mapsin8(localtheta, 4, MATRIX_HEIGHT - 4);
        effects.leds[XY(x, y)] += color;
        x = mapcos8(localtheta, 12, MATRIX_WIDTH - 12);
        y = mapsin8(localtheta, 12, MATRIX_HEIGHT - 12);
        effects.leds[XY(x, y)] += color;

        color = effects.ColorFromCurrentPalette(level / 5);
        localtheta = theta - (i * 2 + 1) * (256 / 14);
        x = mapcos8(localtheta, 4, MATRIX_WIDTH - 4);
        y = mapsin8(localtheta, 4, MATRIX_HEIGHT - 4);
        effects.leds[XY(x, y)] += color;
        x = mapcos8(localtheta, 12, MATRIX_WIDTH - 12);
        y = mapsin8(localtheta, 12, MATRIX_HEIGHT - 12);
        effects.leds[XY(x, y)] += color;
      }

      effects.standardNoiseSmearing();
    }

    int currentIndex = 0;

    int getCurrentIndex() {
      return currentIndex;
    }

    AudioPatterns() {
      this->currentItem = items[currentIndex];

      // add the items to the shuffledItems array
      for (int a = 0; a < PATTERN_COUNT; a++) {
        shuffledItems[a] = items[a];
      }

      shuffleItems();
    }

    void moveRandom(int step) {
      currentIndex += step;

      if (currentIndex >= PATTERN_COUNT) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = PATTERN_COUNT - 1;

      if (effects.paletteIndex == effects.RandomPaletteIndex)
        effects.RandomPalette();

      currentItem = shuffledItems[currentIndex];
    }

    void shuffleItems() {
      for (int a = 0; a < PATTERN_COUNT; a++)
      {
        int r = random(a, PATTERN_COUNT);
        AudioPatternFunctionPointer temp = shuffledItems[a];
        shuffledItems[a] = shuffledItems[r];
        shuffledItems[r] = temp;
      }
    }

    char* Drawable::name = (char *)"Audio Patterns";

    void move(int step) {
      currentIndex += step;

      if (currentIndex >= PATTERN_COUNT) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = PATTERN_COUNT - 1;

      if (effects.paletteIndex == effects.RandomPaletteIndex)
        effects.RandomPalette();

      currentItem = items[currentIndex];
    }

    unsigned int drawFrame() {
      ReadAudio();

      (this->*currentItem)();
      return 0;
    }
};

#endif
