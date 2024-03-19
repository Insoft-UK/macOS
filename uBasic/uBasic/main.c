//
//  main.c
//  uBasic
//
//  Created by Richie on 06/03/2020.
//  Copyright © 2020 Insoft. All rights reserved.
//

#include "ubasic.h"

static char program[] =
"10 gosub 100\n\
20 for i = 1 to 10\n\
30 print i: print \"ok\"\n\
31 poke 200,3\n\
40 next i\n\
50 print \"end\"\n\
60 end\n\
100 print \"subroutine\"\n\
110 return\n";

int main(int argc, const char * argv[]) {
    char *ptr = (char *)program;
    for (; *ptr != 0; ptr++) {
        if (*ptr == ':') {
            *ptr++ = '\n';
            *ptr = '0';
        }
    }
    ubasic_init(program);

    do {
      ubasic_run();
    } while(!ubasic_finished());

    return 0;
}
