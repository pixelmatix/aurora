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

const uint16_t EXTERNAL_POWER_MIN = 500;

const byte MATRIX_CENTRE_X = MATRIX_CENTER_X - 1;
const byte MATRIX_CENTRE_Y = MATRIX_CENTER_Y - 1;

const uint16_t NUM_LEDS = MATRIX_WIDTH * MATRIX_HEIGHT;

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
int loadIntSetting(const char* name, uint8_t maxLength, int defaultValue);
void saveIntSetting(const char* name, int value);

uint8_t demoMode = 0;

rgb24 white = { 255, 255, 255 };
rgb24 gray = { 128, 128, 128 };
rgb24 black = { 0, 0, 0 };
rgb24 brown = { 137, 104, 48 };
rgb24 red = { 255, 0, 0 };

#endif
