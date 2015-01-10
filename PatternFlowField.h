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

#ifndef PatternFlowField_H

extern Effects effects;

class PatternFlowField : public Drawable {
public:
    uint16_t x;
    uint16_t y;
    uint16_t z;

    uint16_t speed = 1;
    uint16_t scale = 26;

    static const int boidCount = 40;

    Boid boids[boidCount];
    byte hue = 0;

    void start() {
        x = random16();
        y = random16();
        z = random16();

        for (int i = 0; i < boidCount; i++) {
            boids[i] = Boid(random(MATRIX_WIDTH), 0);
        }
    }

    unsigned int drawFrame() {
        effects.DimAll(240);

        CRGB color = effects.ColorFromCurrentPalette(hue);

        for (int i = 0; i < boidCount; i++) {
            Boid * boid = &boids[i];

            int ioffset = scale * boid->location.x;
            int joffset = scale * boid->location.y;

            uint8_t angle = inoise8(x + ioffset, y + joffset, z);

            boid->velocity.x = cos8(angle) * 0.005;
            boid->velocity.y = sin8(angle) * 0.005;
            boid->update();

            //PVector location = boid->location;
            //PVector velocity = boid->velocity;
            //matrix.drawLine(location.x, location.y, location.x - velocity.x, location.y - velocity.y, color);
            matrix.drawPixel(boid->location.x, boid->location.y, color); // effects.ColorFromCurrentPalette(angle));

            if (boid->location.x < 0 ||
                boid->location.x > MATRIX_WIDTH ||
                boid->location.y < 0 ||
                boid->location.y > MATRIX_HEIGHT) {
                boid->location.x = random(MATRIX_WIDTH);
                boid->location.y = 0;
            }

        }

        hue++;

        x += speed;
        y += speed;
        z += speed;

        return 50;
    }
};

#endif
