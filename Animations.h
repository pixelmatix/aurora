/*
* Aurora: https://github.com/pixelmatix/aurora
* Copyright (c) 2014 Jason Coon
*
* Portions of this code are adapted from Craig Lindley's LightAppliance: https://github.com/CraigLindley/LightAppliance
* Copyright (c) 2014 Craig A. Lindley
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

#ifndef Animations_H
#define Animations_H

#include "FilenameFunctions.h"

class Animations : public Playlist {
  public:

    char* Drawable::name = (char *)"Animations";
  
    int currentIndex = 0;
    int imageCount = 0;

    int getCurrentIndex() {
      return currentIndex;
    }

    void move(int step) {
      currentIndex += step;

      if (currentIndex >= imageCount) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = imageCount - 1;
      
      openGifFilenameByIndex(path, currentIndex);
    }

    void moveRandom(int step) {
      random16_add_entropy(analogRead(0));
      currentIndex = random(0, imageCount);

      if (currentIndex >= imageCount) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = imageCount - 1;
      
      openGifFilenameByIndex(path, currentIndex);
    }

    void setup(char* directoryName) {
      path = directoryName;

      if (!SD.exists(directoryName)) {
        SD.mkdir(directoryName);
      }
      
      imageCount = enumerateGIFFiles(directoryName, true);
      currentIndex = 0;
    }

    unsigned int drawFrame() {
      if (!sdAvailable) {
        backgroundLayer.fillScreen({ 0, 0, 0 });
        indexedLayer.setFont(font5x7);
        indexedLayer.drawString(4, 24, 1, "No SD");
        return 30;
      }
      else if (imageCount < 0) {
        backgroundLayer.fillScreen({ 0, 0, 0 });
        indexedLayer.setFont(font3x5);
        indexedLayer.drawString(3, 24, 1, "No dir");
        return 30;
      }
      else if (imageCount < 1) {
        backgroundLayer.fillScreen({ 0, 0, 0 });
        indexedLayer.setFont(font3x5);
        indexedLayer.drawString(3, 24, 1, "No gifs");
        return 30;
      }

      gifDecoder.decodeFrame();
      
      return gifDecoder.getFrameDelay_ms();
    }

    void start() {
      openGifFilenameByIndex(path, currentIndex);
    }

    void stop() {
      fileClose();
    }

  private:

    char* path;

    int animationDurationSeconds = 3;

    bool paused = true;

};

#endif
