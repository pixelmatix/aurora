/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from "Flocking" in "The Nature of Code" by Daniel Shiffman: http://natureofcode.com/
 * Copyright (c) 2014 Daniel Shiffman
 * http://www.shiffman.net
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

// Flocking
// Daniel Shiffman <http://www.shiffman.net>
// The Nature of Code, Spring 2009

// Demonstration of Craig Reynolds' "Flocking" behavior
// See: http://www.red3d.com/cwr/
// Rules: Cohesion, Separation, Alignment

#ifndef PatternFlock_H

class PatternFlock : public Drawable {
public:
    Boid boids[boidCount];
    Boid predator;

    PVector wind;
    byte hue = 0;
    bool predatorPresent = true;

    void start() {
        for (int i = 0; i < boidCount; i++) {
            boids[i] = Boid(15, 15);
        }

        predatorPresent = random(0, 2) >= 1;
        if (predatorPresent) {
            predator = Boid(31, 31);
            predatorPresent = true;
            predator.maxforce *= 1.6666666;
            //predator.maxspeed *= 2.0;
            predator.neighbordist = 16.0;
            predator.desiredseparation = 0.0;
        }
    }

    unsigned int drawFrame() {
        //matrix.fillScreen(CRGB::Black);
        effects.DimAll(120);

        bool applyWind = random(0, 255) > 250;
        if (applyWind) {
            wind.x = Boid::randomf();
            wind.y = Boid::randomf();
        }

        CRGB color = effects.ColorFromCurrentPalette(hue);

        for (int i = 0; i < boidCount; i++) {
            Boid * boid = &boids[i];

            if (predatorPresent) {
                // flee from predator
                boid->repelForce(predator.location, 10);
            }

            boid->run(boids);
            PVector location = boid->location;
            PVector velocity = boid->velocity;
            matrix.drawLine(location.x, location.y, location.x - velocity.x, location.y - velocity.y, color);

            if (applyWind) {
                boid->applyForce(wind);
                applyWind = false;
            }
        }

        if (predatorPresent) {
            predator.run(boids);
            color = effects.ColorFromCurrentPalette((byte) (hue + 128));
            PVector location = predator.location;
            PVector velocity = predator.velocity;
            matrix.drawLine(location.x, location.y, location.x - velocity.x, location.y - velocity.y, color);
        }

        hue++;

        return 50;
    }
};

#endif
