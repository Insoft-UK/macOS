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

#include "graphics.hpp"
#include "fonts.hpp"

using namespace graphics;

/**
 * Returns 32-bit TrueColor ARGB.
 *
 * Based on Alpha, Red, Green and Blue components values (0 to 31)
 *
 * The Alpha Channel  1 (opaque) to 0 (transparent).
 *
 * \param color the A1R5G5B5.
 *
 * \returns An 32-bit unsigned integer number that can be used as
 *          the color parameter for a drawing function.
 */
static Uint32 convertHighColorToTrueColor(Uint16 color) {
    Uint32 argb = (Uint32)(color & 0b111110000000000) << 9 | (Uint32)(color & 0b1111100000) << 6 | (Uint32)(color & 0b11111) << 3;
    argb |= (argb & 0b000010000000100000001000) >> 3;
    argb |= (argb & 0b001000000010000000100000) >> 4;
    argb |= (argb & 0b100000001000000010000000) >> 5;
    return argb;
}

/**
 * Returns 32-bit TrueColor ARGB.
 *
 * Based on Alpha, Red, Green and Blue components values (0 to 255)
 *
 * The Alpha Channel  255 (opaque) to 0 (transparent).
 *
 * \param source the TrueColor to alpha blend.
 * \param background the TrueColor to alpha blend to.
 *
 * \returns An 32-bit unsigned integer number that can be used as
 *          the color parameter for a drawing function.
 */
static uint32_t alphaBlend(Uint32 source, Uint32 background) {
    // Alpha blending the source and background RGBA colors : ABGR32 (little endian)
    uint32_t alpha = (source >> 24);
    uint32_t rxbx = (((source & 0x00ff00ff) * alpha) + ((background & 0x00ff00ff) * (0xff - alpha))) & 0xff00ff00;
    uint32_t axgx  = ((((source >> 8) & 0x00ff00ff) * alpha) + (((background >> 8) & 0x00ff00ff) * (0xff - alpha))) & 0xff00ff00;
    
    return (rxbx >> 8) | axgx;
}

void graphics::drawPixel(int x, int y, Uint32 color) {
    screen::pixel(x, y, alphaBlend(color, screen::pixel(x, y)));
}

void graphics::copyBitmap(int sx, int sy, int sw, int width, int height, const void *data) {
    uint32_t *bitmap = (Uint32*)data;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            screen::pixel(x, y, bitmap[sw * sy + y * sw + x + sx]);
        }
    }
}

void graphics::drawBitmap(int sx, int sy, int sw, int width, int height, const void *data) {
    uint32_t *bitmap = (Uint32*)data;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            screen::pixel(x, y, alphaBlend(bitmap[sw * sy + y * sw + x + sx], screen::pixel(x, y)));
        }
    }
}



void graphics::drawBitmapScaled(int dx, int dy, int width, int height, float scale_x, float scale_y, const void *data) {
    uint32_t *bitmap = (Uint32*)data;
    
    // Iterate through the bitmap and draw it scaled
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Access pixel color data from the bitmap array
            uint32_t pixel = bitmap[y * width + x];
            
            // Draw the pixel at the scaled position
            for (float sy = 0; sy < scale_y; sy += 1.0f) {
                for (float sx = 0; sx < scale_x; sx += 1.0f) {
                    // Calculate the scaled position
                    int posX = (int)(x * scale_x + sx);
                    int posY = (int)(y * scale_y + sy);
                    drawPixel(posX + dx, posY + dy, pixel);
                }
            }
        }
    }
}









