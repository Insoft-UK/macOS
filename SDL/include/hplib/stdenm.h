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

#ifndef STDENM_H
#define STDENM_H

enum ThemeMode {
    TM_Light = 1,
    TM_Dark  = 2
};

enum MouseType {
    MT_New         = 0,
    MT_Complete    = 1,
    MT_Drag        = 2,
    MT_Stretch     = 3,
    MT_Rotate      = 4,
    MT_LongClick   = 5
};

enum ObjectType {
    OT_Real        = 0,
    OT_Integer     = 1,
    OT_String      = 2,
    OT_Complex     = 3,
    OT_Matrix      = 4,
    OT_Error       = 5,
    OT_List        = 6,
    OT_Function    = 8,
    OT_Unit        = 9,
    OT_Cas         = 14
};


enum EventType {
    ET_MouseDown      = 0,
    ET_MouseMove      = 1,
    ET_MouseUp        = 2,
    ET_MouseClick     = 3,
    ET_MouseStretch   = 5,
    ET_MouseRotate    = 6,
    ET_MouseLongClick = 7
};


enum KeyCode {
    KC_Apps   = 0,
    KC_Symb   = 1,
    KC_Up     = 2,
    KC_Help   = 3,
    KC_Esc    = 4,
    KC_Plot   = 5,
    KC_Left   = 6,
    KC_Right  = 7,
    KC_View   = 8,
    KC_Home   = 9,
    KC_Num    = 10,
    KC_Down   = 11,
    KC_Menu   = 12,
    KC_Cas    = 13,
    KC_Vars   = 14,
    KC_Mem    = 15,
    KC_Units  = 16,
    KC_Xttn   = 17,
    KC_Abc    = 18,
    KC_Bksp   = 19,
    KC_Power  = 20,
    KC_Sin    = 21,
    KC_Cos    = 22,
    KC_Tan    = 23,
    KC_Ln     = 24,
    KC_Log    = 25,
    KC_Sq     = 26,
    KC_Neg    = 27,
    KC_N      = 28,
    KC_Comma  = 29,
    KC_Enter  = 30,
    KC_Eex    = 31,
    KC_Digit7 = 32,
    KC_Digit8 = 33,
    KC_Digit9 = 34,
    KC_Div    = 35,
    KC_Alpha  = 36,
    KC_Digit4 = 37,
    KC_Digit5 = 38,
    KC_Digit6 = 39,
    KC_Mult   = 40,
    KC_Shift  = 41,
    KC_Digit1 = 42,
    KC_Digit2 = 43,
    KC_Digit3 = 44,
    KC_Minus  = 45,
    KC_On     = 46,
    KC_Digit0 = 47,
    KC_Dot    = 48,
    KC_Space  = 49,
    KC_Plus   = 50
};

enum IntegerBase {
    IB_Bin = 0,
    IB_Oct = 1,
    IB_Dec = 2,
    IB_Hex = 3
};

enum StartView {
    SV_Symbolic             = 0,
    SV_Plot                 = 1,
    SV_Numeric              = 2,
    SV_SymbolicSetup        = 3,
    SV_PlotSetup            = 4,
    SV_NumericSetup         = 5,
    SV_AppInfo              = 6,
    SV_ViewsMenu            = 7,
    SV_SplitScrPlotDetail   = 8,
    SV_SplitScrPlotTable    = 9,
    SV_AutoScale            = 10,
    SV_Decimal              = 11,
    SV_Integer              = 12,
    SV_Trig                 = 13,
    SV_HomeScr              = -1,
    SV_HomeModes            = -2,
    SV_MemoryManager        = -3,
    SV_AppLibrary           = -4,
    SV_MatrixCatalog        = -5,
    SV_ListCatalog          = -6,
    SV_ProgramCatalog       = -7,
    SV_NotesCatalog         = -8
};

enum SystemColor {
    SC_Black       = 0,
    SC_DarkGray    = 1,
    SC_LightGray   = 2,
    SC_White       = 3
};

enum FontSize {
    F_Current  = 0,
    F_Size10   = 1,
    F_Size12   = 2,
    F_Small    = 2,
    F_Size14   = 3,
    F_Medium   = 3,
    F_Size16   = 4,
    F_Large    = 4,
    F_Size18   = 5,
    F_Size20   = 6,
    F_Size22   = 7
};

enum Color {
    C_White        = 0x00FFFFFF,
    C_Silver       = 0x00C0C0C0,
    C_Gray         = 0x00808080,
    C_Black        = 0x00000000,
    C_Red          = 0x00FF0000,
    C_Magenta      = 0x00FF00FF,
    C_Cardinal     = 0x00D03020,
    C_Coral        = 0x00F08070,
    C_Pink         = 0x00FFC0C0,
    C_Indigo       = 0x00400080,
    C_Purple       = 0x00800080,
    C_Violet       = 0x008000FF,
    C_Lavender     = 0x00C0C0FF,
    C_NavyBlue     = 0x00000080,
    C_Blue         = 0x000000FF,
    C_Denim        = 0x001060C0,
    C_Lochmara     = 0x000070C0,
    C_SkyBlue      = 0x0080D0FF,
    C_PacificBlue  = 0x0000A0C0,
    C_Teal         = 0x00008080,
    C_Cyan         = 0x0000FFFF,
    C_Aqua         = 0x0000FFFF,
    C_FirGreen     = 0x00003000,
    C_OfficeGreen  = 0x00008000,
    C_Shamrock     = 0x0000A060,
    C_Apple        = 0x0050B040,
    C_LimeGreen    = 0x0000FF00,
    C_Chocolate    = 0x00400808,
    C_Brown        = 0x00905000,
    C_Orange       = 0x00FF8000,
    C_Olive        = 0x00808000,
    C_Tangerine    = 0x00FFD000,
    C_Yellow       = 0x00FFFF00,
    C_Khaki        = 0x00F0E090,
    C_Tan          = 0x00D0B090,
    C_Maroon       = 0x00800000,
    C_SnowWhite    = 0x00FFF0F0,
    C_Transparent  = 0xFF000000,
    C_Glass        = 0xE0000000,
    C_DarkGlass    = 0x7F000000
};

#ifdef __PPL
enum Bool {
    false = 0,
    true = 1
};
#endif

#endif // STDENM_H
