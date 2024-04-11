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

#include "bitmaps.hpp"


#include <fstream>

Bitmap *pbm(std::string &filename)
{
    std::ifstream infile;
    
    Bitmap *bitmap = (Bitmap *)malloc(sizeof(Bitmap));
    if (!bitmap) {
        return nullptr;
    }
    
    infile.open(filename, std::ios::in | std::ios::binary);
    if (!infile.is_open()) {
        free(bitmap);
        return nullptr;
    }
    
    std::string s;
    
    getline(infile, s);
    if (s != "P4") {
        infile.close();
        return bitmap;
    }
    
    getline(infile, s);
    bitmap->width = atoi(s.c_str());
    
    getline(infile, s);
    bitmap->height = atoi(s.c_str());
    
    size_t length = ((bitmap->width + 7) >> 3) * bitmap->height;
    bitmap->data = (unsigned char *)malloc(length);
    
    if (!bitmap->data) {
        free(bitmap);
        infile.close();
        return nullptr;
    }
    infile.read((char *)bitmap->data, length);
    
    infile.close();
    return bitmap;
}

Bitmap *createBitmap(int w, int h)
{
    Bitmap *bitmap = (Bitmap *)malloc(sizeof(Bitmap));
    if (!bitmap) {
        return nullptr;
    }
    
    bitmap->data = malloc(w * h);
    if (!bitmap->data) {
        reset(bitmap);
        return nullptr;
    }
    
    bitmap->width = w;
    bitmap->height = h;
    
    return bitmap;
}

void copyBitmap(const Bitmap *dst, int dx, int dy, const Bitmap *src, int x, int y, uint16_t w, uint16_t h)
{
    uint8_t *d = (uint8_t *)dst->data;
    uint8_t *s = (uint8_t *)src->data;
    
    d += dx + dy * dst->width;
    s += x + y * src->width;
    while (h--) {
        for (int i=0; i<w; i++) {
            d[i] = s[i];
        }
        d += dst->width;
        s += src->width;
    }
}

/**
 @brief    Converts a given monochrome image to a 256 bitmap image.
 @param    monochrome   The monochrome image to convert.
 */
Bitmap *expand(const Bitmap *monochrome)
{
    Bitmap *image;
    image = (Bitmap *)malloc(sizeof(Bitmap));
    if (!image)
        return nullptr;
    
    uint8_t *src = (uint8_t *)monochrome->data;
    uint8_t bitPosition = 1 << 7;
    
    image->width = monochrome->width;
    image->height = monochrome->height;
    image->data = malloc(image->width * image->height);
    if (!image->data) return image;
    
    memset(image->data, 0, image->width * image->height);
    
    uint8_t *dest = (uint8_t *)image->data;
    
    for (int y=0; y<monochrome->height; y++) {
        bitPosition = 1 << 7;
        for (int x=0; x<monochrome->width; x++) {
            if (bitPosition == 0) {
                bitPosition = 1 << 7;
                src++;
            }
            *dest++ = (*src & bitPosition ? 1 : 0);
            bitPosition >>= 1;
        }
        if (bitPosition > 0) {
            src++;
        }
    }
    
    return image;
}

void reset(Bitmap *&bitmap)
{
    if (bitmap) {
        if (bitmap->data) free(bitmap->data);
        free(bitmap);
        bitmap = nullptr;
    }
}
