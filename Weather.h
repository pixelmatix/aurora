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

#ifndef Weather_H
#define Weather_H

class Weather : public Playlist {
  public:
    char* Drawable::name = (char *)"Weather";

    File imageFile;

    int temperature = 99;
    int type = 0;

    char* typesPath = (char *) "/weather/";

    static const uint8_t weatherTypeCount = 10;

    char* types[weatherTypeCount] = {
      (char*) "sunny",
      (char*) "cloudy",
      (char*) "hail",
      (char*) "lightnin",
      (char*) "pcloudy",
      (char*) "pcloudrn",
      (char*) "raining",
      (char*) "snow",
      (char*) "snowrain",
      (char*) "storm",
    };

    bool setTemperature(int value) {
      if (value < -999 || value > 999)
        return false;

      temperature = value;
      
      return true;
    }

    bool setWeatherType(int value) {
      if (value < 0 || value >= weatherTypeCount)
        return false;

      type = value;

      openImageFile(types[type]);
      
      return true;
    }

    void openImageFile(const char* name) {
      if (!sdAvailable)
        return;

      if (imageFile)
        imageFile.close();

      char filepath[20];
      strcpy(filepath, typesPath);
      strcat(filepath, name);
      strcat(filepath, ".gif");

      if (!SD.exists(filepath))
        return;

      imageFile = SD.open(filepath, FILE_READ);
      if (!imageFile)
        return;

      if (imageFile.isDirectory()) {
        imageFile.close();
        return;
      }

      gifPlayer.setFile(imageFile);

      if (!gifPlayer.parseGifHeader()) {
        imageFile.close();
        return;
      }

      gifPlayer.parseLogicalScreenDescriptor();
      gifPlayer.parseGlobalColorTable();
    }

    void start() {
      matrix.fillScreen(CRGB(CRGB::Black));
      openImageFile(types[type]);
    }

    void stop() {
      if (imageFile)
        imageFile.close();
    }

    unsigned int drawFrame() {
      unsigned long result = 30;

      if (imageFile && imageFile.available()) {
        unsigned long result = gifPlayer.drawFrame();
        if (result == ERROR_FINISHED) {
          openImageFile(types[type]);
        }
      }
      else {
        matrix.fillScreen({0, 0, 0});
      }

      if (result < 0) {
        result = 30;
      }

      matrix.setFont(font3x5);

      char temp[4];
      sprintf(temp, "%d", temperature);

      // draw the date
      matrix.drawString(1, 27, clockDisplay.color, temp);

      return result;
    }

    void move(int step) {}
    void moveRandom(int step) {}
    int getCurrentIndex() {
      return 0;
    }
};

#endif
