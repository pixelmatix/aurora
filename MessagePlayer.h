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

#ifndef MessagePlayer_H
#define MessagePlayer_H

extern bool sdAvailable;

class MessagePlayer {
private:
    uint8_t count = 0;
    const char* path;

    bool colorSpecified = false;

    String readLine(File file) {
        String line;
        char c;

        while (file.available() && (c = file.read()) != -1) {
            if (c == '\r') {
                file.read(); // throw away the \n that always follows \r
                break;
            }
            else if (c == '\n') {
                break;
            }

            line.append(c);
        }

        return line;
    }

public:
    int currentIndex = -1;
    char message[textLayerMaxStringLength];
    rgb24 color;
    int offsetFromTop;
    int offsetFromLeft;
    String fontName;
    byte scrollSpeed;
    ScrollMode scrollMode;

    void setup(char* directoryName) {
        path = directoryName;

        count = countFiles(directoryName);
        Serial.print(F("message file count: "));
        Serial.println(count);
        currentIndex = -1;
    }

    // count the number of files
    int countFiles(char* directoryName) {
        int count = 0;

        if(!SD.exists(directoryName))
            return 0;

        File directory = SD.open(directoryName, FILE_READ);
        if(!directory)
            return 0;

        File file = directory.openNextFile();
        while (file) {
            if (!file.isDirectory()) {
                if (isMessageFile(file.name())) {
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

    // Get the name of the message file with specified index
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

            if (isMessageFile(file.name())) {
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

    bool isMessageFile(const char filename []) {
        if (filename[0] == '_')
            return false;

        if (filename[0] == '~')
            return false;

        if (filename[0] == '.')
            return false;

        String filenameString = String(filename).toUpperCase();
        if (filenameString.endsWith(".TXT") != 1 && filenameString.endsWith(".JS") != 1)
            return false;

        return true;
    }

    bool loadNextMessage() {
        if (!sdAvailable)
            return false;

        currentIndex++;
        if (currentIndex >= count) {
            currentIndex = -1;
            return false;
        }

        char name[13];
        getNameByIndex(path, currentIndex, name, count);

        //Serial.print(F("message file name: "));
        //Serial.println(name);

        char filepath[255];
        strcpy(filepath, path);
        strcat(filepath, name);

        Serial.print(F("message file path: "));
        Serial.println(filepath);

        File file = SD.open(filepath, FILE_READ);
        if (!file) {
            // move on to the next file
            return loadNextMessage();
        }

        strcpy(message, "");
        color = { 0, 0, 0 };
        offsetFromTop = 1;
        offsetFromLeft = 1;
        fontName = gohufont11b;
        scrollSpeed = 24;
        scrollMode = wrapForward;

        colorSpecified = false;

        String filenameString = String(name).toUpperCase();
        if (filenameString.endsWith(".TXT") == 1)
            readTextFile(file);
        else if (filenameString.endsWith(".JS") == 1)
            readJsonFile(file);

        if (!colorSpecified)
            color = { 255, 255, 255 };

        file.close();
    }

    void readTextFile(File &file) {
        while (file.available() > 0) {
            String text = readLine(file);
            if (text.startsWith("text=")) {
                text = text.substring(5);
                Serial.print(F("message: "));
                Serial.println(text);
                text.toCharArray(message, textLayerMaxStringLength);
            }
            else if (text.startsWith("red=")) {
                text = text.substring(4);
                if (text.length() > 0) {
                    color.red = text.toInt();
                    colorSpecified = true;
                    Serial.print(F("red: "));
                    Serial.println(color.red);
                }
            }
            else if (text.startsWith("green=")) {
                text = text.substring(6);
                if (text.length() > 0) {
                    color.green = text.toInt();
                    colorSpecified = true;
                    Serial.print(F("green: "));
                    Serial.println(color.green);
                }
            }
            else if (text.startsWith("blue=")) {
                text = text.substring(5);
                if (text.length() > 0) {
                    color.blue = text.toInt();
                    colorSpecified = true;
                    Serial.print(F("blue: "));
                    Serial.println(color.blue);
                }
            }
            else if (text.startsWith("top=")) {
                text = text.substring(4);
                if (text.length() > 0) {
                    offsetFromTop = text.toInt();
                    Serial.print(F("offsetFromTop: "));
                    Serial.println(offsetFromTop);
                }
            }
            else if (text.startsWith("left=")) {
                text = text.substring(5);
                if (text.length() > 0) {
                    offsetFromLeft = text.toInt();
                    Serial.print(F("offsetFromLeft: "));
                    Serial.println(offsetFromLeft);
                }
            }
            else if (text.startsWith("font=")) {
                text = text.substring(5);
                if (text.length() > 0) {
                    fontName = text;
                    Serial.print(F("font: "));
                    Serial.println(fontName);
                }
            }
            else if (text.startsWith("speed=")) {
                text = text.substring(6);
                if (text.length() > 0) {
                    scrollSpeed = text.toInt();
                    Serial.print(F("scrollSpeed: "));
                    Serial.println(scrollSpeed);
                }
            }
            else if (text.startsWith("mode=")) {
                text = text.substring(5);
                if (text.length() > 0) {
                    scrollMode = getScrollMode(text);
                    Serial.print(F("scrollMode: "));
                    Serial.println(text);
                }
            }
        }
    }

    void readJsonFile(File &file) {
        aJsonStream sd_stream(&file);

        Serial.print(F("Parsing json..."));
        aJsonObject* root = aJson.parse(&sd_stream);
        Serial.println(F(" done"));

        aJsonObject* property = aJson.getObjectItem(root, "text");
        if (property->type == aJson_String) {
            strcpy(message, property->valuestring);
            Serial.print(F("message: "));
            Serial.println(message);
        }

        property = aJson.getObjectItem(root, "red");
        if (property->type == aJson_Int) {
            color.red = property->valueint;
            colorSpecified = true;
            Serial.print(F("red: "));
            Serial.println(color.red);
        }

        property = aJson.getObjectItem(root, "green");
        if (property->type == aJson_Int) {
            color.green = property->valueint;
            colorSpecified = true;
            Serial.print(F("green: "));
            Serial.println(color.green);
        }

        property = aJson.getObjectItem(root, "blue");
        if (property->type == aJson_Int) {
            color.blue = property->valueint;
            colorSpecified = true;
            Serial.print(F("blue: "));
            Serial.println(color.blue);
        }

        property = aJson.getObjectItem(root, "top");
        if (property->type == aJson_Int) {
            offsetFromTop = property->valueint;
            Serial.print(F("offsetFromTop: "));
            Serial.println(offsetFromTop);
        }

        property = aJson.getObjectItem(root, "left");
        if (property->type == aJson_Int) {
            offsetFromLeft = property->valueint;
            Serial.print(F("offsetFromLeft: "));
            Serial.println(offsetFromLeft);
        }

        property = aJson.getObjectItem(root, "font");
        if (property->type == aJson_String) {
            fontName = property->valuestring;
            Serial.print(F("font: "));
            Serial.println(fontName);
        }

        property = aJson.getObjectItem(root, "speed");
        if (property->type == aJson_Int) {
            scrollSpeed = property->valueint;
            Serial.print(F("scrollSpeed: "));
            Serial.println(scrollSpeed);
        }

        property = aJson.getObjectItem(root, "mode");
        if (property->type == aJson_String) {
            scrollMode = getScrollMode(property->valuestring);
            Serial.print(F("scrollMode: "));
            Serial.println(scrollMode);
        }

        aJson.deleteItem(root);
    }

    fontChoices getFont() {
        /*
        font3x5,
        font5x7,
        font6x10,
        font8x13,
        gohufont11,
        gohufont11b,
        */

        if (fontName == "font3x5") {
            return font3x5;
        }
        else if (fontName == "font5x7") {
            return font5x7;
        }
        else if (fontName == "font6x10") {
            return font6x10;
        }
        else if (fontName == "font8x13") {
            return font8x13;
        }
        else if (fontName == "gohufont11") {
            return gohufont11;
        }
        else if (fontName == "gohufont11b") {
            return gohufont11b;
        }

        return gohufont11b;
    }

    ScrollMode getScrollMode(String scrollModeName) {
        /*
        wrapForward,
        bounceForward,
        bounceReverse,
        stopped,
        off,
        wrapForwardFromLeft
        */

        if (scrollModeName == "wrapForwardFromLeft") {
            return wrapForwardFromLeft;
        }
        else if (scrollModeName == "wrapForward") {
            return wrapForward;
        }
        else if (scrollModeName == "bounceForward") {
            return bounceForward;
        }
        else if (scrollModeName == "bounceReverse") {
            return bounceReverse;
        }
        else if (scrollModeName == "stopped") {
            return stopped;
        }
        else if (scrollModeName == "off") {
            return off;
        }

        return wrapForwardFromLeft;
    }
};

#endif
