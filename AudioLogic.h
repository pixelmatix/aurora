/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from "Funky Clouds" by Stefan Petrick: https://gist.github.com/anonymous/876f908333cd95315c35
 * Copyright (c) 2014 Stefan Petrick
 * http://www.stefan-petrick.de/wordpress_beta
 *
 * Portions of this code are adapted from "Audio Spectrum Display" by Tony DiCola https://github.com/tdicola/adafruit_guide_fft
 * Copyright (c) 2013 Tony DiCola
 *
 * Portions of this code (octave remapping) are adapted from "Spectrum Analyzer" https://github.com/Vipor26/Spectrum-Analyzer/
 * Copyright (c) 2014 Brian Hamilton
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

#ifndef AudioLogic_H
#define AudioLogic_H

// MSGEQ7 wiring
#define AUDIO_LEFT_PIN A14
#define MSGEQ7_STROBE_PIN 1
#define MSGEQ7_RESET_PIN 0

const int bandCount = 7;
// the 10Bit (0-1023) audio band values
int levels[bandCount];
int peaks[bandCount];
int peakDecay = 8;
bool drawPeaks = true;

int correction[bandCount] = {
  // -64, -76, -115, -120, -106, -116, -141,
  0, 0, 0, 0, 0, 0, 0,
};

const uint8_t interpolatedBandCount = 16;
int interpolatedLevels[interpolatedBandCount];

int horizontalPixelsPerBand = MATRIX_WIDTH / interpolatedBandCount;

// AGC scale adjustment parameters
// adjust these thresholds to control at what level the scale is adjusted
uint16_t lowThreshold = 512;
uint16_t highThreshold = 1025;

// adjust these timeouts (in ms) to control how long the thresholds above have
// to be crossed before making any adjustments
uint16_t lowTimout = 1000;
uint16_t highTimout = 1;

// these timers keep track of how long the thresholds have been crossed
uint32_t lowTimer = 0;
uint32_t highTimer = 0;

// audio scale steps, likely don't need the higher values, but keeping them for testing for now
const uint8_t audioScaleCount = 12;

const uint8_t minAudioLevel = 192;
boolean hasAudio = false;

uint8_t audioScaleMap[audioScaleCount] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

// defines where the AGC level starts (I'm having it start high for now)
uint8_t autoAudioScale = audioScaleMap[audioScaleCount - 1];

// The audioScale sets how much sound is needed in each frequency range to
// show full height.  Higher numbers are more sensitive.
uint8_t audioScale = audioScaleMap[1];

bool audioScaleChanged = false;
bool showingAudioScaleIndicator = false;
unsigned int audioScaleIndicatorTimout = 0;
unsigned int audioScaleIndicatorDuration = 1000;

int getAudioScaleLevel(float audioScale) {
  int level = 0;
  for (int i = 0; i < audioScaleCount; i++) {
    if (audioScaleMap[i] >= audioScale) {
      level = i;
      break;
    }
  }
  return level;
}

float boundAudioScale(float audioScale) {
  if (audioScale < 0)
    audioScale = 255;
  else if (audioScale > 255)
    audioScale = 0;

  return audioScale;
}

void boundAudioScale() {
  audioScale = boundAudioScale(audioScale);
}

float adjustAudioScale(float audioScale, int delta) {
  int level = getAudioScaleLevel(audioScale);

  level += delta;
  if (level < 0)
    level = audioScaleCount - 1;
  if (level >= audioScaleCount)
    level = 0;

  audioScale = audioScaleMap[level];
  audioScale = boundAudioScale(audioScale);

  return audioScale;
}

void saveAudioScaleSetting() {
  saveByteSetting("audioscl.txt", audioScale);
}

void adjustAudioScale(int delta) {
  audioScale = adjustAudioScale(audioScale, delta);
  saveAudioScaleSetting();
}

void InitAudio() {
  // wake up the MSGEQ7
  pinMode(MSGEQ7_RESET_PIN, OUTPUT);
  pinMode(MSGEQ7_STROBE_PIN, OUTPUT);
  digitalWrite(MSGEQ7_RESET_PIN, LOW);
  digitalWrite(MSGEQ7_STROBE_PIN, HIGH);
}

// get the data from the MSGEQ7
void ReadAudio() {

  hasAudio = false;

  digitalWrite(MSGEQ7_RESET_PIN, HIGH);
  digitalWrite(MSGEQ7_RESET_PIN, LOW);

  int max = 0;

  for (byte band = 0; band < bandCount; band++) {
    digitalWrite(MSGEQ7_STROBE_PIN, LOW);
    delayMicroseconds(20);

    int level = analogRead(AUDIO_LEFT_PIN) + correction[band];
    if (level < 0) level = 0;

    levels[band] = level;

    if (level > max) {
      max = level;
    }

    digitalWrite(MSGEQ7_STROBE_PIN, HIGH);
  }

  float tempScale = audioScale;

  if (audioScale <= 0) {
    int maxLevel = (int) (max * autoAudioScale);
    //Serial.println(maxLevel);

    // is the max level too high and is there room to adjust down??
    if (maxLevel >= highThreshold && autoAudioScale >= audioScaleMap[2]) {
      uint32_t now = millis();

      if (highTimer == 0) {
        // start the high timer
        highTimer = now;
        //Serial.println(F("-----------------------"));
      }
      else if (now > highTimout + highTimer) {
        // it's been high long enough, scale down
        autoAudioScale = adjustAudioScale(autoAudioScale, -1);

        // reset the timers
        highTimer = 0;
        lowTimer = 0;

        //Serial.println(F("-----------------------"));
        //Serial.println(F("decreased scale"));

        // // show an indicator of the auto scale change, for now, while testing
        // audioScaleChanged = true;
        // showingAudioScaleIndicator = true;
        // audioScaleIndicatorTimout = millis() + audioScaleIndicatorDuration;
      }
      else {
        // it hasn't been high long enough, do nothing
        //Serial.print(maxLevel);
        //Serial.print(F(" too high for "));
        //Serial.print(now - highTimer);
        //Serial.println(F("ms"));
      }
    }
    // is the max level too low and is there room to adjust up??
    else if (maxLevel <= lowThreshold && autoAudioScale < audioScaleMap[audioScaleCount - 1]) {
      uint32_t now = millis();

      if (lowTimer == 0) {
        // start the low timer
        lowTimer = now;
        //Serial.println(F("-----------------------"));
      }
      else if (now > lowTimout + lowTimer) {
        // it's been low long enough, scale up
        autoAudioScale = adjustAudioScale(autoAudioScale, 1);

        //Serial.println(F("-----------------------"));
        //Serial.println(F("increased scale"));

        // reset the timers
        lowTimer = 0;
        highTimer = 0;

        // show an indicator of the auto scale change, for now, while testing
        // audioScaleChanged = true;
        // showingAudioScaleIndicator = true;
        // audioScaleIndicatorTimout = millis() + audioScaleIndicatorDuration;
      }
      else {
        //Serial.print(maxLevel);
        //Serial.print(F(" too low for "));
        //Serial.print(now - lowTimer);
        //Serial.println(F("ms"));
      }
    }
    else {
      lowTimer = 0;
      highTimer = 0;
    }

    tempScale = autoAudioScale;
  }

  for (byte band = 0; band < bandCount; band++) {
    int level = levels[band] * tempScale;

    if (level > 1023) {
      level = 1023;
    }

    levels[band] = level;

    if (level >= peaks[band]) {
      peaks[band] = level;
    }
    else if (peaks[band] > 0) {
      peaks[band] = peaks[band] - peakDecay;
    }

    if (level > minAudioLevel)
      hasAudio = true;
  }

  float step = (1.0 * (bandCount - 1)) / (interpolatedBandCount - 1);
  for (int i = 0; i < interpolatedBandCount; i++)
  {
    float v = i * step;
    int x = v;
    v = v - x;
    if (i < interpolatedBandCount - 1) interpolatedLevels[i] = (1 - v) * peaks[x] + v * peaks[x + 1];
    else interpolatedLevels[i] = peaks[x]; // prevent out of index error
  }
}

#endif
