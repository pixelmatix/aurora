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

#ifndef Patterns_H
#define Patterns_H

#include "Vector.h"
#include "Boid.h"
#include "Attractor.h"

#include "PatternPride.h"
//#include "PatternNoiseSmearing.h"
#include "PatternSpiro.h"
#include "PatternRadar.h"
//#include "PatternAnalogClock.h"
#include "PatternSwirl.h"
#include "PatternPendulumWave.h"
#include "PatternFlowField.h"
#include "PatternIncrementalDrift.h"
#include "PatternIncrementalDrift2.h"
#include "PatternMunch.h"
//#include "PatternElectricMandala.h"
//#include "PatternSpin.h"
//#include "PatternSimplexNoise.h"
#include "PatternWave.h"
//#include "PatternAttract.h"
//#include "PatternBounce.h"
//#include "PatternFlock.h"
#include "PatternInfinity.h"
//#include "PatternPlasma.h"
//#include "PatternSnake.h"
//#include "PatternInvaders.h"
//#include "PatternCube.h"
//#include "PatternFire.h"
//#include "PatternLife.h"
//#include "PatternMaze.h"
//#include "PatternPulse.h"
//#include "PatternRainbowSmoke.h"
//#include "PatternSpark.h"
//#include "PatternSpiral.h"

class Patterns : public Playlist {
  private:
    PatternPride pride;
//    PatternRainbowFlag rainbowFlag;
//    PatternPaletteSmear paletteSmear;
//    PatternMultipleStream multipleStream;
//    PatternMultipleStream2 multipleStream2;
//    PatternMultipleStream3 multipleStream3;
//    PatternMultipleStream4 multipleStream4;
//    PatternMultipleStream5 multipleStream5;
//    PatternMultipleStream8 multipleStream8;
    PatternSpiro spiro;
    PatternRadar radar;
//    PatternAnalogClock analogClock;
    PatternSwirl swirl;
    PatternPendulumWave pendulumWave;
    PatternFlowField flowField;
    PatternIncrementalDrift incrementalDrift;
    PatternIncrementalDrift2 incrementalDrift2;
    PatternMunch munch;
//    PatternElectricMandala electricMandala;
//    PatternSpin spin;
//    PatternSimplexNoise simplexNoise;
    PatternWave wave;
//    PatternAttract attract;
//    PatternBounce bounce;
//    PatternFlock flock;
    PatternInfinity infinity;
//    PatternPlasma plasma;
//    PatternInvadersSmall invadersSmall;
//    PatternInvadersMedium invadersMedium;
//    PatternInvadersLarge invadersLarge;
//    PatternSnake snake;
//    PatternCube cube;
//    PatternFire fire;
//    PatternLife life;
//    PatternMaze maze;
//    PatternPulse pulse;
//    PatternRainbowSmoke rainbowSmoke;
//    PatternSpark spark;
//    PatternSpiral spiral;

    int currentIndex = 0;
    Drawable* currentItem;

    int getCurrentIndex() {
      return currentIndex;
    }

    static const int PATTERN_COUNT = 11;

    Drawable* shuffledItems[PATTERN_COUNT];

    Drawable* items[PATTERN_COUNT] = {
      &pride,
//      &rainbowFlag,
      &spiro,
//      &paletteSmear,
//      &multipleStream8,
//      &multipleStream5,
//      &multipleStream3,
      &radar,
//      &multipleStream4,
//      &multipleStream2,
//      &life,
      &flowField,
      &pendulumWave,
      &incrementalDrift,
      &incrementalDrift2,
      &munch,
//      &electricMandala,
//      &spin,
//      &simplexNoise,
      &wave,
//      &attract,
//      &analogClock,
      &swirl,
//      &bounce,
//      &flock,
      &infinity,
//      &plasma,
//      &invadersSmall,
//      &invadersMedium,
//      &invadersLarge,
//      &snake,
//      &cube,
//      &fire,
//      &maze,
//      &pulse,
//      &rainbowSmoke,
//      &spark,
//      &spiral,
    };

  public:
    Patterns() {
      // add the items to the shuffledItems array
      for (int a = 0; a < PATTERN_COUNT; a++) {
        shuffledItems[a] = items[a];
      }

      shuffleItems();

      this->currentItem = items[0];
      this->currentItem->start();
    }

    char* Drawable::name = (char *)"Patterns";

    void stop() {
      if (currentItem)
        currentItem->stop();
    }

    void start() {
      if (currentItem)
        currentItem->start();
    }

    void move(int step) {
      currentIndex += step;

      if (currentIndex >= PATTERN_COUNT) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = PATTERN_COUNT - 1;

      if (effects.paletteIndex == effects.RandomPaletteIndex)
        effects.RandomPalette();

      moveTo(currentIndex);

//      if (!isTimeAvailable && currentItem == &analogClock)
//        move(step);
    }

    void moveRandom(int step) {
      currentIndex += step;

      if (currentIndex >= PATTERN_COUNT) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = PATTERN_COUNT - 1;

      if (effects.paletteIndex == effects.RandomPaletteIndex)
        effects.RandomPalette();

      if (currentItem)
        currentItem->stop();

      currentItem = shuffledItems[currentIndex];

      if (currentItem)
        currentItem->start();

//      if (!isTimeAvailable && currentItem == &analogClock)
//        moveRandom(step);
    }

    void shuffleItems() {
      for (int a = 0; a < PATTERN_COUNT; a++)
      {
        int r = random(a, PATTERN_COUNT);
        Drawable* temp = shuffledItems[a];
        shuffledItems[a] = shuffledItems[r];
        shuffledItems[r] = temp;
      }
    }

    void moveTo(int index) {
      if (currentItem)
        currentItem->stop();

      currentIndex = index;

      currentItem = items[currentIndex];

      if (currentItem)
        currentItem->start();
    }

    unsigned int drawFrame() {
      ReadAudio();
      return currentItem->drawFrame();
    }

    void listPatterns() {
      Serial.println(F("{"));
      Serial.print(F("  \"count\": "));
      Serial.print(PATTERN_COUNT);
      Serial.println(",");
      Serial.println(F("  \"results\": ["));

      for (int i = 0; i < PATTERN_COUNT; i++) {
        Serial.print(F("    \""));
        Serial.print(items[i]->name);
        if (i == PATTERN_COUNT - 1)
          Serial.println(F("\""));
        else
          Serial.println(F("\","));
      }

      Serial.println("  ]");
      Serial.println("}");
    }

    bool setPattern(String name) {
      for (int i = 0; i < PATTERN_COUNT; i++) {
        if (name.compareTo(items[i]->name) == 0) {
          moveTo(i);
          return true;
        }
      }

      return false;
    }

    bool setPattern(int index) {
      if (index >= PATTERN_COUNT || index < 0)
        return false;

      moveTo(index);

      return true;
    }
};

#endif
