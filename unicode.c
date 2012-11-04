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

int tonum(char c){
	
	switch(c){
		case 0:
			return 0;
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 3;
		case 4:
			return 4;
		case 5:
			return 5;
		case 6:
			return 6;
		case 7:
			return 7;
		case 8:
			return 8;
		case 9:
			return 9;
		case 'a':
			return 10;
		case 'A':
			return 10;
		case 'b':
			return 11;
		case 'B':
			return 11;
		case 'c':
			return 12;
		case 'C':
			return 12;
		case 'd':
			return 13;
		case 'D':
			return 13;
		case 'e':
			return 14;
		case 'E':
			return 14;
		case 'f':
			return 15;
		case 'F':
			return 15;
		default:
			return -1;
	}		

}

unsigned char * tobytes(char *hex){

	unsigned int length = strlen(hex);
	
	// Check if even
	if(length & 1 == 1){
		return NULL;
	}

	unsigned char * out = malloc((length/2)+1);

	int i = 0;
	int v = 0;

	for(i=0; i<strlen(hex);i+=2){
		int val = tonum(hex[i])*16 + tonum(hex[i+1])*1;
	
		if(val == -1){
			free(out);
			return NULL;
		}

		out[v] = (unsigned int)val;
		v++;
	}

	return out;

}

