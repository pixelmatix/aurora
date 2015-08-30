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

#ifndef SettingsDemoMode_H
#define SettingsDemoMode_H

class SettingsDemoMode : public Runnable {
  private:
    boolean hasChanges = false;

  public:
    void run() {
      while (true) {
        backgroundLayer.fillScreen(CRGB(CRGB::Black));

        scrollingLayer.setOffsetFromTop(MATRIX_HEIGHT);
        scrollingLayer.setFont(font3x5);
        scrollingLayer.setColor({ 255, 255, 255 });
        indexedLayer.fillScreen(0);

        switch (demoMode) {
          case 0:
            indexedLayer.drawString(0, 13, 1, "None");
            break;

          case 1:
            indexedLayer.drawString(0, 7, 1, "Autoplay");
            indexedLayer.drawString(0, 13, 1, "Audio");
            indexedLayer.drawString(0, 19, 1, "Patterns");
            break;

          case 2:
            indexedLayer.drawString(0, 7, 1, "Random");
            indexedLayer.drawString(0, 13, 1, "Audio");
            indexedLayer.drawString(0, 19, 1, "Patterns");
            break;

          case 3:
            indexedLayer.drawString(0, 10, 1, "Autoplay");
            indexedLayer.drawString(0, 16, 1, "Patterns");
            break;

          case 4:
            indexedLayer.drawString(0, 10, 1, "Random");
            indexedLayer.drawString(0, 16, 1, "Patterns");
            break;

          case 5:
            indexedLayer.drawString(0, 10, 1, "Autoplay");
            indexedLayer.drawString(0, 16, 1, "Animations");
            break;

          case 6:
            indexedLayer.drawString(0, 10, 1, "Random");
            indexedLayer.drawString(0, 16, 1, "Animations");
            break;
        }

        indexedLayer.swapBuffers();

        InputCommand command = readCommand(defaultHoldDelay);

        switch (command) {
          case InputCommand::Up:
            adjustDemoMode(-1);
            hasChanges = true;
            break;

          case InputCommand::Down:
            adjustDemoMode(1);
            hasChanges = true;
            break;

          case InputCommand::Select:
          case InputCommand::Back:
            if (hasChanges) {
              saveDemoMode();
              hasChanges = false;
            }
            return;

          default:
            break;
        }
      }
    }

    unsigned int drawFrame() {
      backgroundLayer.fillScreen(CRGB(CRGB::Black));
      backgroundLayer.setFont(font3x5);
      backgroundLayer.drawString(0, 27, { 255, 255, 255 }, versionText);
      return 0;
    }
};

#endif
