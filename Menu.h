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
extern uint8_t brightness;
extern uint8_t backgroundBrightness;
extern const uint8_t brightnessCount;
extern MessagePlayer messagePlayer;
extern ClockDisplay clockDisplay;

// Menu where a single entry is shown at a time.
class Menu {
public:

    enum PlaybackState {
        Paused,
        Autoplay,
        Random,
    };

    int menuStart = 13;
    MenuItem* currentMenuItem;
    Playlist* currentPlaylist;
    bool isPlaylist = false;
    Runnable* currentRunnable;
    bool isRunnable = false;

    int currentIndex = 0;
    int previousIndex = -1;

    bool visible = true;
    bool clockVisible = false;
    bool messageVisible = false;

    unsigned int playbackState = Paused;
    unsigned int autoPlayTimout = 0;
    int autoPlayDurationSeconds = 5;
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

    bool canMoveBack = false;

    void run(MenuItem* menuItems [], int menuItemsCount) {
        while (true) {
            if (currentIndex != previousIndex) {
                if (currentMenuItem && currentMenuItem->drawable) {
                    currentMenuItem->drawable->stop();
                }

                currentMenuItem = menuItems[currentIndex];

                if (currentMenuItem->drawable)
                    currentMenuItem->drawable->start();

                currentPlaylist = static_cast<Playlist*>(currentMenuItem->drawable);
                isPlaylist = currentMenuItem->drawable->isPlaylist();
                currentRunnable = static_cast<Runnable*>(currentMenuItem->drawable);
                isRunnable = currentMenuItem->drawable->isRunnable();

                if (playbackState != Paused) {
                    autoPlayTimout = millis() + (autoPlayDurationSeconds * 1000);
                }
            }

            unsigned int requestedDelay = draw();
            unsigned int requestedDelayTimeout = millis() + requestedDelay;

            while (true) {
                updateForeground(menuItems, menuItemsCount);

                InputCommand command = readCommand(defaultHoldDelay);

                if (command == InputCommand::Up) {
                    if (visible) {
                        currentPlaylist->stop();
                        move(menuItems, menuItemsCount, -1);
                        break;
                    }
                }
                else if (command == InputCommand::Down) {
                    if (visible) {
                        currentPlaylist->stop();
                        move(menuItems, menuItemsCount, 1);
                        break;
                    }
                }
                else if (command == InputCommand::Left) {
                    if (isPlaylist) {
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
                else if (command == InputCommand::Right) {
                    if (isPlaylist) {
                        if (playbackState == Random)
                            currentPlaylist->moveRandom();
                        else
                            currentPlaylist->move(1);

                        if (playbackState != Paused) {
                            autoPlayTimout = millis() + (autoPlayDurationSeconds * 1000);
                        }
                    }
                    break;
                }
                else if (command == InputCommand::Select) {
                    if (currentMenuItem->exit) {
                        currentIndex = 0;
                        previousIndex = -1;
                        if (currentMenuItem->drawable)
                            currentMenuItem->drawable->stop();
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
                else if (command == InputCommand::Back) {
                    if (!visible || canMoveBack) {
                        if (visible) {
                            currentIndex = 0;
                            previousIndex = -1;
                            if (currentMenuItem->drawable)
                                currentMenuItem->drawable->stop();
                            return;
                        }
                        else {
                            visible = true;
                        }
                        updateScrollText = true;
                        break;
                    }
                }
                else if (command == InputCommand::Brightness) {
                    bool wasHolding = isHolding;
                    if (isHolding || cycleBrightness() == 0) {
                        heldButtonHasBeenHandled();
                        powerOff();
                        previousIndex = -1;
                        if (currentMenuItem->drawable)
                            currentMenuItem->drawable->stop();
                    }

                    if (!wasHolding) {
                        brightnessChanged = true;
                        showingBrightnessIndicator = true;
                        brightnessIndicatorTimout = millis() + brightnessIndicatorDuration;
                    }
                }
                else if (command == InputCommand::Power) {
                    powerOff();
                    previousIndex = -1;
                    if (currentMenuItem->drawable)
                        currentMenuItem->drawable->stop();
                }
                else if (command == InputCommand::BrightnessUp) {
                    adjustBrightness(1);
                    brightnessChanged = true;
                    showingBrightnessIndicator = true;
                    brightnessIndicatorTimout = millis() + brightnessIndicatorDuration;
                }
                else if (command == InputCommand::BrightnessDown) {
                    adjustBrightness(-1);
                    brightnessChanged = true;
                    showingBrightnessIndicator = true;
                    brightnessIndicatorTimout = millis() + brightnessIndicatorDuration;
                }
                else if (command == InputCommand::PlayMode) { // toggle pause/play
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
                else if (command == InputCommand::Palette) { // cycle color pallete
                    effects.CyclePalette();
                }
                else if (command == InputCommand::Clock) { // toggle clock visibility, cycle through messages
                    if (!visible) {
                        if (isHolding) {
                            // turn off the clock or message if the button is held
                            messageVisible = false;
                            clockVisible = false;
                        }
                        else if (!clockVisible && !messageVisible) {
                            // if neither are visible, just show the current (clock or message)
                            clockVisible = messagePlayer.currentIndex == -1;
                            messageVisible = !clockVisible;
                        }
                        else {
                            // clock or message is visible, move to the next one
                            messageVisible = messagePlayer.loadNextMessage();
                            clockVisible = false;
                        }

                        updateScrollText = true;
                    }
                }
                else {
                    if (playbackState != Paused && millis() >= autoPlayTimout) {
                        if (isPlaylist) {
                            if (playbackState == Autoplay)
                                currentPlaylist->move(1);
                            else
                                currentPlaylist->moveRandom();
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

    void move(MenuItem* menuItems [], int menuItemsCount, int delta) {
        currentIndex += delta;

        if (currentIndex >= menuItemsCount) {
            currentIndex = 0;
        }
        else if (currentIndex < 0) {
            currentIndex = menuItemsCount - 1;
        }

        // skip hidden menu items
        while (!menuItems[currentIndex]->visible) {
            currentIndex += delta;
        }
    }

    void updateForeground(MenuItem* menuItems [], int menuItemsCount) {
        if (showingPausedIndicator && millis() >= pauseIndicatorTimout) {
            showingPausedIndicator = false;
            updateScrollText = true;
        }

        if (showingBrightnessIndicator && millis() >= brightnessIndicatorTimout) {
            showingBrightnessIndicator = false;
            updateScrollText = true;
        }

        if (currentIndex != previousIndex || updateScrollText || pausedChanged || brightnessChanged) {
            previousIndex = currentIndex;
            updateScrollText = false;

            matrix.clearForeground();

            if (brightnessChanged || showingBrightnessIndicator) {
                brightnessChanged = false;
                matrix.setForegroundFont(gohufont11b);
                matrix.setScrollColor({ 255, 255, 255 });
                matrix.setScrollOffsetFromTop(MATRIX_HEIGHT);
                matrix.setBackgroundBrightness(backgroundBrightness);

                int level = ((float) getBrightnessLevel() / (float) (brightnessCount - 1)) * 100;
                if (level < 1 && brightness > 0)
                    level = 10;

                char text[4];
                sprintf(text, "%3d%%", level);

                matrix.drawForegroundString(4, 11, text, true);
            }
            else if (pausedChanged || showingPausedIndicator) {
                pausedChanged = false;
                matrix.setForegroundFont(font3x5);
                matrix.setScrollColor(color);
                matrix.setScrollOffsetFromTop(MATRIX_HEIGHT);
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
                char *name = currentMenuItem->name;
                matrix.setScrollMode(wrapForwardFromLeft); /* wrapForward, bounceForward, bounceReverse, stopped, off, wrapForwardFromLeft */
                matrix.setScrollSpeed(scrollSpeed);
                matrix.setScrollFont(font5x7);
                matrix.setScrollColor(color);
                matrix.setScrollOffsetFromTop(menuStart);
                matrix.setBackgroundBrightness(backgroundBrightness);
                matrix.scrollText(name, -1);
            }
            else if (messageVisible) {
                char *name = messagePlayer.message;
                matrix.setScrollMode(messagePlayer.scrollMode);
                matrix.setScrollSpeed(messagePlayer.scrollSpeed);
                matrix.setScrollFont(messagePlayer.getFont());
                matrix.setScrollColor(messagePlayer.color);
                matrix.setScrollOffsetFromTop(messagePlayer.offsetFromTop);
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

            if (!visible && !showingPausedIndicator && !showingBrightnessIndicator && clockVisible) {
                clockDisplay.drawFrame();
            }

            matrix.displayForegroundDrawing(false);
        }
        else if (!visible && !showingPausedIndicator && !showingBrightnessIndicator && clockVisible) {
            matrix.clearForeground();

            clockDisplay.drawFrame();

            matrix.displayForegroundDrawing(false);
        }
    }

    unsigned int draw() {
        effects.PrepareFrame();

        // account for any time spent drawing the clock, swapping buffers, etc.
        unsigned int startTime = millis();

        // draw the current item
        unsigned int requestedDelay = currentMenuItem->drawable->drawFrame();

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
