#ifndef PROGMEM
    #define PROGMEM /* None Arduino */
#endif

#ifndef C437_h
#define C437_h

const uint8_t C437_Bitmaps[] PROGMEM = {
    0x06, 0x03, 0x0c, 0x18, 0x78, 0xf8, 0x33, 0x03
};

const GFXglyph C437_Glyphs[] PROGMEM = {
    {     0,  8,  8,  8,  0, -10 }  // 0x 0 ' '
};

const GFXfont C437 PROGMEM = {(uint8_t *) C437_Bitmaps, (GFXglyph *) C437_Glyphs, 0, 0, 16};

#endif /* C437_h */
