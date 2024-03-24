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

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <math.h>

#include "screen.hpp"
#include "fonts.hpp"


namespace graphics {

enum Color {
    ColorWhite        = 0xFFFFFFFF,
    ColorSilver       = 0xFFC0C0C0,
    ColorGray         = 0xFF808080,
    ColorBlack        = 0xFF000000,
    ColorRed          = 0xFFFF0000,
    ColorMagenta      = 0xFFFF00FF,
    ColorCardinal     = 0xFFD03020,
    ColorCoral        = 0xFFF08070,
    ColorPink         = 0xFFFFC0C0,
    ColorIndigo       = 0xFF400080,
    ColorPurple       = 0xFF800080,
    ColorViolet       = 0xFF8000FF,
    ColorLavender     = 0xFFC0C0FF,
    ColorNavyBlue     = 0xFF000080,
    ColorBlue         = 0xFF0000FF,
    ColorDenim        = 0xFF1060C0,
    ColorLochmara     = 0xFF0070C0,
    ColorSkyBlue      = 0xFF80D0FF,
    ColorPacificBlue  = 0xFF00A0C0,
    ColorTeal         = 0xFF008080,
    ColorCyan         = 0xFF00FFFF,
    ColorAqua         = 0xFF00FFFF,
    ColorFirGreen     = 0xFF003000,
    ColorOfficeGreen  = 0xFF008000,
    ColorShamrock     = 0xFF00A060,
    ColorApple        = 0xFF50B040,
    ColorLimeGreen    = 0xFF00FF00,
    ColorChocolate    = 0xFF400808,
    ColorBrown        = 0xFF905000,
    ColorOrange       = 0xFFFF8000,
    ColorOlive        = 0xFF808000,
    ColorTangerine    = 0xFFFFD000,
    ColorYellow       = 0xFFFFFF00,
    ColorKhaki        = 0xFFF0E090,
    ColorTan          = 0xFFD0B090,
    ColorMaroon       = 0xFF800000,
    ColorSnowWhite    = 0xFFFFF0F0,
    ColorClear        = 0x00000000
};

void drawPixel(int x, int y, Uint32 color);
void copyBitmap(int sx, int sy, int sw, int width, int height, const void *data);
void drawBitmap(int sx, int sy, int sw, int width, int height, const void *data);

/**
 * Draws a 32-bit ARGB bitmap image at a given scale of x and y.
 *
 * \param dx the x destination to draw to.
 * \param dy the y destination to draw to.
 * \param width the width of the bitmap image.
 * \param height the height of the bitmap image.
 * \param scale_x the scale factor to scale the image by in the x axis.
 * \param scale_y the scale factor to scale the image by in the y axis.
 * \param data the  32-bit ARGB bitmap.
 *
 * \since This function is available since HP Prime Lib 1.0
 */
void drawBitmapScaled(int dx, int dy, int width, int height, float scale_x, float scale_y, const void *data);

};


#endif /* GRAPHICS_HPP */
