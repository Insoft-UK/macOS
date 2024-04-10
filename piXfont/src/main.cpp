/*
 Copyright (C) 2014 by Insoft
 
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

#include <iostream>
#include <sstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <regex>
#include <fstream>
#include <iomanip>

#include "GFXFont.h"

typedef struct __attribute__((__packed__)) {
    uint16_t width;
    uint16_t height;
    void *data;
} Bitmap;

typedef struct __attribute__((__packed__)) {
    uint16_t left, right, top, bottom;
} Bounds;

template <typename T>
T swap_endian(T u)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

void usage(void)
{
    std::cout << "Copyright (c) 2023 Insoft. All rights reserved\n";
    std::cout << "Adafruit GFX Pixel font creator\n";
    std::cout << "Usage: pixfont in-file [-o out-file]\n";
}

void error(void)
{
    std::cout << "pixfont: try 'pixfont --help' for more information\n";
}

void version(void) {
#define __BUILD_NUMBER 100000
    std::cout
    << "pixfont v"
    << (unsigned)__BUILD_NUMBER / 100000 << "."
    << (unsigned)__BUILD_NUMBER / 10000 % 10 << "."
    << (unsigned)__BUILD_NUMBER / 1000 % 10 << "."
    << std::setfill('0') << std::setw(3) << (unsigned)__BUILD_NUMBER % 1000
    << "\n";
}

std::ifstream::pos_type filesize(std::string &filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    std::ifstream::pos_type pos = in.tellg();
    in.close();
    return pos;
}

Bitmap pbm(std::string &filename)
{
    std::ifstream infile;
    size_t fsize;
    Bitmap bitmap = {0};
    
    fsize = filesize(filename);
    
    infile.open(filename, std::ios::in | std::ios::binary);
    if (!infile.is_open())
        return bitmap;
    
    std::string s;
    
    getline(infile, s);
    if (s != "P4") {
        infile.close();
        return bitmap;
    }
    
    getline(infile, s);
    bitmap.width = atoi(s.c_str());
    
    getline(infile, s);
    bitmap.height = atoi(s.c_str());
    
    size_t length = ((bitmap.width + 7) >> 3) * bitmap.height;
    bitmap.data = (unsigned char *)malloc(length);
    infile.read((char *)bitmap.data, length);
    
    infile.close();
    return bitmap;
}

Bounds boundary(Bitmap &image)
{
    Bounds b = {
        .left = image.width,
        .right = 0,
        .top = image.height,
        .bottom = 0
    };
    
    uint8_t *p = (uint8_t *)image.data;
    
    for (int y=0; y<image.height; y++) {
        for (int x=0; x<image.width; x++) {
            if (p[x + y * image.width]) {
                if (x < b.left) b.left = x;
                if (x > b.right) b.right = x;
                if (y < b.top) b.top = y;
                if (y > b.bottom) b.bottom = y;
            }
        }
    }
    return b;
}

Bitmap grab(const Bitmap &image, const Bounds &bounds)
{
    Bitmap bitmap = {0};
    bitmap.width = bounds.right - bounds.left + 1;
    bitmap.height = bounds.bottom - bounds.top + 1;
    bitmap.data = (unsigned char *)malloc(bitmap.width * bitmap.height);
    
    if (!image.data) return bitmap;
    if (!bitmap.data) return bitmap;
    
    uint8_t *dest = (uint8_t *)bitmap.data;
    uint8_t *src = (uint8_t *)image.data;
    
    for (int y=0; y<bitmap.height; y++) {
        for (int x=0; x<bitmap.width; x++) {
            dest[x + y * bitmap.width] = src[bounds.left + x + (bounds.top + y) * image.width];
        }
    }
    
    return bitmap;
}

/**
 @brief    Writes to the file from a given  256 bitmap image into a series of ones and zeros.
 @param    image   The 256 bitmap image.
 @param    outfile   The out stream for the file
 */
size_t encode(Bitmap &image, std::vector<uint8_t> &data)
{
    uint8_t *p = (uint8_t *)image.data;
    uint8_t bitPosition = 1 << 7;
    uint8_t byte = 0;
    
    for (int i = 0; i < image.width * image.height; i++) {
        if(!bitPosition) bitPosition = 1 << 7;
        if (p[i])
            byte |= bitPosition;
        bitPosition >>= 1;
        if (!bitPosition) {
            data.push_back(byte);
            byte = 0;
        }
    }
    if (bitPosition) {
        data.push_back(byte);
    }
    
    return (image.width * image.height + 7) / 8;
}

/**
 @brief    Converts a given monochrome image to a 256 bitmap image.
 @param    monochrome   The monochrome image to convert.
 */
Bitmap expand(const Bitmap &monochrome)
{
    Bitmap image = {0};
    uint8_t *src = (uint8_t *)monochrome.data;
    uint8_t bitPosition = 1 << 7;
    
    image.width = monochrome.width;
    image.height = monochrome.height;
    image.data = malloc(image.width * image.height);
    if (!image.data) return image;
    
    memset(image.data, 0, image.width * image.height);
    
    uint8_t *dest = (uint8_t *)image.data;
    
    for (int y=0; y<monochrome.height; y++) {
        bitPosition = 1 << 7;
        for (int x=0; x<monochrome.width; x++) {
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

Bounds glyphBounds(const Bitmap &image, int width, int height, int n)
{
    int col = image.width / width;
    
    Bounds bounds;
    bounds.left = (n % col) * width;
    bounds.top = (n / col) * height;
    bounds.right = bounds.left + width - 1;
    bounds.bottom = bounds.top + height - 1;
    return bounds;
}


GFXglyph grabGlyph(const Bitmap &image, int width, int height, int gap, int n, std::vector<uint8_t> &data)
{
    Bounds bounds = glyphBounds(image, width, height, n);
    Bitmap bitmap = grab(image, bounds);
    
    bounds = boundary(bitmap);
    Bitmap section = grab(bitmap, bounds);
    free(bitmap.data);
    
    size_t length = encode(section, data);
    free(section.data);
    
    GFXglyph gfxGlyph;
    gfxGlyph.width = bounds.right - bounds.left + 1;
    gfxGlyph.height = bounds.bottom - bounds.top + 1;
    gfxGlyph.dX = bounds.left;
    gfxGlyph.dY = -height + bounds.top;
    if (gap < 1) {
        gfxGlyph.xAdvance = width;//g.cols + g.x + gap;
    }
    gfxGlyph.offset = (uint8_t)length;
    return gfxGlyph;
}

void create(std::string &filename, std::string &name, int width, int height, int gap)
{
    Bitmap monochrome;
    monochrome = pbm(filename);
    
    Bitmap image = expand(monochrome);
    free(monochrome.data);
    
    
    std::vector<uint8_t> data;
    std::ostringstream osBitmaps, osGlyph, osFont;
    
    uint16_t offset = 0;
    osGlyph << "const GFXglyph " << name << "_Glyphs[] PROGMEM = {";
    
    for (int n=0; n<94; n++) {
        GFXglyph fontGlyph = grabGlyph(image, width, height, gap, n, data);
        
        uint16_t length = fontGlyph.offset;
        fontGlyph.offset = offset;
        offset += length;
        
        osGlyph << "\n    { " <<
        std::setw(5) <<
        (int)fontGlyph.offset << "," <<
        std::setw(3) <<
        (int)fontGlyph.width << "," <<
        std::setw(3) <<
        (int)fontGlyph.height << "," <<
        std::setw(3) <<
        (int)fontGlyph.xAdvance << "," <<
        std::setw(3) <<
        (int)fontGlyph.dX << "," <<
        std::setw(4) <<
        (int)fontGlyph.dY;
        
        if (n<93) {
            osGlyph << " }, // 0x" << std::setw(2) << std::hex << (n + '!') << std::dec << " '" << (char)(n + '!') << "'";
        } else {
            osGlyph << " }  // 0x" << std::setw(2) << std::hex << (n + '!') << std::dec << " '" << (char)(n + '!') << "'";
            osGlyph << "\n};\n";
        }
    }
    
    free(image.data);
    
    osBitmaps << "#ifndef ARDUINO\n    #define PROGMEM\n#endif\n\n"
    << "#ifndef " << name << "_h\n"
    << "#define " << name << "_h\n\n"
    << "const uint8_t " << name << "_Bitmaps[] PROGMEM = {\n    "
    << std::setfill('0') << std::setw(2) << std::hex;
    for (int n=0; n<data.size(); n++) {
        if (n % 32) osBitmaps << " ";
        osBitmaps << "0x" << std::setw(2) << (int)data.at(n);
        if (n<data.size()-1) osBitmaps << ",";
        if (n % 32 == 31) osBitmaps << "\n    ";
    }
    osBitmaps << "\n};\n";
    
    osFont << "const GFXfont " << name << " PROGMEM = {(uint8_t *) " << name << "_Bitmaps, (GFXglyph *) " << name << "_Glyphs, ";
    osFont << 0x21 << ", " << 0x7e << ", " << height << "};\n\n"
    << "#endif /* " << name << "_h */\n";
    
    std::ofstream outfile;
    std::string path;
    
    size_t pos = filename.rfind("/");
    path = filename.substr(0, pos + 1);
    
    outfile.open(path + name + ".h", std::ios::out | std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(osBitmaps.str().c_str(), osBitmaps.str().length());
        outfile.write(osGlyph.str().c_str(), osGlyph.str().length());
        outfile.write(osFont.str().c_str(), osFont.str().length());
        outfile.close();
    }
}

int main(int argc, const char * argv[])
{
    int width, height, gap = 0;
    if ( argc == 1 ) {
        error();
        return 0;
    }
    
    std::string filename, name, prefix, sufix;
    
    for( int n = 1; n < argc; n++ ) {
        if (*argv[n] == '-') {
            if ( strcmp( argv[n], "-n" ) == 0 || strcmp( argv[n], "--name" ) == 0 ) {
                if ( n + 1 >= argc ) {
                    error();
                    exit(100);
                }
                name = argv[n + 1];
                
                n++;
                continue;
            }
            
            if ( strcmp( argv[n], "-w" ) == 0 || strcmp( argv[n], "--width" ) == 0 ) {
                if ( ++n >= argc ) {
                    error();
                    return 0;
                }
                width = atoi(argv[n]);
                continue;
            }
            
            if ( strcmp( argv[n], "-h" ) == 0 || strcmp( argv[n], "--height" ) == 0 ) {
                if ( ++n >= argc ) {
                    error();
                    return 0;
                }
                height = atoi(argv[n]);
                continue;
            }
            
            if ( strcmp( argv[n], "-g" ) == 0 || strcmp( argv[n], "--gap" ) == 0 ) {
                if ( ++n >= argc ) {
                    error();
                    return 0;
                }
                gap = atoi(argv[n]);
                continue;
            }
            
            if ( strcmp( argv[n], "--help" ) == 0 ) {
                usage();
                return 0;
            }
            
            if ( strcmp( argv[n], "--version" ) == 0 ) {
                version();
                return 0;
            }
            
            error();
            return 0;
        }
        filename = argv[n];
    }
    
    if (name.empty()) {
        name = regex_replace(filename, std::regex(R"(\.\w+$)"), "");
        size_t pos = name.rfind("/");
        name = name.substr(pos + 1, name.length() - pos);
    }
 
    
    create(filename, name, width, height, gap);
    
    return 0;
}

