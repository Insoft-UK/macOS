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

#include "fonts.hpp"
#include "graphics.hpp"

using namespace fonts;

static struct {
    int w; // character width in pixels
    int h; // character height in pages (8 pixels)
    int start; // ASCII extents...
    int end;
} font[] = {
    { 6,1,32,127 },
    { 8,1,32,127 },
    { 8,2,32,127 }
};

// Size 16
static unsigned char font8x16[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x38, 0xfc, 0xfc, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x0d, 0x00, 0x00, 0x00,
    0x00, 0x06, 0x1e, 0x00, 0x00, 0x06, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0xf0, 0xf0, 0x40, 0xf0, 0xf0, 0x40, 0x00, 0x02, 0x0f, 0x0f, 0x02, 0x0f, 0x0f, 0x02, 0x00,
    0x30, 0x78, 0xc8, 0xfe, 0x88, 0x38, 0x30, 0x00, 0x06, 0x0e, 0x08, 0x3f, 0x09, 0x0f, 0x06, 0x00,
    0x18, 0x24, 0xa4, 0xd8, 0x60, 0x30, 0x18, 0x00, 0x06, 0x03, 0x01, 0x06, 0x09, 0x09, 0x06, 0x00,
    0x00, 0x98, 0xfc, 0xe4, 0x3c, 0x98, 0x80, 0x00, 0x07, 0x0f, 0x08, 0x09, 0x07, 0x0f, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x16, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xf0, 0xf8, 0x0c, 0x04, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0c, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x0c, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0c, 0x07, 0x03, 0x00, 0x00,
    0x80, 0xa0, 0xe0, 0xc0, 0xc0, 0xe0, 0xa0, 0x80, 0x00, 0x02, 0x03, 0x01, 0x01, 0x03, 0x02, 0x00,
    0x00, 0x80, 0x80, 0xe0, 0xe0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2c, 0x1c, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0xc0, 0x60, 0x30, 0x18, 0x00, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xf8, 0xfc, 0x84, 0xc4, 0x64, 0xfc, 0xf8, 0x00, 0x07, 0x0f, 0x09, 0x08, 0x08, 0x0f, 0x07, 0x00,
    0x00, 0x10, 0x18, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x0f, 0x0f, 0x08, 0x08, 0x00,
    0x08, 0x0c, 0x84, 0xc4, 0x64, 0x3c, 0x18, 0x00, 0x0e, 0x0f, 0x09, 0x08, 0x08, 0x0c, 0x0c, 0x00,
    0x08, 0x0c, 0x44, 0x44, 0x44, 0xfc, 0xb8, 0x00, 0x04, 0x0c, 0x08, 0x08, 0x08, 0x0f, 0x07, 0x00,
    0xc0, 0xe0, 0x30, 0x18, 0xfc, 0xfc, 0x00, 0x00, 0x01, 0x01, 0x01, 0x09, 0x0f, 0x0f, 0x09, 0x00,
    0x7c, 0x7c, 0x44, 0x44, 0x44, 0xc4, 0x84, 0x00, 0x04, 0x0c, 0x08, 0x08, 0x08, 0x0f, 0x07, 0x00,
    0xf8, 0xfc, 0x44, 0x44, 0x44, 0xcc, 0x88, 0x00, 0x07, 0x0f, 0x08, 0x08, 0x08, 0x0f, 0x07, 0x00,
    0x0c, 0x0c, 0x04, 0x84, 0xc4, 0x7c, 0x3c, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00,
    0xb8, 0xfc, 0x44, 0x44, 0x44, 0xfc, 0xb8, 0x00, 0x07, 0x0f, 0x08, 0x08, 0x08, 0x0f, 0x07, 0x00,
    0x78, 0xfc, 0x84, 0x84, 0x84, 0xfc, 0xf8, 0x00, 0x04, 0x0c, 0x08, 0x08, 0x08, 0x0f, 0x07, 0x00,
    0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x0e, 0x00, 0x00, 0x00,
    0x00, 0x80, 0xc0, 0x60, 0x30, 0x18, 0x08, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x0c, 0x08, 0x00,
    0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x08, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x08, 0x0c, 0x06, 0x03, 0x01, 0x00, 0x00,
    0x18, 0x1c, 0x04, 0x84, 0xc4, 0x7c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x0d, 0x00, 0x00, 0x00,
    0xe0, 0x10, 0xc8, 0x28, 0x28, 0xe8, 0x10, 0xe0, 0x03, 0x04, 0x0b, 0x0a, 0x09, 0x0b, 0x02, 0x01,
    0xf0, 0xf8, 0x8c, 0x84, 0x8c, 0xf8, 0xf0, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00,
    0x04, 0xfc, 0xfc, 0x44, 0x44, 0xfc, 0xb8, 0x00, 0x08, 0x0f, 0x0f, 0x08, 0x08, 0x0f, 0x07, 0x00,
    0xf8, 0xfc, 0x04, 0x04, 0x04, 0x1c, 0x18, 0x00, 0x07, 0x0f, 0x08, 0x08, 0x08, 0x0e, 0x06, 0x00,
    0x04, 0xfc, 0xfc, 0x04, 0x04, 0xfc, 0xf8, 0x00, 0x08, 0x0f, 0x0f, 0x08, 0x08, 0x0f, 0x07, 0x00,
    0x04, 0xfc, 0xfc, 0xc4, 0xe4, 0x0c, 0x1c, 0x00, 0x08, 0x0f, 0x0f, 0x08, 0x09, 0x0c, 0x0e, 0x00,
    0x04, 0xfc, 0xfc, 0xc4, 0xe4, 0x0c, 0x1c, 0x00, 0x08, 0x0f, 0x0f, 0x08, 0x01, 0x00, 0x00, 0x00,
    0xf8, 0xfc, 0x04, 0x04, 0x84, 0x9c, 0x98, 0x00, 0x07, 0x0f, 0x08, 0x08, 0x08, 0x0f, 0x0f, 0x00,
    0xfc, 0xfc, 0x40, 0x40, 0x40, 0xfc, 0xfc, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00,
    0x00, 0x00, 0x04, 0xfc, 0xfc, 0x04, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0f, 0x0f, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x04, 0xfc, 0xfc, 0x04, 0x00, 0x06, 0x0e, 0x08, 0x08, 0x0f, 0x07, 0x00, 0x00,
    0x04, 0xfc, 0xfc, 0xc0, 0xe0, 0x3c, 0x1c, 0x00, 0x08, 0x0f, 0x0f, 0x00, 0x01, 0x0f, 0x0e, 0x00,
    0x04, 0xfc, 0xfc, 0x04, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0f, 0x0f, 0x08, 0x08, 0x0c, 0x0e, 0x00,
    0xfc, 0xf8, 0x70, 0xe0, 0x70, 0xf8, 0xfc, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00,
    0xfc, 0xf8, 0x70, 0xe0, 0xc0, 0xfc, 0xfc, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x01, 0x0f, 0x0f, 0x00,
    0xf8, 0xfc, 0x04, 0x04, 0x04, 0xfc, 0xf8, 0x00, 0x07, 0x0f, 0x08, 0x08, 0x08, 0x0f, 0x07, 0x00,
    0x04, 0xfc, 0xfc, 0x84, 0x84, 0xfc, 0x78, 0x00, 0x08, 0x0f, 0x0f, 0x08, 0x00, 0x00, 0x00, 0x00,
    0xf8, 0xfc, 0x04, 0x04, 0x04, 0xfc, 0xf8, 0x00, 0x07, 0x0f, 0x08, 0x0e, 0x0c, 0x1f, 0x17, 0x00,
    0x04, 0xfc, 0xfc, 0x84, 0x84, 0xfc, 0x78, 0x00, 0x08, 0x0f, 0x0f, 0x00, 0x01, 0x0f, 0x0e, 0x00,
    0x18, 0x3c, 0x64, 0x44, 0xc4, 0x9c, 0x18, 0x00, 0x06, 0x0e, 0x08, 0x08, 0x08, 0x0f, 0x07, 0x00,
    0x00, 0x1c, 0x0c, 0xfc, 0xfc, 0x0c, 0x1c, 0x00, 0x00, 0x00, 0x08, 0x0f, 0x0f, 0x08, 0x00, 0x00,
    0xfc, 0xfc, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0x00, 0x07, 0x0f, 0x08, 0x08, 0x08, 0x0f, 0x07, 0x00,
    0xfc, 0xfc, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0x00, 0x01, 0x03, 0x06, 0x0c, 0x06, 0x03, 0x01, 0x00,
    0xfc, 0xfc, 0x00, 0x80, 0x00, 0xfc, 0xfc, 0x00, 0x0f, 0x07, 0x03, 0x01, 0x03, 0x07, 0x0f, 0x00,
    0x0c, 0x3c, 0xf0, 0xe0, 0xf0, 0x3c, 0x0c, 0x00, 0x0c, 0x0f, 0x03, 0x01, 0x03, 0x0f, 0x0c, 0x00,
    0x00, 0x3c, 0x7c, 0xc0, 0xc0, 0x7c, 0x3c, 0x00, 0x00, 0x00, 0x08, 0x0f, 0x0f, 0x08, 0x00, 0x00,
    0x1c, 0x0c, 0x84, 0xc4, 0x64, 0x3c, 0x1c, 0x00, 0x0e, 0x0f, 0x09, 0x08, 0x08, 0x0c, 0x0e, 0x00,
    0x00, 0x00, 0xfc, 0xfc, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x08, 0x08, 0x00, 0x00,
    0x18, 0x30, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x00,
    0x00, 0x00, 0x04, 0x04, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x0f, 0x0f, 0x00, 0x00,
    0x20, 0x30, 0x18, 0x0c, 0x18, 0x30, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x00, 0x00, 0x00, 0x0e, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xa0, 0xa0, 0xa0, 0xe0, 0xc0, 0x00, 0x00, 0x07, 0x0f, 0x08, 0x08, 0x07, 0x0f, 0x08, 0x00,
    0x04, 0xfc, 0xfc, 0x20, 0x20, 0xe0, 0xc0, 0x00, 0x00, 0x0f, 0x0f, 0x08, 0x08, 0x0f, 0x07, 0x00,
    0xc0, 0xe0, 0x20, 0x20, 0x20, 0x60, 0x40, 0x00, 0x07, 0x0f, 0x08, 0x08, 0x08, 0x0c, 0x04, 0x00,
    0xc0, 0xe0, 0x20, 0x24, 0xfc, 0xfc, 0x00, 0x00, 0x07, 0x0f, 0x08, 0x08, 0x07, 0x0f, 0x08, 0x00,
    0xc0, 0xe0, 0x20, 0x20, 0x20, 0xe0, 0xc0, 0x00, 0x07, 0x0f, 0x09, 0x09, 0x09, 0x0d, 0x05, 0x00,
    0x00, 0x20, 0xf8, 0xfc, 0x24, 0x2c, 0x08, 0x00, 0x00, 0x08, 0x0f, 0x0f, 0x08, 0x00, 0x00, 0x00,
    0xc0, 0xe0, 0x20, 0x20, 0xc0, 0xe0, 0x20, 0x00, 0x27, 0x6f, 0x48, 0x48, 0x7f, 0x3f, 0x00, 0x00,
    0x04, 0xfc, 0xfc, 0x40, 0x20, 0xe0, 0xc0, 0x00, 0x08, 0x0f, 0x0f, 0x00, 0x00, 0x0f, 0x0f, 0x00,
    0x00, 0x00, 0x20, 0xec, 0xec, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0f, 0x0f, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x20, 0xec, 0xec, 0x00, 0x00, 0x30, 0x70, 0x40, 0x40, 0x7f, 0x3f, 0x00, 0x00,
    0x04, 0xfc, 0xfc, 0x80, 0xc0, 0x60, 0x20, 0x00, 0x08, 0x0f, 0x0f, 0x01, 0x03, 0x0e, 0x0c, 0x00,
    0x00, 0x00, 0x04, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0f, 0x0f, 0x08, 0x00, 0x00,
    0xe0, 0xe0, 0x60, 0xc0, 0x60, 0xe0, 0xc0, 0x00, 0x0f, 0x0f, 0x00, 0x07, 0x00, 0x0f, 0x0f, 0x00,
    0x20, 0xe0, 0xc0, 0x20, 0x20, 0xe0, 0xc0, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x0f, 0x0f, 0x00,
    0xc0, 0xe0, 0x20, 0x20, 0x20, 0xe0, 0xc0, 0x00, 0x07, 0x0f, 0x08, 0x08, 0x08, 0x0f, 0x07, 0x00,
    0x20, 0xe0, 0xc0, 0x20, 0x20, 0xe0, 0xc0, 0x00, 0x40, 0x7f, 0x7f, 0x48, 0x08, 0x0f, 0x07, 0x00,
    0xc0, 0xe0, 0x20, 0x20, 0xe0, 0xe0, 0x00, 0x00, 0x07, 0x0f, 0x08, 0x48, 0x7f, 0x7f, 0x40, 0x00,
    0x20, 0xe0, 0xc0, 0x60, 0x20, 0x60, 0x60, 0x00, 0x08, 0x0f, 0x0f, 0x08, 0x00, 0x00, 0x00, 0x00,
    0x40, 0xe0, 0xa0, 0x20, 0x20, 0x60, 0x40, 0x00, 0x04, 0x0c, 0x09, 0x09, 0x0b, 0x0e, 0x04, 0x00,
    0x20, 0x20, 0xf8, 0xfc, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0f, 0x08, 0x04, 0x00, 0x00,
    0xe0, 0xe0, 0x00, 0x00, 0xe0, 0xe0, 0x00, 0x00, 0x07, 0x0f, 0x08, 0x08, 0x07, 0x0f, 0x08, 0x00,
    0xe0, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0x00, 0x01, 0x03, 0x06, 0x0c, 0x06, 0x03, 0x01, 0x00,
    0xe0, 0xe0, 0x00, 0xc0, 0x00, 0xe0, 0xe0, 0x00, 0x07, 0x0f, 0x0c, 0x07, 0x0c, 0x0f, 0x07, 0x00,
    0x60, 0xe0, 0x80, 0x00, 0x80, 0xe0, 0x60, 0x00, 0x0c, 0x0e, 0x03, 0x01, 0x03, 0x0e, 0x0c, 0x00,
    0xe0, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0x00, 0x47, 0x4f, 0x48, 0x48, 0x68, 0x3f, 0x1f, 0x00,
    0x60, 0x20, 0x20, 0xa0, 0xe0, 0x60, 0x20, 0x00, 0x0c, 0x0e, 0x0b, 0x09, 0x08, 0x08, 0x0c, 0x00,
    0x00, 0x40, 0x40, 0xf8, 0xbc, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0f, 0x08, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x3c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x04, 0x04, 0xbc, 0xf8, 0x40, 0x40, 0x00, 0x00, 0x08, 0x08, 0x0f, 0x07, 0x00, 0x00, 0x00,
    0x08, 0x0c, 0x04, 0x0c, 0x08, 0x0c, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Size 6
static unsigned char font6x6[] = {
  0x00,0x00,0x00,0x00,0x00,0x00, // 0x20 32
  0x5c,0x00,0x00,0x00,0x00,0x00, // 0x21 33 !
  0x06,0x00,0x06,0x00,0x00,0x00, // 0x22 34 "
  0x28,0x7c,0x28,0x7c,0x28,0x00, // 0x23 35 #
  0x5c,0x54,0xfe,0x54,0x74,0x00, // 0x24 36 $
  0x44,0x20,0x10,0x08,0x44,0x00, // 0x25 37 %
  0x28,0x54,0x54,0x20,0x50,0x00, // 0x26 38 &
  0x06,0x00,0x00,0x00,0x00,0x00, // 0x27 39 '
  0x38,0x44,0x00,0x00,0x00,0x00, // 0x28 40 (
  0x44,0x38,0x00,0x00,0x00,0x00, // 0x29 41 )
  0x02,0x07,0x02,0x00,0x00,0x00, // 0x2A 42 *
  0x10,0x10,0x7c,0x10,0x10,0x00, // 0x2B 43 +
  0xc0,0x00,0x00,0x00,0x00,0x00, // 0x2C 44 ,
  0x10,0x10,0x10,0x10,0x10,0x00, // 0x2D 45 -
  0x40,0x00,0x00,0x00,0x00,0x00, // 0x2E 46 .
  0x60,0x10,0x0c,0x00,0x00,0x00, // 0x2F 47 /
  0x7c,0x64,0x54,0x4c,0x7c,0x00, // 0x30 48 0
  0x48,0x7c,0x40,0x00,0x00,0x00, // 0x31 49 1
  0x64,0x54,0x54,0x54,0x48,0x00, // 0x32 50 2
  0x44,0x54,0x54,0x54,0x6c,0x00, // 0x33 51 3
  0x3c,0x20,0x70,0x20,0x20,0x00, // 0x34 52 4
  0x5c,0x54,0x54,0x54,0x24,0x00, // 0x35 53 5
  0x7c,0x54,0x54,0x54,0x74,0x00, // 0x36 54 6
  0x04,0x04,0x64,0x14,0x0c,0x00, // 0x37 55 7
  0x7c,0x54,0x54,0x54,0x7c,0x00, // 0x38 56 8
  0x5c,0x54,0x54,0x54,0x7c,0x00, // 0x39 57 9
  0x44,0x00,0x00,0x00,0x00,0x00, // 0x3A 58 :
  0xc4,0x00,0x00,0x00,0x00,0x00, // 0x3B 59 ;
  0x10,0x28,0x44,0x00,0x00,0x00, // 0x3C 60 <
  0x28,0x28,0x28,0x28,0x28,0x00, // 0x3D 61 =
  0x44,0x28,0x10,0x00,0x00,0x00, // 0x3E 62 >
  0x08,0x04,0x54,0x08,0x00,0x00, // 0x3F 63 ?
  0x7c,0x44,0x54,0x54,0x5c,0x00, // 0x40 64 @
  0x7c,0x24,0x24,0x24,0x7c,0x00, // 0x41 65 A
  0x7c,0x54,0x54,0x54,0x6c,0x00, // 0x42 66 B
  0x7c,0x44,0x44,0x44,0x44,0x00, // 0x43 67 C
  0x7c,0x44,0x44,0x44,0x38,0x00, // 0x44 68 D
  0x7c,0x54,0x54,0x54,0x44,0x00, // 0x45 69 E
  0x7c,0x14,0x14,0x14,0x04,0x00, // 0x46 70 F
  0x7c,0x44,0x44,0x54,0x74,0x00, // 0x47 71 G
  0x7c,0x10,0x10,0x10,0x7c,0x00, // 0x48 72 H
  0x44,0x44,0x7c,0x44,0x44,0x00, // 0x49 73 I
  0x60,0x40,0x40,0x44,0x7c,0x00, // 0x4A 74 J
  0x7c,0x10,0x10,0x28,0x44,0x00, // 0x4B 75 K
  0x7c,0x40,0x40,0x40,0x40,0x00, // 0x4C 76 L
  0x7c,0x08,0x10,0x08,0x7c,0x00, // 0x4D 77 M
  0x7c,0x08,0x10,0x20,0x7c,0x00, // 0x4E 78 N
  0x38,0x44,0x44,0x44,0x38,0x00, // 0x4F 79 O
  0x7c,0x14,0x14,0x14,0x08,0x00, // 0x50 80 P
  0x3c,0x24,0x64,0x24,0x3c,0x00, // 0x51 81 Q
  0x7c,0x14,0x14,0x14,0x68,0x00, // 0x52 82 R
  0x5c,0x54,0x54,0x54,0x74,0x00, // 0x53 83 S
  0x04,0x04,0x7c,0x04,0x04,0x00, // 0x54 84 T
  0x7c,0x40,0x40,0x40,0x7c,0x00, // 0x55 85 U
  0x0c,0x30,0x40,0x30,0x0c,0x00, // 0x56 86 V
  0x3c,0x40,0x30,0x40,0x3c,0x00, // 0x57 87 W
  0x44,0x28,0x10,0x28,0x44,0x00, // 0x58 88 X
  0x0c,0x10,0x60,0x10,0x0c,0x00, // 0x59 89 Y
  0x44,0x64,0x54,0x4c,0x44,0x00, // 0x5A 90 Z
  0x7c,0x44,0x00,0x00,0x00,0x00, // 0x5B 91 [
  0x0c,0x10,0x60,0x00,0x00,0x00, // 0x5C 92 \ backslash
  0x44,0x7c,0x00,0x00,0x00,0x00, // 0x5D 93 ]
  0x00,0x01,0x00,0x01,0x00,0x00, // 0x5E 94 ^
  0x40,0x40,0x40,0x40,0x40,0x40, // 0x5F 95 _
  0x00,0x01,0x00,0x00,0x00,0x00, // 0x60 96 `
  0x7c,0x24,0x24,0x24,0x7c,0x00, // 0x61 97 a
  0x7c,0x54,0x54,0x54,0x6c,0x00, // 0x62 98 b
  0x7c,0x44,0x44,0x44,0x44,0x00, // 0x63 99 c
  0x7c,0x44,0x44,0x44,0x38,0x00, // 0x64 100 d
  0x7c,0x54,0x54,0x54,0x44,0x00, // 0x65 101 e
  0x7c,0x14,0x14,0x14,0x04,0x00, // 0x66 102 f
  0x7c,0x44,0x44,0x54,0x74,0x00, // 0x67 103 g
  0x7c,0x10,0x10,0x10,0x7c,0x00, // 0x68 104 h
  0x44,0x44,0x7c,0x44,0x44,0x00, // 0x69 105 i
  0x60,0x40,0x40,0x44,0x7c,0x00, // 0x6A 106 j
  0x7c,0x10,0x10,0x28,0x44,0x00, // 0x6B 107 k
  0x7c,0x40,0x40,0x40,0x40,0x00, // 0x6C 108 l
  0x7c,0x08,0x10,0x08,0x7c,0x00, // 0x6D 109 m
  0x7c,0x08,0x10,0x20,0x7c,0x00, // 0x6E 110 n
  0x38,0x44,0x44,0x44,0x38,0x00, // 0x6F 111 o
  0x7c,0x14,0x14,0x14,0x08,0x00, // 0x70 112 p
  0x3c,0x24,0x64,0x24,0x3c,0x00, // 0x71 113 q
  0x7c,0x14,0x14,0x14,0x68,0x00, // 0x72 114 r
  0x5c,0x54,0x54,0x54,0x74,0x00, // 0x73 115 s
  0x04,0x04,0x7c,0x04,0x04,0x00, // 0x74 116 t
  0x7c,0x40,0x40,0x40,0x7c,0x00, // 0x75 117 u
  0x0c,0x30,0x40,0x30,0x0c,0x00, // 0x76 118 v
  0x3c,0x40,0x30,0x40,0x3c,0x00, // 0x77 119 w
  0x44,0x28,0x10,0x28,0x44,0x00, // 0x78 120 x
  0x0c,0x10,0x60,0x10,0x0c,0x00, // 0x79 121 y
  0x44,0x64,0x54,0x4c,0x44,0x00, // 0x7A 122 z
  0x10,0x7c,0x44,0x00,0x00,0x00, // 0x7B 123 {
  0x6c,0x00,0x00,0x00,0x00,0x00, // 0x7C 124 |
  0x44,0x7c,0x10,0x00,0x00,0x00, // 0x7D 125 }
  0x02,0x01,0x02,0x01,0x00,0x00, // 0x7E 126 ~
  0x00,0x00,0x00,0x00,0x00,0x00, // 0x7F 127
};


static void* glyph(unsigned char ascii, FontSize fontSize) {
    int offset = ((int)ascii - font[0].start) * (font[fontSize].w * font[fontSize].h);
   
    if (fontSize == FontSizeSmall) {
        return &font6x6[offset];
    }
    
    if (fontSize == FontSizeLarge) {
        return &font8x16[offset];
    }
    
    return 0;
}

static int drawGlyph(int x, int y, void *glyphData, uint32_t color, FontSize fontSize) {
    int w = 0;
    
    if (!glyphData) return x;
    unsigned char *data = (unsigned char *)glyphData;
    
    for (int k=0; k<font[fontSize].h; k++, y+=8) {
        for (int j=0; j<font[fontSize].w; j++) {
            if (data[j] && w < j) w = j;
            for (int i=7; i>=0; i--) {
                if (data[j] & (1 << i)) graphics::drawPixel(j + x, i + y, color);
            }
        }
        data += font[fontSize].w;
    }
    
    return w + 2;
}

int fonts::drawText(const char* text, int x, int y, FontSize fontSize, uint32_t textColor) {
    unsigned char *s = (unsigned char*)text;
    while (*s != 0) {
        if (*s == 32) {
            x += font[fontSize].w;
            s++;
            continue;
        }
        if (*s < 32 || *s > 127) {
            s++;
            continue;
        }
        x += drawGlyph(x, y, glyph(*s++, fontSize), textColor, fontSize);
    }
    return x;
}
