#ifndef PROCMODLIB
#define PROCMODLIB

#include "unicode.h"

#include<stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>


#if defined(__ILP32__)
        #define WORD int32_t
        #define WORDFORMAT "%08x"
#elif defined(__LP64__)
        #define WORD int64_t
        #define WORDFORMAT "%016lx"
#else
        #error Unsupported architecture
#endif

void dumpprocess(int procid, char *file);


#endif
