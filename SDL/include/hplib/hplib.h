/*
 Copyright © 2023 Insoft. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#ifndef HPLIB_H
#define HPLIB_H

#include "stdenm.h"
#include "stdtdef.h"

#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       240

#define SCREEN_TOP            0
#define SCREEN_MIDDLE       120
#define SCREEN_BOTTOM       239

#define SCREEN_LEFT           0
#define SCREEN_CENTRE       160
#define SCREEN_RIGHT        319

#define TOUCH_MENU          220

#define KEY_PRESS 1
#define EVENT 3

struct Event {
    int type;
    int x;
    int y;
};

struct Pt {
    int x;
    int y;
    int xOrig;
    int yOrig;
    int type;
};

struct Point {
    int x;
    int y;
};

struct Size {
    int w;
    int h;
};

struct Zone {
    int x;
    int y;
    int w;
    int h;
};

#ifdef __PPL
#define ceil  CEILING
#define floor FLOOR
#define round ROUND
#endif

#define __SCREEN_STORE G9


#endif // HPLIB_H
