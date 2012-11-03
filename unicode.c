#include "unicode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char * asciitou16(char *ascii){

        char * ret = calloc(1,(strlen(ascii)*2) + 2);

        char v;
        int off = 1;

        int i = 0;
        int z = 1;
        for(i=0;i<strlen(ascii);i++){
                ret[z] = ascii[i];
                z+=2;
        }

        return ret;
}

void print_u16(char *str){

        int i = 0;
        
	for(i=1;i<u16bytes(str);i+=2){
                printf("%c",str[i]);
        }
        
	printf("\n");

}

int u16bytes(char *str){
        int count = 0;

        while(*str != *++str){
                count++;
        }

        return count;
}


