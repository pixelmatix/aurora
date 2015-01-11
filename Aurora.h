/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Designed in collaboration with Pixelmatix using the SmartMatrix Library: http://pixelmatix.com
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

#ifndef AURORA_H_
#define AURORA_H_

#include <SmartMatrix_32x32.h>
const int MATRIX_CENTER_X = MATRIX_WIDTH / 2;
const int MATRIX_CENTER_Y = MATRIX_HEIGHT / 2;


void powerOff();
void loadSettings();
int getBrightnessLevel();
int getBackgroundBrightnessLevel();
void adjustBrightness(int delta);
uint8_t cycleBrightness();
void adjustBackgroundBrightness(int d);
void boundBrightness();
void boundBackgroundBrightness();
void saveBrightnessSetting();
void saveBackgroundBrightnessSetting();
int loadIntSetting(char* dir, const char* settingPath, int maxLength, int defaultValue);
void saveIntSetting(char* dir, const char* settingPath, int value);


#endif
