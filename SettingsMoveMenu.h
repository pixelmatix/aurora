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

#ifndef SettingsMoveMenu_H
#define SettingsMoveMenu_H

class SettingsMoveMenu : public Runnable {
  private:
    boolean hasChanges = false;

  public:
    void run() {
      while (true) {
        backgroundLayer.fillScreen(CRGB(CRGB::Black));

        indexedLayer.setFont(gohufont11b);

        uint8_t x = 0;
        uint8_t y = menuY;
        
        indexedLayer.setIndexedColor(1, menuColor);
        indexedLayer.fillScreen(0);
        indexedLayer.drawString(x, y, 0, "Move Menu");

        for (int16_t x = 2; x <= 26; x += 6) {
          // upper indicators
          backgroundLayer.drawTriangle(x + 0, y - 1, x + 1, y - 2, x + 2, y - 1, CRGB(CRGB::SlateGray));
  
          // lower indicators
          backgroundLayer.drawTriangle(x + 0, y + 11, x + 1, y + 12, x + 2, y + 11, CRGB(CRGB::SlateGray));
        }
        
        backgroundLayer.swapBuffers();
        indexedLayer.swapBuffers();

        InputCommand command = readCommand(defaultHoldDelay);

        switch (command) {
          case InputCommand::Up:
            adjustY(-1);
            hasChanges = true;
            break;

          case InputCommand::Down:
            adjustY(1);
            hasChanges = true;
            break;

          case InputCommand::Select:
          case InputCommand::Back:
            if (hasChanges) {
              saveMenuY();
              hasChanges = false;
            }
            return;

          default:
            break;
        }
      }
    }

    void boundMenuY() {
      if(menuY < 0) menuY = 0;
      if(menuY > MATRIX_HEIGHT - 11) menuY = MATRIX_HEIGHT - 11;
    }
    
    void saveMenuY() {
      saveByteSetting(menuYFilename, menuY);
    }

    void adjustY(int d) {
      menuY += d;
      boundMenuY();
    }

    unsigned int drawFrame() {
      backgroundLayer.fillScreen(CRGB(CRGB::Black));
      backgroundLayer.setFont(font3x5);
      backgroundLayer.drawString(0, 27, { 255, 255, 255 }, versionText);
      return 0;
    }
};

#endif
