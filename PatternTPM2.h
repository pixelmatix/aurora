/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from FastLED Fire2012 example by Mark Kriegsman: https://github.com/FastLED/FastLED/tree/master/examples/Fire2012WithPalette
 * Copyright (c) 2013 FastLED
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

#ifndef PatternTPM2_H
#define PatternTPM2_H

#ifndef Effects_H
#include "Effects.h"
#endif

#define tpm2Header 0xc9
#define tpm2netHeader 0x9c
#define tpm2DataFrame 0xda
#define tpm2Command 0xc0
#define tpm2Answer 0xaa
#define tpm2Footer 0x36
#define tpm2Acknowledge 0xac


class PatternTPM2 : public Drawable {
private:
    unsigned long lastData = 0;

    void drawFrameTPM2() {
      int bufferSize = matrix.getScreenHeight() * matrix.getScreenWidth() * 3;
      rgb24 *buffer = matrix.backBuffer();

      // Check header
      if (Serial.read() != tpm2Header)
        return;
      
      byte packetType = Serial.read();
      int frameSize = (Serial.read() << 8) | Serial.read();
      
      // 0xDA is a data frame (to be copied into pixel buffer)
      if (packetType == tpm2DataFrame) {
        // Don't allow frame to overrun buffer
        if (frameSize > bufferSize)
          frameSize = bufferSize;
          
        // Read frame into buffer
        Serial.readBytes((char *)buffer, frameSize);
      }
      
      // Check footer
      if (Serial.read() != tpm2Footer)
        return;
        
      matrix.swapBuffers();
      Serial.write(tpm2Acknowledge);
    }

    void drawFrameTPM2net() {
      // PixelController implements a broken version of the tpm2Net protocol over serial
      // This code is bug-compatible with PixelController because we assume that other,
      // more well behaved software will use tpm2 instead of tpm2net
      int bufferSize = matrix.getScreenHeight() * 32 * 3;
      rgb24 *buffer = matrix.backBuffer();
      byte currentPacket = 0;
      byte totalPackets = 255;
      
      while (currentPacket < totalPackets) {
        // Check header
        if (Serial.read() != tpm2netHeader)
          return;

        // Check packet type        
        if (Serial.read() != tpm2DataFrame)
          return;  
        
        int payloadSize = (Serial.read() << 8) | Serial.read();
      
        currentPacket = Serial.read();  
        totalPackets = Serial.read();    
        
        if (currentPacket * 170 + payloadSize > bufferSize)
          return;

        int bytesReceived = Serial.readBytes((char *)(buffer + currentPacket * 170), payloadSize);
        if (bytesReceived != payloadSize)
          return;
      
        // Check footer
        if (Serial.read() != tpm2Footer)
          return;
      }

      matrix.swapBuffers();
      Serial.write(tpm2Acknowledge);
    }
    
  public:

    unsigned int drawFrame() {
      if (Serial.available() > 0) {
        lastData = millis();
        switch (Serial.peek()) {
          case tpm2Header:
            drawFrameTPM2();
            break;
          case tpm2netHeader:
            drawFrameTPM2net();
            break;
          default:
            Serial.read();
        }
      } else if (millis() - lastData > 1000) {
          matrix.fillScreen({ 0, 0, 0 });
          matrix.setFont(font3x5);
          matrix.drawString(3, 24, { 255, 255, 255 }, "Waiting");
      }
      return 10;
    }
};

#endif
