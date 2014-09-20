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

#ifndef Menu_H
#define Menu_H

// Menu where a single entry is shown at a time.
class Menu {
public:

    enum PlaybackState {
        Paused,
        Autoplay,
        Random,
    };

    int menuStart = 13;
    MenuItem currentMenuItem;
    Playlist* currentPlaylist;
    Runnable* currentRunnable;
    bool isRunnable = false;

    int currentIndex = 0;
    int previousIndex = -1;

    bool visible = true;
    bool clockVisible = false;

    unsigned int playbackState = Paused;
    unsigned int autoPlayTimout = 0;
    int autoPlayDurationSeconds = 10;
    bool pausedChanged = false;
    bool showingPausedIndicator = false;
    unsigned int pauseIndicatorTimout = 0;
    unsigned int pauseIndicatorDuration = 1000;

    bool brightnessChanged = false;
    bool showingBrightnessIndicator = false;
    unsigned int brightnessIndicatorTimout = 0;
    unsigned int brightnessIndicatorDuration = 1000;

    bool updateScrollText = false;

    unsigned char scrollSpeed = 24;

    rgb24 color = CRGB(CRGB::Green);

    void run(MenuItem menuItems [], int menuItemsCount) {
        while (true) {
            if (showingPausedIndicator && millis() >= pauseIndicatorTimout) {
                showingPausedIndicator = false;
                updateScrollText = true;
            }

            if (showingBrightnessIndicator && millis() >= brightnessIndicatorTimout) {
                showingBrightnessIndicator = false;
                updateScrollText = true;
            }

            if (currentIndex != previousIndex || updateScrollText || pausedChanged || brightnessChanged) {
                updateScrollText = false;

                matrix.clearForeground();
                matrix.displayForegroundDrawing(false);

                if (currentIndex != previousIndex) {
                    if (currentMenuItem.drawable)
                        currentMenuItem.drawable->stop();

                    previousIndex = currentIndex;
                    currentMenuItem = menuItems[currentIndex];

                    if (currentMenuItem.drawable)
                        currentMenuItem.drawable->start();

                    currentPlaylist = static_cast<Playlist*>(currentMenuItem.drawable);
                    currentRunnable = static_cast<Runnable*>(currentMenuItem.drawable);
                    isRunnable = currentMenuItem.drawable->isRunnable();

                    if (playbackState != Paused) {
                        autoPlayTimout = millis() + (autoPlayDurationSeconds * 1000);
                    }
                }

                if (brightnessChanged || showingBrightnessIndicator) {
                    brightnessChanged = false;
                    matrix.setForegroundFont(gohufont11b);
                    matrix.setScrollColor({ 255, 255, 255 });
                    matrix.setScrollOffsetFromEdge(MATRIX_HEIGHT);
                    matrix.setBackgroundBrightness(backgroundBrightness);

                    int level = ((float) getBrightnessLevel() / (float) (brightnessCount - 1)) * 100;
                    if (level < 1 && brightness > 0)
                        level = 1;

                    char text[4];
                    sprintf(text, "%3d%%", level);

                    matrix.drawForegroundString(4, 11, text, true);
                }
                else if (pausedChanged || showingPausedIndicator) {
                    pausedChanged = false;
                    matrix.setForegroundFont(font3x5);
                    matrix.setScrollColor(color);
                    matrix.setScrollOffsetFromEdge(MATRIX_HEIGHT);
                    matrix.setBackgroundBrightness(backgroundBrightness);

                    switch (playbackState) {
                        case Paused:
                            matrix.drawForegroundMonoBitmap(12, 10, pauseBitmapWidth, pauseBitmapHeight, pauseBitmap, true);
                            matrix.drawForegroundString(0, 18, "  Pause", true);
                            break;
                        case Autoplay:
                            matrix.drawForegroundMonoBitmap(12, 10, playBitmapWidth, playBitmapHeight, playBitmap, true);
                            matrix.drawForegroundString(0, 18, "Autoplay", true);
                            break;
                        case Random:
                            matrix.drawForegroundMonoBitmap(12, 10, playBitmapWidth, playBitmapHeight, playBitmap, true);
                            matrix.drawForegroundString(0, 18, " Random", true);
                            break;
                    }
                }
                else if (visible) {
                    // Scroll the current item
                    char *name = currentMenuItem.name;
                    int length = strlen((const char *) name);

                    if (length * 5 > 31) {
                        matrix.setScrollMode(wrapForwardFromLeft); /* wrapForward, bounceForward, bounceReverse, stopped, off, wrapForwardFromLeft */
                    }
                    else {
                        matrix.setScrollMode(stopped);
                    }

                    matrix.setScrollSpeed(scrollSpeed);
                    matrix.setScrollFont(font5x7);
                    matrix.setScrollColor(color);
                    matrix.setScrollOffsetFromEdge(menuStart);

                    matrix.setBackgroundBrightness(backgroundBrightness);
                    matrix.scrollText(name, -1);
                }
                else if (clockVisible) {
                    matrix.scrollText("", 1);
                    matrix.setBackgroundBrightness(backgroundBrightness);
                }
                else {
                    matrix.scrollText("", 1);
                    matrix.setBackgroundBrightness(255);
                }
            }

            unsigned int requestedDelay = draw(currentIndex, menuItems, menuItemsCount);
            unsigned int requestedDelayTimeout = millis() + requestedDelay;

            while (true) {
                unsigned long irCode = readIRCode(defaultHoldDelay);

                if (irCode == IRCODE_UP) {
                    if (visible) {
                        currentIndex--;
                        if (currentIndex < 0) {
                            currentIndex = menuItemsCount - 1;
                        }
                        break;
                    }
                    /*else {
                        adjustBrightness(1);
                        }*/
                }
                else if (irCode == IRCODE_DOWN) {
                    if (visible) {
                        currentIndex++;
                        if (currentIndex >= menuItemsCount) {
                            currentIndex = 0;
                        }
                        break;
                    }
                    /*else {
                        adjustBrightness(-1);
                        }*/
                }
                else if (irCode == IRCODE_LEFT) {
                    if (currentPlaylist) {
                        if (playbackState == Random)
                            currentPlaylist->moveRandom();
                        else
                            currentPlaylist->move(-1);

                        if (playbackState != Paused) {
                            autoPlayTimout = millis() + (autoPlayDurationSeconds * 1000);
                        }
                    }
                    break;
                }
                else if (irCode == IRCODE_RIGHT) {
                    if (currentPlaylist) {
                        if (playbackState == Random)
                            currentPlaylist-> moveRandom();
                        else
                            currentPlaylist->move(1);

                        if (playbackState != Paused) {
                            autoPlayTimout = millis() + (autoPlayDurationSeconds * 1000);
                        }
                    }
                    break;
                }
                else if (irCode == IRCODE_SELECT) {
                    if (currentMenuItem.exit) {
                        currentIndex = 0;
                        previousIndex = -1;
                        if (currentMenuItem.drawable)
                            currentMenuItem.drawable->stop();
                        return;
                    }
                    else if (isRunnable) {
                        matrix.scrollText("", 1);
                        currentRunnable->run();
                    }
                    else {
                        visible = !visible;
                    }
                    updateScrollText = true;
                    break;
                }
                else if (irCode == IRCODE_POWER) {
                    bool wasHolding = isHolding;
                    if (isHolding || cycleBrightness() == 0) {
                        heldButtonHasBeenHandled();
                        powerOff();
                        previousIndex = -1;
                        if (currentMenuItem.drawable)
                            currentMenuItem.drawable->stop();
                    }

                    if (!wasHolding) {
                        brightnessChanged = true;
                        showingBrightnessIndicator = true;
                        brightnessIndicatorTimout = millis() + brightnessIndicatorDuration;
                    }
                }
                else if (irCode == IRCODE_A) { // toggle pause/play
                    playbackState++;
                    if (playbackState > Random)
                        playbackState = Paused;
                    if (playbackState != Paused) {
                        autoPlayTimout = millis() + (autoPlayDurationSeconds * 1000);
                    }
                    pausedChanged = true;
                    showingPausedIndicator = true;
                    pauseIndicatorTimout = millis() + pauseIndicatorDuration;
                }
                else if (irCode == IRCODE_B) { // cycle color pallete
                    effects.CyclePalette();
                }
                else if (irCode == IRCODE_C) { // toggle clock visibility
                    if (!visible) {
                        clockVisible = !clockVisible;
                        updateScrollText = true;
                        // if the clock is not available, show the error message again
                        clockDigitalShort.hasShownError = false;
                        if (!isTimeAvailable)
                            clockVisible = true;
                    }
                }
                else {
                    if (playbackState != Paused && millis() >= autoPlayTimout) {
                        if (currentPlaylist) {
                            if(playbackState == Autoplay)
                                currentPlaylist->move(1);
                            else
                                currentPlaylist-> moveRandom();
                        }
                        autoPlayTimout = millis() + (autoPlayDurationSeconds * 1000);
                        break;
                    }
                }

                if (millis() >= requestedDelayTimeout)
                    break;
            }
        }
    }

private:

    void animate(int currentIndex, MenuItem menuItems [], int menuItemsCount, int offset) {
        for (int i = 1; i < 7; i++) {
            drawAt(currentIndex, menuItems, menuItemsCount, menuStart + offset * i, true);
        }
    }

    unsigned int draw(int currentIndex, MenuItem menuItems [], int menuItemsCount) {
        return drawAt(currentIndex, menuItems, menuItemsCount, menuStart, false);
    }

    unsigned int drawAt(int currentIndex, MenuItem menuItems [], int menuItemsCount, int start, bool isAnimating) {
        effects.PrepareFrame();

        // account for any time spent drawing the clock, swapping buffers, etc.
        unsigned int startTime = millis();

        // draw the current item
        unsigned int requestedDelay = currentMenuItem.drawable->drawFrame();

        if (!visible && !showingPausedIndicator && !showingBrightnessIndicator && clockVisible) {
            clockDisplay.drawFrame();
        }

        effects.ShowFrame();

        unsigned int adjustment = millis() - startTime;
        requestedDelay -= adjustment;

        if (requestedDelay < 0)
            requestedDelay = 0;

        //if (!isAnimating) delay(20);
        return requestedDelay;
    }
};

#endif
