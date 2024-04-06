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
#include <regex>
#include <fstream>

typedef struct __attribute__((__packed__)) {
    int16_t resolution;     /*
                             resolution [0 = low res, 1 = medium res, 2 = high res]
                             Other bits may be used in the future; use a simple bit
                             test rather than checking for specific word values.
                             */
    
    int16_t palette[16];
} Degas;


template <typename T>
T swap_endian(T u) {
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

void usage(void) {
    std::cout << "Copyright (c) 2023 Insoft. All rights reserved\n";
    std::cout << "The Atari ST/STE NeoChrome image file converter\n";
    std::cout << "Usage: neo2bmp in-file [-o out-file]\n";
}

void error(void) {
    std::cout << "neo2bmp: try 'neo2bmp --help' for more information\n";
}

void version(void) {
#define __BUILD_NUMBER 100116
    std::cout
    << "neo2bmp v"
    << (unsigned)__BUILD_NUMBER / 100000 << "."
    << (unsigned)__BUILD_NUMBER / 10000 % 10 << "."
    << (unsigned)__BUILD_NUMBER / 1000 % 10 << "."
    << std::setfill('0') << std::setw(3) << (unsigned)__BUILD_NUMBER % 1000
    << "\n";
}

std::ifstream::pos_type filesize(std::string &filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    std::ifstream::pos_type pos = in.tellg();
    in.close();
    return pos;
}

size_t loadBinary(void **dest, std::string &filename) {
    size_t fsize;
    std::ifstream infile;
    
    if ((fsize = filesize(filename)) == 0) return 0;
    
    infile.open(filename, std::ios::in | std::ios::binary);
    
    if (!infile.is_open()) return 0;

    if (!(*dest = malloc(fsize))) return 0;
   
    infile.read((char *)*dest, fsize);
    infile.close();
    return fsize;
}

bool isDegas(const void *rawData, long unsigned int length) {
    Degas *degas = (Degas *)rawData;
    
    /*
     A Degas file will always be exacly 32,034 bytes in length.
     A DEGAS Elite file will always be exacly 32,066 bytes in length.
     */
    if (length != 32034 || length != 32066)
        return false;

#ifdef __LITTLE_ENDIAN__
    degas->resolution = swap_endian(degas->resolution);
#endif
    if (degas->resolution > 2) return false;
 
    return true;
}

//// PackBits Compression Algorithm
//void unpackBits(void *dst, const void *pck, size_t n) {
//    char *p = (char *)pck;
//    char *o = (char *)dst;
//    
//    int i = 0;
//    
//    while (i < n) {
//        int k = p[i];
//        if (k >= 128) {
//            k = 256 - k;
//            for (int j = 0; j <= k; ++j) {
//                *o++ = p[i+1];
//            }
//            i++;
//        } else {
//            int j;
//            for (j = 0; j <= k; ++j) {
//                *o++ = p[i+j+1];
//            }
//            i += j;
//        }
//        i++;
//    }
//}


uint8_t bitplaneToIndex(const uint16_t *bitplanes, int n) {
    uint8_t index = 0;
    for (int p=0; p<4; p++) {
        uint16_t bitplane;
#ifdef __LITTLE_ENDIAN__
        bitplane = swap_endian(bitplanes[p]);
#else
        bitplane = bitplanes[p];
#endif
        if (bitplane & 1 << n) index |= 1 << p;
    }
    return index;
}

int degasToBmp(std::string &filename, std::string &out_filename) {
    std::ofstream outfile;
    void *buf = 0;
    
    Degas *degas;
    
    unsigned char preDefinedBmpHeader[] = {
        0x42, 0x4D, 0x71, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00,
        0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x38, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x10, 0x00,
        0x00, 0x00, 0x10, 0x00, 0x00, 0x00
    };
    

    size_t length = loadBinary(&buf, filename);
    if (isDegas(buf, length))
        return 0;;
    degas = (Degas *)buf;
    
    
    outfile.open(out_filename, std::ios::out | std::ios::binary);
    if (!outfile.is_open()) {
        free(buf);
        return 0;
    }

    
    outfile.write((const char *)preDefinedBmpHeader, sizeof(preDefinedBmpHeader));
    
    for (int i=0; i<16; i++) {
#ifdef __LITTLE_ENDIAN__
        degas->palette[i] = swap_endian(degas->palette[i]);
#endif
        float r,g,b;
        
        /*
         Atari ST  :- xx xx xx xx xx R2 R1 R0  xx G2 G1 G0 xx B2 B1 B0 (be)
         Atari STE :- xx xx xx xx R0 R3 R2 R1  G0 G3 G2 G1 B0 B3 B2 B1
         */
        degas->palette[i] = (degas->palette[i] << 1 & 0xEEE) | (degas->palette[i] >> 3 & 0x111);

        r = (float)(degas->palette[i] >> 8 & 15) / 15.0;
        g = (float)(degas->palette[i] >> 4 & 15) / 15.0;
        b = (float)(degas->palette[i] & 15) / 15.0;

        outfile.put((char)(b * 255));
        outfile.put((char)(g * 255));
        outfile.put((char)(r * 255));
        outfile.put(0xFF);
    }
    
    uint16_t *bitplanes = (uint16_t *)buf;
    bitplanes += sizeof(Degas) / 2;
    for (int i=0; i<4000; i++) {
        
        // Now we convert the bitplane to a series of indice, since each one is 4-bits we do two indice at a time before writing it to file.
            uint8_t hl = 0;
            for (int n=15; n>=0;) {
                hl = bitplaneToIndex(bitplanes, n--);
                hl <<= 4;
                hl |=  bitplaneToIndex(bitplanes, n--);
                outfile.put(hl);
            }
        bitplanes += 4;
    }
    
    free(buf);
    outfile.close();
    return 0;
    
error:
    if (buf) free(buf);
    
    if (outfile.is_open())
        outfile.close();
    
    if (degas->resolution != 0) {
        std::cout << "Only low resolution supported.\n";
    }
    return -1;
}


int main(int argc, const char * argv[])
{
    if ( argc == 1 ) {
        error();
        return 0;
    }
    
    std::string ifilename, ofilename, prefix, sufix, name;
    
    for( int n = 1; n < argc; n++ ) {
        if ( strcmp( argv[n], "-o" ) == 0 || strcmp( argv[n], "--out" ) == 0 ) {
            if ( n + 1 >= argc ) {
                error();
                exit(100);
            }
            ofilename = argv[n + 1];
            if (std::string::npos == ofilename.rfind('.')) {
                ofilename += ".bmp";
            }
            
            n++;
            continue;
        }
        
        if ( strcmp( argv[n], "--help" ) == 0 ) {
            usage();
            exit(0x65);
        }
        
        if ( strcmp( argv[n], "--version" ) == 0 ) {
            version();
            return 0;
        }
        
        ifilename = argv[n];
    }
    
    if (ofilename.empty()) {
        ofilename = regex_replace(ifilename, std::regex(R"(\.\w+$)"), ".bmp");
        if (ifilename == ofilename) return -1;
    }
 
    if (degasToBmp(ifilename, ofilename) == -1) {
        std::cout << "error\n";
    }
    
    return 0;
}

