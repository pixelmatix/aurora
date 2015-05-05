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

#ifndef MenuItem_H
#define MenuItem_H

struct MenuItem {
  char *name;
  Drawable *drawable;
  bool exit = false;
  bool visible = true;
  bool audioScaleEnabled = false;
  bool playModeEnabled = false;
  bool paletteEnabled = false;

  MenuItem() {
  }

  MenuItem(char *name, Drawable *drawable) {
    MenuItem::name = name;
    MenuItem::drawable = drawable;
  }

  MenuItem(char *name, Drawable *drawable, bool exit) {
    MenuItem::name = name;
    MenuItem::drawable = drawable;
    MenuItem::exit = exit;
  }
};

#endif
