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

#ifndef GameTetris_H
#define GameTetris_H

class GameTetris : public Runnable {
  private:
    uint32_t delays = 0;
    uint16_t delay_ = 500;
    uint32_t bdelay = 0;
    uint16_t buttondelay = 150;
    uint16_t btdowndelay = 30;
    uint16_t btsidedelay = 80;
    unsigned char blocktype;
    unsigned char blockrotation;

    bool isSetup = false;

    static const uint8_t FIELD_WIDTH = 10;
    static const uint8_t FIELD_HEIGHT = 20;

    const rgb24 COLOR_BLACK = { 0, 0, 0 };
    const rgb24 COLOR_RED = { 255, 0, 0 };
    const rgb24 COLOR_GREEN = { 0, 255, 0 };
    const rgb24 COLOR_LGREEN = { 0, 80, 0 };
    const rgb24 COLOR_BLUE = { 0, 0, 255 };
    const rgb24 COLOR_WHITE = { 255, 255, 255 };
    const rgb24 COLOR_GRAY = { 127, 127, 127 };
    const rgb24 COLOR_CYAN = { 0, 255, 255 };
    const rgb24 COLOR_ORANGE = { 255, 165, 0 };
    const rgb24 COLOR_YELLOW = { 255, 255, 0 };
    const rgb24 COLOR_PURPLE = { 160, 32, 240 };

    rgb24 blockColors[8] = {
      COLOR_BLACK,  // 0 Blank
      COLOR_CYAN,   // 1 I
      COLOR_BLUE,   // 2 J
      COLOR_ORANGE, // 3 L
      COLOR_YELLOW, // 4 O
      COLOR_GREEN,  // 5 S
      COLOR_PURPLE, // 6 T
      COLOR_RED,    // 7 Z
    };

    uint8_t block[FIELD_WIDTH][FIELD_HEIGHT + 2]; //2 extra for rotation
    uint8_t pile[FIELD_WIDTH][FIELD_HEIGHT];

    unsigned long startTime;
    unsigned long elapsedTime;
    uint8_t cnt = 0;

    uint8_t newBlockIndex;
    uint8_t nextBlockIndex;
    uint8_t blockBag[7] = { 0, 1, 2, 3, 4, 5, 6 };
    uint8_t nextBlockType;
    uint8_t nextBlock[6][4];

    int score;
    char scoreText[8];

    int linesCleared;
    char linesClearedText[8];

    bool isPaused = false;

    int i;
    int j;

    void reset() {
      // Serial.println("reset");
      newBlockIndex = 0;

      // generate new random bag of blocks
      for (i = 0; i < 7; i++) {
        int r = random(i, 6);
        int temp = blockBag[i];
        blockBag[i] = blockBag[r];
        blockBag[r] = temp;
      }

      linesCleared = 0;
      sprintf(linesClearedText, "L:%d", linesCleared);

      score = 0;
      sprintf(scoreText, "S:%d", score);

      for (int y = FIELD_HEIGHT - 1; y >= 0; y--)
      {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
          pile[x][y] = 0;
          block[x][y] = 0;
        }
      }
    }

    InputCommand handleInput() {
      InputCommand command = readCommand(100);

      switch (command) {
        case InputCommand::Up:
          rotate();
          break;

        case InputCommand::Down:
          movedown();
          break;

        case InputCommand::Left:
          moveleft();
          break;

        case InputCommand::Right:
          moveright();
          break;

        default:
          break;
      }

      return command;
    }

    void check_gameover()
    {
      uint8_t cnt = 0;
      uint8_t lineCount = 0;

      for (i = FIELD_HEIGHT - 1; i >= 0; i--)
      {
        cnt = 0;
        for (j = 0; j < FIELD_WIDTH; j++)
        {
          if (pile[j][i] > 0)
          {
            cnt++;
          }
        }
        if (cnt == FIELD_WIDTH)
        {
          lineCount++;
          // we have a solid line all the way across
          for (j = 0; j < FIELD_WIDTH; j++)
          {
            pile[j][i] = 0;
          }
          delay(50);

          int k;
          for (k = i; k > 0; k--)
          {
            for (j = 0; j < FIELD_WIDTH; j++)
            {
              pile[j][k] = pile[j][k - 1];
            }
          }
          for (j = 0; j < FIELD_WIDTH; j++)
          {
            pile[j][0] = 0;
          }
          delay(50);
          i++;
        }
      }

      for (i = 0; i < FIELD_WIDTH; i++)
      {
        if (pile[i][0] > 0) {
          gameover();
          return;
        }
      }

      if (lineCount > 0) {
        linesCleared += lineCount;
        sprintf(linesClearedText, "L:%d", linesCleared);

        score += lineCount * lineCount;
        sprintf(scoreText, "S:%d", score);
      }
    }

    void gameover()
    {
      startTime = millis();

      delay(3000);

      reset();
    }

    bool space_below()
    {
      for (i = FIELD_HEIGHT - 1; i >= 0; i--)
      {
        for (j = 0; j < FIELD_WIDTH; j++)
        {
          if (block[j][i] > 0)
          {
            if (i == FIELD_HEIGHT - 1)
              return false;
            if (pile[j][i + 1] > 0)
            {
              return false;
            }
          }
        }
      }
      return true;
    }

    bool space_left()
    {
      for (i = FIELD_HEIGHT - 1; i >= 0; i--)
      {
        for (j = 0; j < FIELD_WIDTH; j++)
        {
          if (block[j][i] > 0)
          {
            if (j == 0)
              return false;
            if (pile[j - 1][i] > 0)
            {
              return false;
            }
          }
        }
      }
      return true;
    }

    bool space_left2()
    {
      for (i = FIELD_HEIGHT - 1; i >= 0; i--)
      {
        for (j = 0; j < FIELD_WIDTH; j++)
        {
          if (block[j][i] > 0)
          {
            if (j == 0 || j == 1)
              return false;
            if (pile[j - 1][i] > 0 || pile[j - 2][i] > 0)
            {
              return false;
            }
          }
        }
      }
      return true;
    }

    bool space_left3()
    {
      for (i = FIELD_HEIGHT - 1; i >= 0; i--)
      {
        for (j = 0; j < FIELD_WIDTH; j++)
        {
          if (block[j][i] > 0)
          {
            if (j == 0 || j == 1 || j == 2)
              return false;
            if (pile[j - 1][i] > 0 || pile[j - 2][i] > 0 || pile[j - 3][i] > 0)
            {
              return false;
            }
          }
        }
      }
      return true;
    }

    bool space_right()
    {
      for (i = FIELD_HEIGHT - 1; i >= 0; i--)
      {
        for (j = 0; j < FIELD_WIDTH; j++)
        {
          if (block[j][i] > 0)
          {
            if (j == FIELD_WIDTH - 1)
              return false;
            if (pile[j + 1][i] > 0)
            {
              return false;
            }
          }
        }
      }
      return true;
    }

    bool space_right2()
    {
      for (i = FIELD_HEIGHT - 1; i >= 0; i--)
      {
        for (j = 0; j < FIELD_WIDTH; j++)
        {
          if (block[j][i] > 0)
          {
            if (j == FIELD_WIDTH - 1 || j == FIELD_WIDTH - 2)
              return false;
            if (pile[j + 1][i] > 0 || pile[j + 2][i] > 0)
            {
              return false;
            }
          }
        }
      }
      return true;
    }

    bool space_right3()
    {
      for (i = FIELD_HEIGHT - 1; i >= 0; i--)
      {
        for (j = 0; j < FIELD_WIDTH; j++)
        {
          if (block[j][i] > 0)
          {
            if (j == FIELD_WIDTH - 1 || j == FIELD_WIDTH - 2 || j == FIELD_WIDTH - 3)
              return false;
            if (pile[j + 1][i] > 0 || pile[j + 2][i] > 0 || pile[j + 3][i] > 0)
            {
              return false;
            }
          }
        }
      }
      return true;
    }

    bool moveleft()
    {
      if (space_left())
      {
        for (i = 0; i < FIELD_WIDTH - 1; i++)
        {
          for (j = 0; j < FIELD_HEIGHT; j++)
          {
            block[i][j] = block[i + 1][j];
          }
        }

        for (j = 0; j < FIELD_HEIGHT; j++)
        {
          block[FIELD_WIDTH - 1][j] = 0;
        }

        return 1;
      }

      return 0;
    }

    bool moveright()
    {
      if (space_right())
      {
        for (i = FIELD_WIDTH - 1; i > 0; i--)
        {
          for (j = 0; j < FIELD_HEIGHT; j++)
          {
            block[i][j] = block[i - 1][j];
          }
        }

        for (j = 0; j < FIELD_HEIGHT; j++)
        {
          block[0][j] = 0;
        }

        return 1;

      }
      return 0;
    }

    void movedown()
    {
      if (space_below())
      {
        //move down
        for (i = FIELD_HEIGHT - 1; i >= 0; i--)
        {
          for (j = 0; j < FIELD_WIDTH; j++)
          {
            block[j][i] = block[j][i - 1];
          }
        }
        for (i = 0; i < FIELD_WIDTH - 1; i++)
        {
          block[i][0] = 0;
        }
      }
      else
      {
        //merge and new block
        for (i = 0; i < FIELD_WIDTH; i++)
        {
          for (j = 0; j < FIELD_HEIGHT; j++)
          {
            if (block[i][j] > 0)
            {
              pile[i][j] = block[i][j];
              block[i][j] = 0;
            }
          }
        }
        newBlock();
      }
    }

    void newBlock() {
      // Serial.println("newBlock");
      check_gameover();

      blocktype = blockBag[newBlockIndex];

      newBlockIndex++;

      // need to generate a new bag of blocks?
      if (newBlockIndex == 7) {
        newBlockIndex = 0;

        for (i = 0; i < 7; i++) {
          int r = random(i, 6);
          int temp = blockBag[i];
          blockBag[i] = blockBag[r];
          blockBag[r] = temp;
        }
      }

      nextBlockIndex = newBlockIndex;
      nextBlockType = blockBag[newBlockIndex];

      blockrotation = 0;

      if (blocktype == 0) // I
        // 0000
      {
        block[3][0] = blocktype + 1;
        block[4][0] = blocktype + 1;
        block[5][0] = blocktype + 1;
        block[6][0] = blocktype + 1;

        blockrotation = 1;
      }
      else if (blocktype == 1) // J
        // 0
        // 0 0 0
      {
        block[3][0] = blocktype + 1;
        block[3][1] = blocktype + 1;
        block[4][1] = blocktype + 1;
        block[5][1] = blocktype + 1;
      }
      else if (blocktype == 2) // L
        //     0
        // 0 0 0
      {
        block[5][0] = blocktype + 1;
        block[3][1] = blocktype + 1;
        block[4][1] = blocktype + 1;
        block[5][1] = blocktype + 1;
      }
      else if (blocktype == 3) // O
        // 0 0
        // 0 0
      {
        block[4][0] = blocktype + 1;
        block[4][1] = blocktype + 1;
        block[5][0] = blocktype + 1;
        block[5][1] = blocktype + 1;
      }
      else if (blocktype == 4) // S
        //   0 0
        // 0 0
      {
        block[4][0] = blocktype + 1;
        block[5][0] = blocktype + 1;
        block[3][1] = blocktype + 1;
        block[4][1] = blocktype + 1;
      }
      else if (blocktype == 5) // T
        //   0
        // 0 0 0
      {
        block[4][0] = blocktype + 1;
        block[3][1] = blocktype + 1;
        block[4][1] = blocktype + 1;
        block[5][1] = blocktype + 1;
      }
      else if (blocktype == 6) // Z
        // 0 0
        //   0 0
      {
        block[3][0] = blocktype + 1;
        block[4][0] = blocktype + 1;
        block[4][1] = blocktype + 1;
        block[5][1] = blocktype + 1;
      }

      // next block
      for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 6; x++) {
          nextBlock[x][y] = 0;
        }
      }

      if (nextBlockType == 0) // I
        // 0000
      {
        nextBlock[1][2] = nextBlockType + 1;
        nextBlock[2][2] = nextBlockType + 1;
        nextBlock[3][2] = nextBlockType + 1;
        nextBlock[4][2] = nextBlockType + 1;
      }
      else if (nextBlockType == 1) // J
        // 0
        // 0 0 0
      {
        nextBlock[1][1] = nextBlockType + 1;
        nextBlock[1][2] = nextBlockType + 1;
        nextBlock[2][2] = nextBlockType + 1;
        nextBlock[3][2] = nextBlockType + 1;
      }
      else if (nextBlockType == 2) // L
        //     0
        // 0 0 0
      {
        nextBlock[1][2] = nextBlockType + 1;
        nextBlock[2][2] = nextBlockType + 1;
        nextBlock[3][2] = nextBlockType + 1;
        nextBlock[3][1] = nextBlockType + 1;
      }
      else if (nextBlockType == 3) // O
        // 0 0
        // 0 0
      {
        nextBlock[1][1] = nextBlockType + 1;
        nextBlock[2][1] = nextBlockType + 1;
        nextBlock[1][2] = nextBlockType + 1;
        nextBlock[2][2] = nextBlockType + 1;
      }
      else if (nextBlockType == 4) // S
        //   0 0
        // 0 0
      {
        nextBlock[2][1] = nextBlockType + 1;
        nextBlock[3][1] = nextBlockType + 1;
        nextBlock[1][2] = nextBlockType + 1;
        nextBlock[2][2] = nextBlockType + 1;
      }
      else if (nextBlockType == 5) // T
        //   0
        // 0 0 0
      {
        nextBlock[1][2] = nextBlockType + 1;
        nextBlock[2][1] = nextBlockType + 1;
        nextBlock[2][2] = nextBlockType + 1;
        nextBlock[3][2] = nextBlockType + 1;
      }
      else if (nextBlockType == 6) // Z
        // 0 0
        //   0 0
      {
        nextBlock[1][1] = nextBlockType + 1;
        nextBlock[2][1] = nextBlockType + 1;
        nextBlock[2][2] = nextBlockType + 1;
        nextBlock[3][2] = nextBlockType + 1;
      }
    }

    bool check_overlap()
    {
      for (i = 0; i < FIELD_HEIGHT; i++)
      {
        for (j = 0; j < FIELD_WIDTH - 1; j++)
        {
          if (block[j][i] > 0)
          {
            if (pile[j][i])
              return false;
          }
        }
      }
      for (i = FIELD_HEIGHT; i < FIELD_HEIGHT + 2; i++)
      {
        for (j = 0; j < FIELD_WIDTH - 1; j++)
        {
          if (block[j][i] > 0)
          {
            return false;
          }
        }
      }
      return true;
    }

    void rotate()
    {
      //skip for square block(3)
      if (blocktype == 3) return;

      int xi = 0;
      int yi = 0;

      //detect left
      for (i = FIELD_WIDTH - 1; i >= 0; i--)
      {
        for (j = 0; j < FIELD_HEIGHT; j++)
        {
          if (block[i][j] > 0)
          {
            xi = i;
          }
        }
      }

      //detect up
      for (i = FIELD_HEIGHT - 1; i >= 0; i--)
      {
        for (j = 0; j < FIELD_WIDTH; j++)
        {
          if (block[j][i] > 0)
          {
            yi = i;
          }
        }
      }

      if (blocktype == 0)
      {
        if (blockrotation == 0)
        {
          if (!space_left())
          {
            if (space_right3())
            {
              if (!moveright())
                return;
              xi++;
            }
            else return;
          }
          else if (!space_right())
          {
            if (space_left3())
            {
              if (!moveleft())
                return;
              if (!moveleft())
                return;
              xi--;
              xi--;
            }
            else
              return;
          }
          else if (!space_right2())
          {
            if (space_left2())
            {
              if (!moveleft())
                return;
              xi--;
            }
            else
              return;
          }

          block[xi][yi] = 0;
          block[xi][yi + 2] = 0;
          block[xi][yi + 3] = 0;

          block[xi - 1][yi + 1] = blocktype + 1;
          block[xi + 1][yi + 1] = blocktype + 1;
          block[xi + 2][yi + 1] = blocktype + 1;

          blockrotation = 1;
        }
        else
        {
          block[xi][yi] = 0;
          block[xi + 2][yi] = 0;
          block[xi + 3][yi] = 0;

          block[xi + 1][yi - 1] = blocktype + 1;
          block[xi + 1][yi + 1] = blocktype + 1;
          block[xi + 1][yi + 2] = blocktype + 1;

          blockrotation = 0;
        }
      }

      //offset to mid
      xi++;
      yi++;

      if (blocktype == 1)
      {
        if (blockrotation == 0)
        {
          block[xi - 1][yi - 1] = 0;
          block[xi - 1][yi] = 0;
          block[xi + 1][yi] = 0;

          block[xi][yi - 1] = blocktype + 1;
          block[xi + 1][yi - 1] = blocktype + 1;
          block[xi][yi + 1] = blocktype + 1;

          blockrotation = 1;
        }
        else if (blockrotation == 1)
        {
          if (!space_left())
          {
            if (!moveright())
              return;
            xi++;
          }
          xi--;

          block[xi][yi - 1] = 0;
          block[xi + 1][yi - 1] = 0;
          block[xi][yi + 1] = 0;

          block[xi - 1][yi] = blocktype + 1;
          block[xi + 1][yi] = blocktype + 1;
          block[xi + 1][yi + 1] = blocktype + 1;

          blockrotation = 2;
        }
        else if (blockrotation == 2)
        {
          yi--;

          block[xi - 1][yi] = 0;
          block[xi + 1][yi] = 0;
          block[xi + 1][yi + 1] = 0;

          block[xi][yi - 1] = blocktype + 1;
          block[xi][yi + 1] = blocktype + 1;
          block[xi - 1][yi + 1] = blocktype + 1;

          blockrotation = 3;
        }
        else
        {
          if (!space_right())
          {
            if (!moveleft())
              return;
            xi--;
          }
          block[xi][yi - 1] = 0;
          block[xi][yi + 1] = 0;
          block[xi - 1][yi + 1] = 0;

          block[xi - 1][yi - 1] = blocktype + 1;
          block[xi - 1][yi] = blocktype + 1;
          block[xi + 1][yi] = blocktype + 1;

          blockrotation = 0;
        }
      }

      if (blocktype == 2)
      {
        if (blockrotation == 0)
        {
          block[xi + 1][yi - 1] = 0;
          block[xi - 1][yi] = 0;
          block[xi + 1][yi] = 0;

          block[xi][yi - 1] = blocktype + 1;
          block[xi + 1][yi + 1] = blocktype + 1;
          block[xi][yi + 1] = blocktype + 1;

          blockrotation = 1;
        }
        else if (blockrotation == 1)
        {
          if (!space_left())
          {
            if (!moveright())
              return;
            xi++;
          }
          xi--;

          block[xi][yi - 1] = 0;
          block[xi + 1][yi + 1] = 0;
          block[xi][yi + 1] = 0;

          block[xi - 1][yi] = blocktype + 1;
          block[xi + 1][yi] = blocktype + 1;
          block[xi - 1][yi + 1] = blocktype + 1;

          blockrotation = 2;
        }
        else if (blockrotation == 2)
        {
          yi--;

          block[xi - 1][yi] = 0;
          block[xi + 1][yi] = 0;
          block[xi - 1][yi + 1] = 0;

          block[xi][yi - 1] = blocktype + 1;
          block[xi][yi + 1] = blocktype + 1;
          block[xi - 1][yi - 1] = blocktype + 1;

          blockrotation = 3;
        }
        else
        {
          if (!space_right())
          {
            if (!moveleft())
              return;
            xi--;
          }
          block[xi][yi - 1] = 0;
          block[xi][yi + 1] = 0;
          block[xi - 1][yi - 1] = 0;

          block[xi + 1][yi - 1] = blocktype + 1;
          block[xi - 1][yi] = blocktype + 1;
          block[xi + 1][yi] = blocktype + 1;

          blockrotation = 0;
        }
      }

      if (blocktype == 4)
      {
        if (blockrotation == 0)
        {
          block[xi + 1][yi - 1] = 0;
          block[xi - 1][yi] = 0;

          block[xi + 1][yi] = blocktype + 1;
          block[xi + 1][yi + 1] = blocktype + 1;

          blockrotation = 1;
        }
        else
        {
          if (!space_left())
          {
            if (!moveright())
              return;
            xi++;
          }
          xi--;

          block[xi + 1][yi] = 0;
          block[xi + 1][yi + 1] = 0;

          block[xi - 1][yi] = blocktype + 1;
          block[xi + 1][yi - 1] = blocktype + 1;

          blockrotation = 0;
        }
      }


      if (blocktype == 5)
      {
        if (blockrotation == 0)
        {
          block[xi][yi - 1] = 0;
          block[xi - 1][yi] = 0;
          block[xi + 1][yi] = 0;

          block[xi][yi - 1] = blocktype + 1;
          block[xi + 1][yi] = blocktype + 1;
          block[xi][yi + 1] = blocktype + 1;

          blockrotation = 1;
        }
        else if (blockrotation == 1)
        {
          if (!space_left())
          {
            if (!moveright())
              return;
            xi++;
          }
          xi--;

          block[xi][yi - 1] = 0;
          block[xi + 1][yi] = 0;
          block[xi][yi + 1] = 0;

          block[xi - 1][yi] = blocktype + 1;
          block[xi + 1][yi] = blocktype + 1;
          block[xi][yi + 1] = blocktype + 1;

          blockrotation = 2;
        }
        else if (blockrotation == 2)
        {
          yi--;

          block[xi - 1][yi] = 0;
          block[xi + 1][yi] = 0;
          block[xi][yi + 1] = 0;

          block[xi][yi - 1] = blocktype + 1;
          block[xi - 1][yi] = blocktype + 1;
          block[xi][yi + 1] = blocktype + 1;

          blockrotation = 3;
        }
        else
        {
          if (!space_right())
          {
            if (!moveleft())
              return;
            xi--;
          }
          block[xi][yi - 1] = 0;
          block[xi - 1][yi] = 0;
          block[xi][yi + 1] = 0;

          block[xi][yi - 1] = blocktype + 1;
          block[xi - 1][yi] = blocktype + 1;
          block[xi + 1][yi] = blocktype + 1;

          blockrotation = 0;
        }
      }

      if (blocktype == 6)
      {
        if (blockrotation == 0)
        {
          block[xi - 1][yi - 1] = 0;
          block[xi][yi - 1] = 0;

          block[xi + 1][yi - 1] = blocktype + 1;
          block[xi][yi + 1] = blocktype + 1;

          blockrotation = 1;
        }
        else
        {
          if (!space_left())
          {
            if (!moveright())
              return;
            xi++;
          }
          xi--;

          block[xi + 1][yi - 1] = 0;
          block[xi][yi + 1] = 0;

          block[xi - 1][yi - 1] = blocktype + 1;
          block[xi][yi - 1] = blocktype + 1;

          blockrotation = 0;
        }
      }

      //if rotating made block and pile overlap, push rows up
      while (!check_overlap())
      {
        for (i = 0; i < FIELD_HEIGHT + 2; i++)
        {
          for (j = 0; j < FIELD_WIDTH; j++)
          {
            block[j][i] = block[j][i + 1];
          }
        }
        delays = millis() + delay_;
      }
    }

    void setup() {
      // Serial.println("setup");
      randomSeed(analogRead(5));

      reset();

      newBlock();

      isSetup = true;
    }

    void update() {
      // Serial.println("TetrisGame::update");
      delay(30);

      if (delays < millis())
      {
        delays = millis() + delay_;
        movedown();
      }
    }

  public:
    void run() {
      // Turn off any text scrolling
      matrix.scrollText("", 1);
      matrix.setScrollMode(off);

      // Fonts are font3x5, font5x7, font6x10, font8x13
      matrix.setFont(font3x5);

      if (!isSetup) setup();

      isPaused = false;

      while (true) {
        InputCommand command = handleInput();

        switch (command) {
          case InputCommand::Back:
          case InputCommand::Select:
          case InputCommand::Power:
          case InputCommand::CycleBrightness:
            isPaused = true;
            return;

          default:
            break;
        }

        drawFrame();
      }
    }

    unsigned int drawFrame() {
      if (!isPaused) {
        update();
      }

      // Serial.println("TetrisGame::draw");

      // Serial.println("clearing the screen");
      // Clear screen
      matrix.fillScreen(COLOR_BLACK);

      // Serial.println("drawing the border");
      // draw border
      matrix.drawRectangle(10, 5, 21, 26, COLOR_GRAY);

      // Serial.println("drawing the score");
      // draw score
      matrix.drawString(0, 0, COLOR_GRAY, scoreText);

      // Serial.println("drawing the lines cleared count");
      // draw lines cleared
      matrix.drawString(0, 27, COLOR_GRAY, linesClearedText);

      // Serial.println("drawing the next block indicator");
      // draw next block
      for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 6; x++) {
          int b = nextBlock[x][y];

          if (b == 0)
            matrix.drawPixel(x + 23, y + 6, COLOR_GRAY);
          else
            matrix.drawPixel(x + 23, y + 6, blockColors[b]);
        }
      }

      uint8_t left = (MATRIX_WIDTH - FIELD_WIDTH) / 2;
      uint8_t top = (MATRIX_HEIGHT - FIELD_HEIGHT) / 2;
      //      int right = MATRIX_WIDTH - left;
      //      int bottom = MATRIX_HEIGHT - top;

      // Serial.println("drawing the blocks in the play field");

      for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
          rgb24 color = COLOR_BLACK;

          // Serial.print("getting block type at pile[");
          // Serial.print(x);
          // Serial.print("][");
          // Serial.print(y);
          // Serial.print("]...");
          int blockType = pile[x][y];
          // Serial.println(" done");

          if (blockType == 0) {
            // Serial.print("no block on pile, getting block type at block[");
            // Serial.print(x);
            // Serial.print("][");
            // Serial.print(y);
            // Serial.print("]...");
            blockType = block[x][y];
            // Serial.println(" done");
          }

          if (blockType > 0) {
            // Serial.print("getting block color at blockColors[");
            // Serial.print(blockType);
            // Serial.print("]...");
            color = blockColors[blockType];
            // Serial.println(" done");
          }

          // Serial.print("drawing pixel at ");
          // Serial.print(x + left);
          // Serial.print(", ");
          // Serial.print(y + top);
          // Serial.print(" of type ");
          // Serial.print(blockType);
          // Serial.print(" in color ");
          // Serial.print(color.red);
          // Serial.print(", ");
          // Serial.print(color.green);
          // Serial.print(", ");
          // Serial.print(color.blue);
          matrix.drawPixel(x + left, y + top, color);
          // Serial.println("... done");
        }
      }

      // Serial.println("swapping matrix buffers");
      matrix.swapBuffers();

      return 15;
    }
};

#endif
