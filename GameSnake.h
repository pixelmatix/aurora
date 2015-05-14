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

#ifndef GameSnake_H
#define GameSnake_H

class GameSnake : public Runnable {
  private:
    enum DIRECTION {
      UP, DOWN, LEFT, RIGHT
    };

    struct Point {
      int8_t x;
      int8_t y;
    };

    bool isSetup = false;

    DIRECTION direction = RIGHT;
    bool isPaused = false;
    Point snakeHead;
    Point apple;
    QueueArray<Point> segments;
    uint16_t segmentCount = 1;
    uint16_t maxSegmentCount = 1024;
    uint8_t segmentIncrement = 1;
    uint8_t segmentIncrementMultiplier = 1;

    unsigned long lastMillis = 0;
    uint8_t moveSpeed = 150;

    //    uint16_t score;
    //    char scoreText[8];

    CRGB snakeColor = CRGB::Green;
    CRGB appleColor = CRGB::Red;
    CRGB black = CRGB::Black;

    void reset() {
      // Clear screen
      matrix.fillScreen(black);

      //      score = 0;
      //      sprintf(scoreText, "%d", score);

      newApple();

      segmentCount = 4;
      segmentIncrement = 4;
      segmentIncrementMultiplier = 1;

      while (segments.count() > 0) {
        segments.dequeue();
      }

      segments.enqueue(snakeHead);
    }

    void newApple() {
      while (true) {
        apple.x = random(0, 32);
        apple.y = random(0, 32);

        CRGB color = effects.leds[XY(apple.x, apple.y)];
        if (color == black)
          break;
      }
    }

    InputCommand handleInput() {
      InputCommand command = readCommand(); // defaultHoldDelay

      switch (command) {
        case InputCommand::Up:
          if (direction != DOWN) direction = UP;
          break;

        case InputCommand::Down:
          if (direction != UP) direction = DOWN;
          break;

        case InputCommand::Left:
          if (direction != RIGHT) direction = LEFT;
          break;

        case InputCommand::Right:
          if (direction != LEFT) direction = RIGHT;
          break;

        case InputCommand::Select:
          isPaused = !isPaused;
          break;

        default:
          break;
      }

      return command;
    }

    void update() {
      if (millis() - lastMillis >= moveSpeed)
      {
        Point newSnakeHead;
        newSnakeHead.x = snakeHead.x;
        newSnakeHead.y = snakeHead.y;

        // move the snake
        switch (direction)
        {
          case UP:
            newSnakeHead.y--;
            break;
          case DOWN:
            newSnakeHead.y++;
            break;
          case LEFT:
            newSnakeHead.x--;
            break;
          case RIGHT:
            newSnakeHead.x++;
            break;
        }

        // wrap the snake if it hits the edge of the screen (for now)
        if (newSnakeHead.x >= MATRIX_WIDTH) {
          newSnakeHead.x = 0;
        }
        else if (newSnakeHead.x < 0) {
          newSnakeHead.x = MATRIX_WIDTH - 1;
        }

        if (newSnakeHead.y >= MATRIX_HEIGHT) {
          newSnakeHead.y = 0;
        }
        else if (newSnakeHead.y < 0) {
          newSnakeHead.y = MATRIX_HEIGHT - 1;
        }

        CRGB color = effects.leds[XY(newSnakeHead.x, newSnakeHead.y)];
        if (color == snakeColor) {
          // snake ate itself
          die();
        }

        segments.enqueue(newSnakeHead);

        // draw the new location for the snake head
        //        matrix.drawPixel(newSnakeHead.x, newSnakeHead.y, snakeColor);

        if (newSnakeHead.x == apple.x && newSnakeHead.y == apple.y) {
          segmentCount += segmentIncrement * segmentIncrementMultiplier;

          //          score++;
          //          sprintf(scoreText, "%d", score);

          if (segmentCount > maxSegmentCount) {
            segmentCount = maxSegmentCount;
          }
          newApple();
        }

        // trim the end of the snake if it gets too long
        while (segments.count() > segmentCount) {
          //          Point oldSnakeSegment = segments.dequeue();
          //          matrix.drawPixel(oldSnakeSegment.x, oldSnakeSegment.y, black);
          segments.dequeue();
        }

        snakeHead = newSnakeHead;

        lastMillis = millis();
      }
    }

    void die() {
      delay(1000);
      reset();
    }

    void setup() {
      randomSeed(analogRead(5));

      reset();

      snakeHead.x = 16;
      snakeHead.y = 16;

      direction = RIGHT;

      isSetup = true;
    }

  public:
    void run() {
      // Turn off any text scrolling
      matrix.scrollText("", 1);
      matrix.setScrollMode(off);

      if (!isSetup) setup();

      isPaused = false;

      while (true) {
        InputCommand command = handleInput();

        if (command == InputCommand::Back) {
          isPaused = true;
          return;
        }

        drawFrame();
      }
    }

    unsigned int drawFrame() {
      // run in menu background

      if (!isSetup) setup();

      if (!isPaused)
        update();

      matrix.fillScreen(black);
      
      matrix.drawPixel(apple.x, apple.y, appleColor);

      // draw segments
      QueueArray<Point> tempSegments;

      while (segments.count() > 0) {
        Point segment = segments.dequeue();
        tempSegments.enqueue(segment);
        matrix.drawPixel(segment.x, segment.y, snakeColor);
      }

      while (tempSegments.count() > 0) {
        segments.enqueue(tempSegments.dequeue());
      }

      matrix.swapBuffers();

      return 15;
    }
};

#endif
