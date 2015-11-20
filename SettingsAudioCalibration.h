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

#ifndef SettingsAudioCalibration_H
#define SettingsAudioCalibration_H

class SettingsAudioCalibration : public Runnable {
  private:
    int currentBandIndex = 0;
    uint8_t audioScaleBefore = 0;
    boolean hasChanges = false;

    boolean selectingMode = true; // start in mode selection
    int mode = 0; // start in manual

    const int manualMode = 0;
    const int autoMode = 1;
    const int resetMode = 2;
    const int exitMode = 3;

  public:

    void run() {
      selectingMode = true; // start in mode selection
      mode = 0; // start in manual

      audioScaleBefore = audioScale;
      audioScale = 1;

      while (true) {
        backgroundLayer.fillScreen(CRGB(CRGB::Black));

        ReadAudio();

        for (int i = 0; i < bandCount; i++) {
          int level = levels[i];

          CRGB color = CRGB::Green;

          if (i == currentBandIndex && mode == manualMode) {
            color = CRGB::Red;

            if (mode == manualMode && !selectingMode) {
              char text[4];
              sprintf(text, "%d", correction[i]);
              backgroundLayer.drawString(1, 1, { 255, 0, 0 }, text);
            }
          }
          else if (mode == autoMode && !selectingMode) {
            color = CRGB::Red;

            if (level > 0) {
              correction[i] = correction[i] - level;
              hasChanges = true;
            }
          }

          backgroundLayer.drawLine(i * 4 + 2, MATRIX_HEIGHT - level / MATRIX_HEIGHT, i * 4 + 2, 31, color);
          backgroundLayer.drawLine(i * 4 + 3, MATRIX_HEIGHT - level / MATRIX_HEIGHT, i * 4 + 3, 31, color);
          backgroundLayer.drawLine(i * 4 + 4, MATRIX_HEIGHT - level / MATRIX_HEIGHT, i * 4 + 4, 31, color);
          backgroundLayer.drawLine(i * 4 + 5, MATRIX_HEIGHT - level / MATRIX_HEIGHT, i * 4 + 5, 31, color);
        }

        rgb24 white = { 255, 255, 255};

        if (mode == manualMode && selectingMode)
          backgroundLayer.drawString(0, 1, white, "<Manual>");
        else if (mode == autoMode && selectingMode)
          backgroundLayer.drawString(0, 1, white, "< Auto >");
        else if (mode == resetMode && selectingMode)
          backgroundLayer.drawString(0, 1, white, "<Reset >");
        else if (mode == exitMode && selectingMode)
          backgroundLayer.drawString(0, 1, white, "< Exit >");

        backgroundLayer.swapBuffers();

        InputCommand command = readCommand(defaultHoldDelay);

        switch (command) {
          case InputCommand::Up:
            if (mode == manualMode && !selectingMode)
              adjust(1);
            break;

          case InputCommand::Down:
            if (mode == manualMode && !selectingMode)
              adjust(-1);
            break;

          case InputCommand::Left:
            if (mode == manualMode && !selectingMode)
              currentBandIndex--;
            else if (selectingMode)
              mode--;
            break;

          case InputCommand::Right:
            if (mode == manualMode && !selectingMode)
              currentBandIndex++;
            else if (selectingMode)
              mode++;
            break;

          case InputCommand::Select:
            selectingMode = !selectingMode;

            if (mode == exitMode) {
              audioScale = audioScaleBefore;
              if (hasChanges) {
                save();
                hasChanges = false;
              }
              return;
            }
            else if (!selectingMode && mode == autoMode) {
              resetCorrection();
            }
            else if (!selectingMode && mode == resetMode) {
              resetCorrection();
              selectingMode = true;
            }
            break;

          case InputCommand::Back:
          case InputCommand::Power:
          case InputCommand::CycleBrightness:
            audioScale = audioScaleBefore;
            if (hasChanges) {
              save();
              hasChanges = false;
            }
            return;

          default:
            break;
        }

        if (currentBandIndex < 0)
          currentBandIndex = bandCount - 1;
        if (currentBandIndex >= bandCount)
          currentBandIndex = 0;

        if (mode < manualMode)
          mode = exitMode;
        if (mode > exitMode)
          mode = manualMode;
      }
    }

    void resetCorrection() {
      for (int i = 0; i < bandCount; i++) {
        correction[i] = 0;
      }
      hasChanges = true;
    }

    unsigned int drawFrame() {
      backgroundLayer.fillScreen(CRGB(CRGB::Black));
      backgroundLayer.setFont(font3x5);
      backgroundLayer.drawString(0, 27, { 255, 255, 255 }, versionText);
      return 0;
    }

    void adjust(int d) {
      hasChanges = true;
      correction[currentBandIndex] += d;
      if (correction[currentBandIndex] > 0)
        correction[currentBandIndex] = 0;
    }

    void save() {
      char text[4];

      for (int i = 0; i < bandCount; i++) {
        sprintf(text, "audioc%d.txt", i);
        saveIntSetting(text, correction[i]);
      }
    }

    void load() {
      char text[4];

      for (int i = 0; i < bandCount; i++) {
        sprintf(text, "audioc%d.txt", i);
        correction[i] = loadIntSetting(text, 4, 0);
      }
    }
};

#endif
