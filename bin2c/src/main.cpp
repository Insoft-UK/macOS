/*
 The MIT License (MIT)
 
 Copyright (c) 2024 Insoft. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */


#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <regex>

#ifndef NAME_MAX
#define NAME_MAX 255
#endif

#define OPT_FORCE_HEX   1

int options = 1;

#define ERROR(s...) { printf("error: " s); puts("\n"); }

void char2cchar(char *buf, char c)
{
    if ((!(options & OPT_FORCE_HEX)) && ((c>=32) && (c<='~'))) {
        if (c=='\'') {
            strcpy(buf, "'\\''");
        } else if (c=='\\') {
            strcpy(buf, "'\\\\'");
        } else {
            snprintf(buf, 1024, "'%c'", (unsigned char)c);
        }
    } else {
        snprintf(buf, 1024, "0x%02x", (unsigned char)c);
    }
}

#define BUFSIZE 1024
int g_char_per_line = 32;
#define CHARS_PER_LINE g_char_per_line

void bin2c_table(long size, FILE *in, FILE *out)
{
    char buf[BUFSIZE];
    size_t s = sizeof buf;
    int i, j=0;
    char elem[16];
    
    while ((s = fread(buf, 1, s, in))) {
        for (i = 0; i < s; i++) {
            long elemlen;
            if (i % CHARS_PER_LINE == 0) {
                fputs("\t", out);
            }
            char2cchar(elem, buf[i]);
            elemlen = strlen(elem);
            fputs(elem, out);
            if (j+1 < size) {
                int k;
                fputs(",", out);
                for (k=0; k<4-elemlen; k++) fputs(" ", out);
                if ((j+1) % CHARS_PER_LINE == 0) {
                    fputs("\n", out);
                }
            } else {
                fputs("\n", out);
            }
            j++;
        }
    }
}

void bin2c(char *name, long size, FILE *in, FILE *out, char *inname)
{
    fprintf(out, "char %s[%ld] = {\n", name, size);
    bin2c_table(size, in, out);
    fputs("};\n", out);
}

void bin2h(char *name, long size, FILE *out, char *outname)
{
    fprintf(out, "#ifndef __%s\n", name);
    fprintf(out, "#define __%s\n\n", name);
    fputs("extern\n#ifdef __cplusplus\n\"C\"\n#endif\n", out);
    fprintf(out, "char %s[%ld];\n", name, size);
    fprintf(out, "\n#endif /* __%s */\n", name);
}

void syntax(void)
{
    printf("syntax: bin2c [-n dataname] infile [-c c-outfile] [-h h-outfile]\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    long l;
    FILE *in=NULL, *out=NULL, *outh=NULL;
    
    
    std::string in_filename, out_filename_c, out_filename_h;
    std::string name;
    
    for (int n = 1; n < argc; n++) {
        if (*argv[n] == '-') {
            std::string args(argv[n]);
            
            if (args == "-c") {
                if (++n > argc) syntax();
                out_filename_c = argv[n];
                continue;
            }
            
            if (args == "-h") {
                if (++n > argc) syntax();
                out_filename_h = argv[n];
                continue;
            }
            
            if (args == "-n") {
                if (++n > argc) syntax();
                name = argv[n];
                continue;
            }
        }
        in_filename = argv[n];
    }
    
    if (in_filename.empty())
        syntax();
    
    
    out_filename_c = regex_replace(in_filename, std::regex(R"(.\w+$)"), ".c");
    out_filename_h = regex_replace(in_filename, std::regex(R"(.\w+$)"), ".h");
    
    
    if (name.empty()) {
        size_t pos = in_filename.rfind("/");
        if (pos != std::string::npos)
            name = in_filename.substr(pos + 1, in_filename.length() - pos - 1);
        name = regex_replace(name, std::regex(R"(.\w+$)"), "");
    }
    
    in=fopen(in_filename.c_str(), "rb");
    if (!in) {
        ERROR("can't open input file: %s", in_filename.c_str());
        goto cleanup;
    }
    
    
    outh=fopen(out_filename_h.c_str(), "wb");
    if (!outh) {
        ERROR("can't open .h output file: %s", out_filename_h.c_str());
        goto cleanup;
    }
    
    out=fopen(out_filename_c.c_str(), "wb");
    if (!out) {
        ERROR("can't open .c output file: %s", out_filename_c.c_str());
        goto cleanup;
    }
    
    fseek(in, 0, SEEK_END);
    l=ftell(in);
    fseek(in, 0, SEEK_SET);
    
    bin2c((char *)name.c_str(), l, in, out, (char *)in_filename.c_str());
    bin2h((char *)name.c_str(), l, outh, (char *)out_filename_h.c_str());
    
cleanup:
    if (out) fclose(out);
    if (outh) fclose(out);
    if (in) fclose(in);
    
    return 0;
}
