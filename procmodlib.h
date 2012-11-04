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
        #define WORD uint32_t
        #define WORDFORMAT "%08x"
	#define WORDMAX 0xFFFFFFFF
	#define TOWORD(x) strtoul(x,NULL,10)
#elif defined(__LP64__)
        #define WORD uint64_t
        #define WORDFORMAT "%016lx"
	#define WORDMAX 0xFFFFFFFFFFFFFFFF
	#define TOWORD(x) strtoul(x,NULL,10)
#else	
        #error Unsupported architecture
#endif

void dumpprocess(int procid, char *file);
int procreplace(int procid, char *find, int findlen, char *replace,
                int replacelen, WORD lowerbound, WORD upperbound);


#endif
