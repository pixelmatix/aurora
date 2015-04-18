/*
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

#ifndef PatternBitmap_H
#define PatternBitmap_H

class PatternBitmap : public Drawable {
private:

public:
    PatternBitmap() {
        name = (char *)"Bitmap";
    }

    unsigned int drawFrame() {
        char* filename = (char *) "/aurora/pmlogo32.bmp";

        if (sdAvailable) {
            if (SD.exists(filename)) {
                effects.DimAll(230);

                bitmapPlayer.drawBitmap(filename, 0, 0, true);

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
            else {
                matrix.drawString(0, 0, { 255, 255, 255 }, (char *)"No file");
            }
        }
        else {
            matrix.drawString(0, 0, { 255, 255, 255 }, (char *)"No SD");
        }

        return 30;
    }
};

#endif
