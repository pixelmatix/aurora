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

#ifndef Games_H
#define Games_H

#include "GameSnake.h"
#include "GameTetris.h"

class Games : public Runnable {
  private:
    Menu gamesMenu;

    GameSnake snake;
    GameTetris tetris;
    Drawable exit;

    static const int itemCount = 3;

    MenuItem menuItemSnake = MenuItem((char *)"Snake", &snake);
    MenuItem menuItemTetris = MenuItem((char *)"Tetris", &tetris);
    MenuItem menuItemExit = MenuItem((char *)"Exit Games", &exit, true);

    MenuItem* menuItems[itemCount] = {
      &menuItemSnake,
      &menuItemTetris,
      &menuItemExit,
    };

  public:
    Games() {
    }

    char* Drawable::name = (char *)"Games";

    void run() {
      gamesMenu.canMoveBack = true;
      gamesMenu.run(menuItems, itemCount);
    }

    unsigned int drawFrame() {
      return snake.drawFrame();
//      return 30;
    }

    void start() {
    }

    void stop() {
    }
};

#endif
