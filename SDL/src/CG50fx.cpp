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

#include "charset.hpp"
#include "graphics.hpp"

#include <array>

#define COPYRIGHT "Copyright © 2023 Insoft. All rights reserved"


class LIST {
private:
    std::array<double, 10000> list;
    
public:
    long operator()(int i) {
        LIST D;
        return D.list[i-1];
    }
};

void setup(void) {
    
}

void loop(void) {
    
//    TEXTOUT_P(COPYRIGHT, SCREEN_CENTRE - size.w / 2, 0, F_Small, C_Red);
    
    LIST l;
    long i = l(1);
    
    
    charset::drawText("!! Arduino", 0, 0, graphics::ColorRed, graphics::ColorPink);
    screen::pixel(40, 100, graphics::ColorRed);
    
    uint32_t bitmap[4*4] = {
        0xFFFF0000,0xFF00FF00,0xFF0000FF,0xFFFF00FF,
        0xFFFFFF00,0xFF00FFFF,0xFF000000,0xFFFFFFFF,
        0x7FFF0000,0xF0FF0000,0xFFFF0000,0xFFFF0000,
        0xEFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000
    };
    graphics::drawBitmapScaled(10, 24, 4, 4, 5, 8, bitmap);
    while (true) {
        
    }
    return;
}
