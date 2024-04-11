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
#include "bitmaps.hpp"

void usage(void)
{
    std::cout << "Copyright (c) 2023 Insoft. All rights reserved\n";
    std::cout << "Adafruit GFX Pixel font creator\n";
    std::cout << "Usage: pixfont in-file -w [width] -h [height] [-o out-file]\n";
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

GFXglyph autoGFXglyphSettings(Bitmap *image)
{
    GFXglyph gfxGlyph;
    int minX, maxX, minY, maxY;
    
    if (!image || !image->data) return gfxGlyph;
    
    uint8_t *p = (uint8_t *)image->data;
    
    maxX = 0;
    maxY = 0;
    minX = image->width - 1;
    minY = image->height - 1;
    
    
    for (int y=0; y<image->height; y++) {
        for (int x=0; x<image->width; x++) {
            if (!p[x + y * image->width]) continue;
            if (x < minX) minX = x;
            if (x > maxX) maxX = x;
            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
        }
    }
    
    if (maxX < minX || maxY < minY) {
        minX = minY = 0;
        maxX = 8;
        maxY = 16;
    }
    
    gfxGlyph.offset = 0;
    gfxGlyph.width = maxX - minX + 1;
    gfxGlyph.height = maxY - minY + 1;
    gfxGlyph.xAdvance = image->width;
    gfxGlyph.dX = minX;
    gfxGlyph.dY = -image->height + minY;
    
    return gfxGlyph;
}

bool isGlyphImageBlank(const Bitmap *image)
{
    if (!image || !image->data) return true;
    
    uint8_t *p = (uint8_t *)image->data;
    for (int l=0; image->width * image->height; l++) {
        if (*p++) return false;
    }
    
    return true;
}


/**
 @brief    Writes to the file from a given  256 bitmap image into a series of ones and zeros.
 @param    image   The 256 bitmap image.
 @param    outfile   The out stream for the file
 */
size_t encode(Bitmap *image, std::vector<uint8_t> &data)
{
    uint8_t *p = (uint8_t *)image->data;
    uint8_t bitPosition = 1 << 7;
    uint8_t byte = 0;
    
    for (int i = 0; i < image->width * image->height; i++) {
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
    
    return (image->width * image->height + 7) / 8;
}




void create(std::string &filename, std::string &name, GFXfont &gfxFont, int width, int hs, int vs)
{
    Bitmap *monochrome;
    monochrome = pbm(filename);
    
    if (!monochrome) {
        std::cout << "Unable to load file " << filename << ".\n";
        return;
    }
    
    if ((width + hs) * (gfxFont.yAdvance + vs) * (gfxFont.last - gfxFont.first + 1) > monochrome->width * monochrome->height) {
        std::cout << "The extraction of glyphs from the provided bitmap image exceeds what is possible based on the image dimensions.\n";
        reset(monochrome);
        return;
    }
    
    Bitmap *image = expand(monochrome);
    if (!image) {
        std::cout << "#ERROR! Failed to Allocate Memory.\n";
        reset(monochrome);
        return;
    }
    
    reset(monochrome);
    
    std::vector<uint8_t> data;
    std::ostringstream osGlyph;
    
    uint16_t offset = 0;
    osGlyph << "const GFXglyph " << name << "_Glyphs[] PROGMEM = {"; // " << (parameters.l - parameters.f + 1) << "
    
    
    Bitmap *bitmap = createBitmap(width, gfxFont.yAdvance);
    
    
    int glyphs = gfxFont.last - gfxFont.first + 1;
    for (int n = 0; n < glyphs; n++) {
        int col = image->width / (width + hs);
        int x = (n % col) * (width + hs);
        int y = (n / col) * (gfxFont.yAdvance + vs);
        copyBitmap(bitmap, 0, 0, image, x, y, bitmap->width, bitmap->height);
        
        GFXglyph gfxGlyph = {
            offset, 
            static_cast<uint8_t>(width),
            static_cast<uint8_t>(gfxFont.yAdvance),
            static_cast<uint8_t>(width),
            0,
            static_cast<int8_t>(-gfxFont.yAdvance)
        };
        if (!isGlyphImageBlank(bitmap)) {
            
            gfxGlyph = autoGFXglyphSettings(bitmap);
            Bitmap *section = createBitmap(gfxGlyph.width, gfxGlyph.height);
            if (!section) {
                std::cout << "#ERROR! #001.\n";
                reset(bitmap);
                reset(image);
                return;
            }
            copyBitmap(section, 0, 0, bitmap, gfxGlyph.dX, gfxGlyph.dY + gfxFont.yAdvance, gfxGlyph.width, gfxGlyph.height);

            size_t length = encode(section, data);
            reset(section);
            
            
            gfxGlyph.offset = offset;
            offset += length;
        }
        
        osGlyph << "\n    { " << std::setw(5) << (int)gfxGlyph.offset << "," << std::setw(3) << (int)gfxGlyph.width << "," << std::setw(3) << (int)gfxGlyph.height << "," << std::setw(3) << (int)gfxGlyph.xAdvance << "," << std::setw(3) << (int)gfxGlyph.dX << "," << std::setw(4) << (int)gfxGlyph.dY;
        char charactor = n + gfxFont.first;
        if (charactor < ' ') charactor = ' ';
        
        if (n < glyphs - 1) {
            osGlyph << " }, // 0x" << std::setw(2) << std::hex << (n + gfxFont.first) << std::dec << " '" << charactor << "'";
        } else {
            osGlyph << " }  // 0x" << std::setw(2) << std::hex << (n + gfxFont.first) << std::dec << " '" << charactor << "'";
            osGlyph << "\n};\n";
        }
        
        
    }
    
    reset(bitmap);
    reset(image);
    
    std::ostringstream os;
    
    os << "#ifndef PROGMEM\n    #define PROGMEM /* None Arduino */\n#endif\n\n"
    << "#ifndef " << name << "_h\n"
    << "#define " << name << "_h\n\n"
    << "const uint8_t " << name << "_Bitmaps[] PROGMEM = {\n    "
    << std::setfill('0') << std::setw(2) << std::hex; // " << data.size() << "
    for (int n = 0; n < data.size(); n++) {
        if (n % 12) os << " ";
        os << "0x" << std::setw(2) << (int)data.at(n);
        if (n < data.size()-1) os << ",";
        if (n % 12 == 11) os << "\n    ";
    }
    os << "\n};\n\n";
    
    os << osGlyph.str() << std::dec;
    
    os << "\nconst GFXfont " << name << " PROGMEM = {(uint8_t *) " << name << "_Bitmaps, (GFXglyph *) " << name << "_Glyphs, ";
    os << gfxFont.first << ", " << gfxFont.last << ", " << (int)gfxFont.yAdvance << "};\n\n" << "#endif /* " << name << "_h */\n";
    
    std::ofstream outfile;
    std::string path;
    
    size_t pos = filename.rfind("/");
    path = filename.substr(0, pos + 1);
    
    outfile.open(path + name + ".h", std::ios::out | std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(os.str().c_str(), os.str().length());
        outfile.close();
    }
}

int main(int argc, const char * argv[])
{
    if ( argc == 1 ) {
        error();
        return 0;
    }
    
    std::string filename, name, prefix, sufix;
    
    GFXfont gfxFont = { 0, 0, 32, 95, 8 };
    int width = 8, hs = 0, vs = 0;
    
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
                gfxFont.yAdvance = atoi(argv[n]);
                continue;
            }
            
            if ( strcmp( argv[n], "-f" ) == 0 || strcmp( argv[n], "--first" ) == 0 ) {
                if ( ++n >= argc ) {
                    error();
                    return 0;
                }
                gfxFont.first = atoi(argv[n]);
                continue;
            }
            
            if ( strcmp( argv[n], "-l" ) == 0 || strcmp( argv[n], "--last" ) == 0 ) {
                if ( ++n >= argc ) {
                    error();
                    return 0;
                }
                gfxFont.last = atoi(argv[n]);
                continue;
            }
            
            if ( strcmp( argv[n], "-vs" ) == 0 ) {
                if ( ++n >= argc ) {
                    error();
                    return 0;
                }
                vs = atoi(argv[n]);
                continue;
            }
            
            if ( strcmp( argv[n], "-hs" ) == 0 ) {
                if ( ++n >= argc ) {
                    error();
                    return 0;
                }
                hs = atoi(argv[n]);
                continue;
            }
            
//            if ( strcmp( argv[n], "-x" ) == 0 ) {
//                if ( ++n >= argc ) {
//                    error();
//                    return 0;
//                }
//                parameters.x = atoi(argv[n]);
//                continue;
//            }
            
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
 
    create(filename, name, gfxFont, width, hs, vs);
    
    return 0;
}

