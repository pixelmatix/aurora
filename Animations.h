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

extern GifPlayer gifPlayer;

class Animations : public Playlist {
  public:

    char* Drawable::name = (char *)"Animations";
  
    int currentIndex = 0;
    int imageCount = 1;

    int getCurrentIndex() {
      return currentIndex;
    }

    void move(int step) {
      currentIndex += step;

      if (currentIndex >= imageCount) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = imageCount - 1;

      openImageFile(false);
    }

    void moveRandom(int step) {
      random16_add_entropy(analogRead(0));
      currentIndex = random(0, imageCount);

      if (currentIndex >= imageCount) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = imageCount - 1;

      openImageFile(false);
    }

    bool setAnimation(int index) {
      if (index >= imageCount || index < 0)
        return false;

      currentIndex = index;
      openImageFile(false);

      return true;
    }

    bool setAnimation(String name) {
      if (!SD.exists(path))
        return false;

      File directory = SD.open(path, FILE_READ);
      if (!directory)
        return -1;

      int index = -1;
      bool foundMatch = false;

      File file = directory.openNextFile();
      while (file) {
        if (!file.isDirectory()) {
          if (isAnimationFile(file.name())) {
            index++;
          }

          if (name.compareTo(file.name()) == 0) {
            foundMatch = true;
            currentIndex = index;
            break;
          }
        }

        file.close();
        file = directory.openNextFile();
      }
      file.close();
      directory.close();

      if (foundMatch) {
        currentIndex = index;

        if (currentIndex >= imageCount) currentIndex = 0;
        else if (currentIndex < 0) currentIndex = imageCount - 1;

        openImageFile(false);

        return true;
      }

      return false;
    }

    unsigned int drawFrame() {
      if (!sdAvailable) {
        matrix.fillScreen({ 0, 0, 0 });
        matrix.setFont(font5x7);
        matrix.drawString(4, 24, { 255, 255, 255 }, "No SD");
        return 30;
      }
      else if (imageCount < 0) {
        matrix.fillScreen({ 0, 0, 0 });
        matrix.setFont(font3x5);
        matrix.drawString(3, 24, { 255, 255, 255 }, "No dir");
        return 30;
      }
      else if (imageCount < 1) {
        matrix.fillScreen({ 0, 0, 0 });
        matrix.setFont(font3x5);
        matrix.drawString(3, 24, { 255, 255, 255 }, "No gifs");
        return 30;
      }

      unsigned long result = gifPlayer.drawFrame();
      if (result == ERROR_FINISHED) {
        openImageFile(true);
        isCurrentItemFinished = true;
      }
      else if (result < 0) {
        return 0;
      }

      return result;
    }

    void start() {
      openImageFile(false);
    }

    void stop() {
      if (imageFile)
        imageFile.close();
    }

    void setup(char* directoryName) {
      path = directoryName;

      if (!SD.exists(directoryName)) {
        SD.mkdir(directoryName);
      }
      
      imageCount = countFiles(directoryName);
      currentIndex = 0;
    }

    bool isAnimationFile(const char filename []) {
      if (filename[0] == '_')
        return false;

      if (filename[0] == '~')
        return false;

      if (filename[0] == '.')
        return false;

      String filenameString = String(filename).toUpperCase();
      if (filenameString.endsWith(".GIF") != 1)
        return false;

      return true;
    }

    void listFiles() {
      Serial.println(F("{"));
      Serial.print(F("  \"count\": "));
      Serial.print(imageCount);
      Serial.println(",");
      Serial.println(F("  \"results\": ["));

      int i = 0;

      if (SD.exists(path)) {
        File directory = SD.open(path, FILE_READ);
        if (directory) {

          File file = directory.openNextFile();
          while (file) {
            if (!file.isDirectory()) {
              if (isAnimationFile(file.name())) {
                Serial.print(F("    \""));
                Serial.print(file.name());
                if (i == imageCount - 1)
                  Serial.println(F("\""));
                else
                  Serial.println(F("\","));
                i++;
              }
            }

            file.close();
            file = directory.openNextFile();
          }
          file.close();
          directory.close();
        }
      }

      Serial.println("  ]");
      Serial.println("}");
    }

  private:

    char* path;

    File imageFile;

    int animationDurationSeconds = 3;

    bool paused = true;

    char filepath[255];

    // count the number of files
    int countFiles(char* directoryName) {
      int count = 0;

      if (!SD.exists(directoryName))
        return -1;

      File directory = SD.open(directoryName, FILE_READ);
      if (!directory)
        return -1;

      File file = directory.openNextFile();
      while (file) {
        if (!file.isDirectory()) {
          if (isAnimationFile(file.name())) {
            count++;
          }
        }

        file.close();
        file = directory.openNextFile();
      }
      file.close();
      directory.close();

      return count;
    }

    void openImageFile(boolean reopen) {
      if (!sdAvailable || imageCount < 1)
        return;

      if (reopen) {
        imageFile.seek(0);
      }
      else {
        if (imageFile)
          imageFile.close();

        char name[13];
        getNameByIndex(path, currentIndex, name, imageCount);

        strcpy(filepath, path);
        strcat(filepath, name);

        imageFile = SD.open(filepath, FILE_READ);
        if (!imageFile)
          return;

        gifPlayer.setFile(imageFile);
        isCurrentItemFinished = false;
      }

      if (!gifPlayer.parseGifHeader()) {
        imageFile.close();
        return;
      }

      gifPlayer.parseLogicalScreenDescriptor();
      gifPlayer.parseGlobalColorTable();
    }

    // Get the name of the GIF file with specified index
    void getNameByIndex(const char *directoryName, int index, char *nameBuffer, int numberOfFiles) {
      // Make sure index is in range
      if ((index < 0) || (index >= numberOfFiles))
        return;

      File directory = SD.open(directoryName, FILE_READ);
      if (!directory)
        return;

      // Make sure file is closed before starting
      File file = directory.openNextFile();

      while (file && (index >= 0)) {
        if (file.isDirectory()) {
          file.close();
          continue;
        }

        if (isAnimationFile(file.name())) {
          index--;

          // Copy the filename name into the buffer
          strcpy(nameBuffer, file.name());
        }

        file.close();
        file = directory.openNextFile();
      }

      file.close();
      directory.close();
    }
};

#endif
