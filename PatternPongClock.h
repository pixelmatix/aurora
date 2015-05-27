/*
*
* RGB Pong Clock - Andrew Holmes @pongclock
* https://github.com/pkourany/RGBPongClock
*
* Inspired by, and shamelessly derived from
*     Nick's LED Projects
* https://123led.wordpress.com/about/
*
* Modified for SmartMatrix by Jason Coon
*
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

#ifndef PatternPongClock_H

#define BAT1_X 2                         // Pong left bat x pos (this is where the ball collision occurs, the bat is drawn 1 behind these coords)
#define BAT2_X 28
#define SHOWCLOCK 5000

class PatternPongClock : public Drawable {
  private:

    float ballpos_x, ballpos_y;
    byte erase_x = 10;  //holds ball old pos so we can erase it, set to blank area of screen initially.
    byte erase_y = 10;
    float ballvel_x, ballvel_y;
    int bat1_y = 5;  //bat starting y positions
    int bat2_y = 5;
    int bat1_target_y = 5;  //bat targets for bats to move to
    int bat2_target_y = 5;
    byte bat1_update = 1;  //flags - set to update bat position
    byte bat2_update = 1;
    byte bat1miss, bat2miss; //flags set on the minute or hour that trigger the bats to miss the ball, thus upping the score to match the time.
    byte restart = 1;   //game restart flag - set to 1 initially to setup 1st game

  public:

    PatternPongClock() {
      name = (char *)"PongClock";
    }

    void start() {
    }

    unsigned int drawFrame() {
      clockDisplay.readTime();

      matrix.fillScreen(CRGB(CRGB::Black));

      //draw pitch centre line
      int adjust = 0;
      if (time.Second % 2 == 0) adjust = 1;
      for (byte i = 0; i < 16; i++) {
        if ( i % 2 == 0 ) { //plot point if an even number
          matrix.drawPixel(16, i + adjust, { 255, 255, 255});
        }
      }

      int ampm = clockDigitalShort.twentyFourHour ? 0 : 1;
      //update score / time
      byte mins = time.Minute;
      byte hours = time.Hour;
      if (hours > 12) {
        hours = hours - ampm * 12;
      }
      if (hours < 1) {
        hours = hours + ampm * 12;
      }

      matrix.setFont(font3x5);
      char buffer[3];

      sprintf(buffer, "%02d", hours);
      matrix.drawString(8, 1, { 255, 255, 255}, buffer);

      sprintf(buffer, "%02d", mins);
      matrix.drawString(18, 1, { 255, 255, 255}, buffer);

      //if restart flag is 1, setup a new game
      if (restart) {
        //set ball start pos
        ballpos_x = 16;
        ballpos_y = random (4, 12);

        //pick random ball direction
        if (random(0, 2) > 0) {
          ballvel_x = 1;
        }
        else {
          ballvel_x = -1;
        }
        if (random(0, 2) > 0) {
          ballvel_y = 0.5;
        }
        else {
          ballvel_y = -0.5;
        }
        //draw bats in initial positions
        bat1miss = 0;
        bat2miss = 0;
        //reset game restart flag
        restart = 0;
      }

      //if coming up to the minute: secs = 59 and mins < 59, flag bat 2 (right side) to miss the return so we inc the minutes score
      if (time.Second == 59 && time.Minute < 59) {
        bat1miss = 1;
      }
      // if coming up to the hour: secs = 59  and mins = 59, flag bat 1 (left side) to miss the return, so we inc the hours score.
      if (time.Second == 59 && time.Minute == 59) {
        bat2miss = 1;
      }

      //AI - we run 2 sets of 'AI' for each bat to work out where to go to hit the ball back
      //very basic AI...
      // For each bat, First just tell the bat to move to the height of the ball when we get to a random location.
      //for bat1
      if (ballpos_x == random(18, 32)) {
        bat1_target_y = ballpos_y;
      }
      //for bat2
      if (ballpos_x == random(4, 16)) {
        bat2_target_y = ballpos_y;
      }

      //when the ball is closer to the left bat, run the ball maths to find out where the ball will land
      if (ballpos_x == 15 && ballvel_x < 0) {

        byte end_ball_y = pong_get_ball_endpoint(ballpos_x, ballpos_y, ballvel_x, ballvel_y);

        //if the miss flag is set,  then the bat needs to miss the ball when it gets to end_ball_y
        if (bat1miss == 1) {
          bat1miss = 0;
          if ( end_ball_y > 8) {
            bat1_target_y = random (0, 3);
          }
          else {
            bat1_target_y = 8 + random (0, 3);
          }
        }
        //if the miss flag isn't set,  set bat target to ball end point with some randomness so its not always hitting top of bat
        else {
          bat1_target_y = end_ball_y - random (0, 6);
          //check not less than 0
          if (bat1_target_y < 0) {
            bat1_target_y = 0;
          }
          if (bat1_target_y > 10) {
            bat1_target_y = 10;
          }
        }
      }

      //right bat AI
      //if positive velocity then predict for right bat - first just match ball height
      //when the ball is closer to the right bat, run the ball maths to find out where it will land
      if (ballpos_x == 17 && ballvel_x > 0) {

        byte end_ball_y = pong_get_ball_endpoint(ballpos_x, ballpos_y, ballvel_x, ballvel_y);

        //if flag set to miss, move bat out way of ball
        if (bat2miss == 1) {
          bat2miss = 0;
          //if ball end point above 8 then move bat down, else move it up- so either way it misses
          if (end_ball_y > 8) {
            bat2_target_y = random (0, 3);
          }
          else {
            bat2_target_y = 8 + random (0, 3);
          }
        }
        else {
          //set bat target to ball end point with some randomness
          bat2_target_y =  end_ball_y - random (0, 6);
          //ensure target between 0 and 15
          if (bat2_target_y < 0) {
            bat2_target_y = 0;
          }
          if (bat2_target_y > 10) {
            bat2_target_y = 10;
          }
        }
      }

      //move bat 1 towards target
      //if bat y greater than target y move down until hit 0 (dont go any further or bat will move off screen)
      if (bat1_y > bat1_target_y && bat1_y > 0 ) {
        bat1_y--;
        bat1_update = 1;
      }

      //if bat y less than target y move up until hit 10 (as bat is 6)
      if (bat1_y < bat1_target_y && bat1_y < 10) {
        bat1_y++;
        bat1_update = 1;
      }

      //draw bat 1
      //      if (bat1_update) {
      matrix.fillRectangle(BAT1_X - 1, bat1_y, BAT1_X, bat1_y + 5, { 255, 255, 255});
      //      }

      //move bat 2 towards target (dont go any further or bat will move off screen)
      //if bat y greater than target y move down until hit 0
      if (bat2_y > bat2_target_y && bat2_y > 0 ) {
        bat2_y--;
        bat2_update = 1;
      }

      //if bat y less than target y move up until hit max of 10 (as bat is 6)
      if (bat2_y < bat2_target_y && bat2_y < 10) {
        bat2_y++;
        bat2_update = 1;
      }

      //draw bat2
      //      if (bat2_update) {
      matrix.fillRectangle(BAT2_X + 1, bat2_y, BAT2_X + 2, bat2_y + 5, { 255, 255, 255});
      //      }

      //update the ball position using the velocity
      ballpos_x =  ballpos_x + ballvel_x;
      ballpos_y =  ballpos_y + ballvel_y;

      //check ball collision with top and bottom of screen and reverse the y velocity if either is hit
      if (ballpos_y <= 0 ) {
        ballvel_y = ballvel_y * -1;
        ballpos_y = 0; //make sure value goes no less that 0
      }

      if (ballpos_y >= 15) {
        ballvel_y = ballvel_y * -1;
        ballpos_y = 15; //make sure value goes no more than 15
      }

      //check for ball collision with bat1. check ballx is same as batx
      //and also check if bally lies within width of bat i.e. baty to baty + 6. We can use the exp if(a < b && b < c)
      if ((int)ballpos_x == BAT1_X + 1 && (bat1_y <= (int)ballpos_y && (int)ballpos_y <= bat1_y + 5) ) {

        //random if bat flicks ball to return it - and therefor changes ball velocity
        if (!random(0, 3)) { //not true = no flick - just straight rebound and no change to ball y vel
          ballvel_x = ballvel_x * -1;
        }
        else {
          bat1_update = 1;
          byte flick;  //0 = up, 1 = down.

          if (bat1_y > 1 || bat1_y < 8) {
            flick = random(0, 2);  //pick a random dir to flick - up or down
          }

          //if bat 1 or 2 away from top only flick down
          if (bat1_y <= 1 ) {
            flick = 0;   //move bat down 1 or 2 pixels
          }
          //if bat 1 or 2 away from bottom only flick up
          if (bat1_y >=  8 ) {
            flick = 1;  //move bat up 1 or 2 pixels
          }

          switch (flick) {
            //flick up
            case 0:
              bat1_target_y = bat1_target_y + random(1, 3);
              ballvel_x = ballvel_x * -1;
              if (ballvel_y < 2) {
                ballvel_y = ballvel_y + 0.2;
              }
              break;

            //flick down
            case 1:
              bat1_target_y = bat1_target_y - random(1, 3);
              ballvel_x = ballvel_x * -1;
              if (ballvel_y > 0.2) {
                ballvel_y = ballvel_y - 0.2;
              }
              break;
          }
        }
      }

      //check for ball collision with bat2. check ballx is same as batx
      //and also check if bally lies within width of bat i.e. baty to baty + 6. We can use the exp if(a < b && b < c)
      if ((int)ballpos_x == BAT2_X && (bat2_y <= (int)ballpos_y && (int)ballpos_y <= bat2_y + 5) ) {

        //random if bat flicks ball to return it - and therefor changes ball velocity
        if (!random(0, 3)) {
          ballvel_x = ballvel_x * -1;    //not true = no flick - just straight rebound and no change to ball y vel
        }
        else {
          bat1_update = 1;
          byte flick;  //0 = up, 1 = down.

          if (bat2_y > 1 || bat2_y < 8) {
            flick = random(0, 2);  //pick a random dir to flick - up or down
          }
          //if bat 1 or 2 away from top only flick down
          if (bat2_y <= 1 ) {
            flick = 0;  //move bat up 1 or 2 pixels
          }
          //if bat 1 or 2 away from bottom only flick up
          if (bat2_y >=  8 ) {
            flick = 1;   //move bat down 1 or 2 pixels
          }

          switch (flick) {
            //flick up
            case 0:
              bat2_target_y = bat2_target_y + random(1, 3);
              ballvel_x = ballvel_x * -1;
              if (ballvel_y < 2) {
                ballvel_y = ballvel_y + 0.2;
              }
              break;

            //flick down
            case 1:
              bat2_target_y = bat2_target_y - random(1, 3);
              ballvel_x = ballvel_x * -1;
              if (ballvel_y > 0.2) {
                ballvel_y = ballvel_y - 0.2;
              }
              break;
          }
        }
      }

      //plot the ball on the screen
      byte plot_x = (int)(ballpos_x + 0.5f);
      byte plot_y = (int)(ballpos_y + 0.5f);

      matrix.drawPixel(plot_x, plot_y, {255, 255, 255});

      //check if a bat missed the ball. if it did, reset the game.
      if ((int)ballpos_x == 0 || (int) ballpos_x == 32) {
        restart = 1;
      }

      return 40;
    }

    byte pong_get_ball_endpoint(float tempballpos_x, float  tempballpos_y, float  tempballvel_x, float tempballvel_y) {
      //run prediction until ball hits bat
      while (tempballpos_x > BAT1_X && tempballpos_x < BAT2_X  ) {
        tempballpos_x = tempballpos_x + tempballvel_x;
        tempballpos_y = tempballpos_y + tempballvel_y;
        //check for collisions with top / bottom
        if (tempballpos_y <= 0 || tempballpos_y >= 15) {
          tempballvel_y = tempballvel_y * -1;
        }
      }
      return tempballpos_y;
    }
};

#endif
