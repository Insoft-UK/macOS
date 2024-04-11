/*
 Copyright © 2024 Insoft. All rights reserved.
 
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

#ifndef bitmaps_h
#define bitmaps_h

#include <iostream>
#include <sstream>
#include <stdint.h>

typedef struct __attribute__((__packed__)) {
    uint16_t width;
    uint16_t height;
    void *data;
} Bitmap;

typedef struct {
    uint16_t left, right, top, bottom;
} Bounds;

Bitmap *pbm(std::string &filename);
Bitmap *createBitmap(int w, int h);
void copyBitmap(const Bitmap *dst, int dx, int dy, const Bitmap *src, int x, int y, uint16_t w, uint16_t h);
Bitmap *expand(const Bitmap *monochrome);
void reset(Bitmap *&bitmap);

#endif /* bitmaps_h */
