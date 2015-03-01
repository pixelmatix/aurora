/*
 * TPM2 support added by J.B. Langston: https://github.com/jblang/aurora
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

#ifndef StreamingMode_H
#define StreamingMode_H

/* This implements the TPM2 and TPM2.net protocols as originally described in German here:
 * http://www.ledstyles.de/index.php/Thread/18969-tpm2-Protokoll-zur-Matrix-Lichtsteuerung/
 *
 * I have translated the protocol description to English here:
 * https://gist.github.com/jblang/89e24e2655be6c463c56
 *
 * PixelController configuration for SmartMatrix with TPM2 protocol support:
 * https://gist.github.com/jblang/cccb8b4f7caedc689c49
 *
 */
 
#define tpm2Header 0xc9
#define tpm2netHeader 0x9c
#define tpm2DataFrame 0xda
#define tpm2Command 0xc0
#define tpm2Answer 0xaa
#define tpm2Footer 0x36
#define tpm2Acknowledge 0xac

class StreamingMode : public Drawable {
private:
    uint32_t lastData = 1000;

    void drawFrameTPM2() {
      int bufferSize = matrix.getScreenHeight() * matrix.getScreenWidth() * 3;
      rgb24 *buffer = matrix.backBuffer();

      // Check header
      if (Serial.read() != tpm2Header)
        return;
      
      // Only handle data frames
      if (Serial.read() != tpm2DataFrame)
        return;

      int payloadSize = (Serial.read() << 8) | Serial.read();

      // Don't allow frame to overrun buffer
      if (payloadSize > bufferSize)
        return;
          
      // Copy frame data into buffer
      int bytesReceived = Serial.readBytes((char *)buffer, payloadSize);
      
      // Make sure we received what we were promised
      if (bytesReceived != payloadSize)
        return;
      
      // Check footer
      if (Serial.read() != tpm2Footer)
        return;
      
      // If packet has been validated, swap buffers and acknowledge
        //matrix.swapBuffers();
      Serial.write(tpm2Acknowledge);
    }

    void drawFrameTPM2net() {
      /* 
       * PixelController sends tpm2.net packets over serial and always uses a payload
       * size of 170 bytes. This code is assumes that if the protocol is tpm2.net,
       * we're talking to PixelController, since most other software uses tpm2
       * instead of tpm2.net when sending over serial links. If other software sends
       * tpm2.net packets with payloads of different sizes, this code may break.
       */
       
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
        
        // Don't allow data to overrun buffer
        if (currentPacket * 170 + payloadSize > bufferSize)
          return;

        // Copy frame data into buffer at correct position
        int bytesReceived = Serial.readBytes((char *)(buffer + currentPacket * 170), payloadSize);

        // Make sure we received what we were promised
        if (bytesReceived != payloadSize)
          return;
      
        // Check footer
        if (Serial.read() != tpm2Footer)
          return;
      }

      // Once all the packets have been validated, swap buffers and acknowledge
        //matrix.swapBuffers();
      Serial.write(tpm2Acknowledge);
    }
    
  public:

    boolean haveReceivedData = false;

    boolean handleStreaming() {
        boolean receivedData = false;

        // Make sure serial data is waiting
        if (Serial.available() > 0) {
            // Check which protocol we're using
            switch (Serial.peek()) {
                case tpm2Header:
                    drawFrameTPM2();
                    lastData = millis();
                    receivedData = true;
                    break;

                case tpm2netHeader:
                    drawFrameTPM2net();
                    lastData = millis();
                    receivedData = true;
                    break;

                default:
                    // If we don't recognize the protocol, throw the byte away
                    Serial.read();
                    break;
            }
        }

        if(receivedData) {
            haveReceivedData = true;
            return true;
        }

        if (haveReceivedData && millis() - lastData < 1000) {
            return true;
        }

        haveReceivedData = false;

        return false;
    }

    unsigned int drawFrame() {
      // Make sure serial data is waiting
      if (Serial.available() > 0) {
        // Check which protocol we're using
        switch (Serial.peek()) {
          case tpm2Header:
            drawFrameTPM2();
                    // Record when the last data came in
                    lastData = millis();
            break;

          case tpm2netHeader:
            drawFrameTPM2net();
                    // Record when the last data came in
                    lastData = millis();
            break;

          default:
            // If we don't recognize the protocol, throw the byte away
            Serial.read();
                    break;
            }
        }
        else if (millis() - lastData > 1000) {
          // If it's been longer than a second since we last received data
          // blank the screen and notify that we're waiting for data.
          matrix.fillScreen({ 0, 0, 0 });
          matrix.setFont(font3x5);
          matrix.drawString(3, 24, { 255, 255, 255 }, "Waiting");
      }
      return 10;
    }
};

#endif
