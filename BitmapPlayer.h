/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from Adafruit's spitftbitmap example: https://github.com/adafruit/Adafruit_ILI9340/blob/master/examples/spitftbitmap/spitftbitmap.ino
 * Written by Limor Fried/Ladyada for Adafruit Industries: http://www.adafruit.com
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

#ifndef BitmapPlayer_H
#define BitmapPlayer_H

#define BUFFPIXEL 64

class BitmapPlayer {
private:
    // These read 16- and 32-bit types from the SD card file.
    // BMP data is stored little-endian, Arduino is little-endian too.
    // May need to reverse subscript order if porting elsewhere.

    uint16_t read16(File& f) {
        uint16_t result;
        ((uint8_t *) &result)[0] = f.read(); // LSB
        ((uint8_t *) &result)[1] = f.read(); // MSB
        return result;
    }

    uint32_t read32(File& f) {
        uint32_t result;
        ((uint8_t *) &result)[0] = f.read(); // LSB
        ((uint8_t *) &result)[1] = f.read();
        ((uint8_t *) &result)[2] = f.read();
        ((uint8_t *) &result)[3] = f.read(); // MSB
        return result;
    }

public:
    uint32_t drawBitmap(char *filename, uint8_t x = 0, uint8_t y = 0, bool transparency = false, rgb24 transparentColor = { 0, 0, 0 }) {
        File   bmpFile;
        int      bmpWidth, bmpHeight;   // W+H in pixels
        uint8_t  bmpDepth;              // Bit depth (currently must be 24)
        uint32_t bmpImageoffset;        // Start of image data in file
        uint32_t rowSize;               // Not always = bmpWidth; may have padding
        uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
        uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
        boolean  goodBmp = false;       // Set to true on valid header parse
        boolean  flip = true;        // BMP is stored bottom-to-top
        int      w, h, row, col;
        //  uint8_t  r, g, b;
        uint32_t pos = 0, startTime = millis();

        if ((x >= matrix.getScreenWidth()) || (y >= matrix.getScreenHeight())) return 0;

        Serial.println();
        Serial.print("Loading image '");
        Serial.print(filename);
        Serial.println('\'');

        // Open requested file on SD card
        bmpFile = SD.open(filename, FILE_READ);
        if (!bmpFile) {
            return 0;
        }

        // Parse BMP header
        if (read16(bmpFile) == 0x4D42) { // BMP signature
            Serial.print("File size: ");
            Serial.println(read32(bmpFile));
            read32(bmpFile); // Read & ignore creator bytes
            bmpImageoffset = read32(bmpFile); // Start of image data
            Serial.print("Image Offset: ");
            Serial.println(bmpImageoffset, DEC);
            // Read DIB header
            Serial.print("Header size: ");
            Serial.println(read32(bmpFile));
            bmpWidth = read32(bmpFile);
            bmpHeight = read32(bmpFile);
            if (read16(bmpFile) == 1) { // # planes -- must be '1'
                bmpDepth = read16(bmpFile); // bits per pixel
                Serial.print("bmpDepth: ");
                Serial.println(bmpDepth);
                if ((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

                    goodBmp = true; // Supported BMP format -- proceed!
                    Serial.print("Image size: ");
                    Serial.print(bmpWidth);
                    Serial.print('x');
                    Serial.println(bmpHeight);

                    // BMP rows are padded (if needed) to 4-byte boundary
                    rowSize = (bmpWidth * 3 + 3) & ~3;

                    // If bmpHeight is negative, image is in top-down order.
                    // This is not canon but has been observed in the wild.
                    if (bmpHeight < 0) {
                        bmpHeight = -bmpHeight;
                        flip = false;
                    }

                    // Crop area to be loaded
                    w = bmpWidth;
                    h = bmpHeight;

                    if ((x + w - 1) >= matrix.getScreenWidth())  w = matrix.getScreenWidth() - x;
                    if ((y + h - 1) >= matrix.getScreenHeight()) h = matrix.getScreenWidth() - y;

                    for (row = 0; row < h; row++) { // For each scanline...
                        if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
                            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                        else     // Bitmap is stored top-to-bottom
                            pos = bmpImageoffset + row * rowSize;
                        if (bmpFile.position() != pos) { // Need seek?
                            bmpFile.seek(pos);
                            buffidx = sizeof(sdbuffer); // Force buffer reload
                        }

                        // optimize by setting pins now
                        for (col = 0; col < w; col++) { // For each pixel...
                            // Time to read more pixel data?
                            if (buffidx >= sizeof(sdbuffer)) { // Indeed
                                bmpFile.read(sdbuffer, sizeof(sdbuffer));
                                buffidx = 0; // Set index to beginning
                            }

                            // Convert pixel from BMP to Matrix format, push to display
                            rgb24 color;
                            color.blue = sdbuffer[buffidx++];
                            color.green = sdbuffer[buffidx++];
                            color.red = sdbuffer[buffidx++];

                            if (transparency &&
                                transparentColor.red == color.red &&
                                transparentColor.red == color.green &&
                                transparentColor.red == color.blue)
                                continue;

                            matrix.drawPixel(x + col, y + row, color);

                        } // end pixel
                    } // end scanline
                    // matrix.swapBuffers(true);
                    Serial.print("Loaded in ");
                    Serial.print(millis() - startTime);
                    Serial.println(" ms");
                } // end goodBmp
            }
        }

        bmpFile.close();

        if (!goodBmp) {
            Serial.println("BMP format not recognized.");
            return 0;
        }
        else {
            return millis() - startTime;
        }
    }
};

#endif