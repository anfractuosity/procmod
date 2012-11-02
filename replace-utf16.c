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

void printf16(char *str){
	int i = 0;
	for(i=1;i<u16strlen(str);i+=2){
		printf("%c",str[i]);
	}
	printf("\n");
}

int u16strlen(char *str){
	
	int count = 0;

	while(*str != *++str){
		count++;
	}

	return count;

}


void replace1(char *haystack, char *needle, char *replace) {
 
    int z = 0;
    int pos = 0;
    int i = 0;
    for (i = 0; i < 8; i++) {
        if (haystack[i] == needle[z]) {
            if (z == 0)
                pos = i;
            z++;
        } else
            z = 0;
    }
 
    printf("start: %d\nreplace %d\n", pos, u16strlen(needle));
    for (i = 0; i < u16strlen(replace); i++) {
        haystack[pos] = replace[i];
        pos++;
    }
 
}
 
int main(int argc, char **argv) {
    printf(">>>> %d\n",u16strlen("\0h\0e\0l\0l\0o\0\0"));

    if (argc != 4) {
        printf("Arguments: PID find replace\n");
        return 1;
    }
 
    int PID = atoi(argv[1]);
    printf("PID %d\n", atoi(argv[1]));
 
    char *find = asciitou16("Pause/play"); //argv[2];
    char *replace = asciitou16("Meow");//argv[3];

    printf16(find);

    if (u16strlen(replace) > u16strlen(find)) {
        printf("Can't replace with more characters than find");
        return 1;
    }
 
    char heappath[100];
 
    sprintf(heappath, "/proc/%d/maps", PID);
 
    FILE *hp = fopen(heappath, "r");
 
    if (hp == NULL) {
        printf("Heap not found");
        return 1;
    }
    long ret = 0;
    errno = 0;
    ret = ptrace(PTRACE_ATTACH, PID, NULL, 0);
 
    if (ret == -1) {
        printf("Failed to attach to process\n");
        return;
    }
 
    if (errno) {
        printf("Failed to attach %d", errno);
        return;
    }
    int status;
    int w = waitpid(PID, &status, WUNTRACED);
 
    if (w == -1) {
        perror("Waitpid failure");
        exit(1);
    }
    if (WIFSTOPPED(status)) {
        printf("OK\n");
    } else
        printf("NOT OK\n");
 
    long from = 0;
    long to = 0;
 
    char t1, t2, t3, t4;
    long tmpv;
    char buf[500];
 
    int i = 0;
 
    //int heapfound = 1;
    while (1) {
 
        char string[300];
        if (!fgets(string, 300, hp)) {
            printf("NO LINE FOUND\n");
            goto end;
        }
 
        char perm[10];
 
        sscanf(string, "%016llx-%016llx %s ", &from, &to, &perm);
 
        printf("FROM %016llx TO %016llx %s\n", from, to, perm);
 
        if (strstr(perm, "---p")) {
            continue;
        }
 
        char findclean[100];
        sprintf(findclean, "%s", find);
 
        long *val = (long *)findclean;
        int pos = 0;
        long start = 0;
        long starto = 0;
 
        long mash[100];
        long mval = 0;
        while (1) {
 
            errno = 0;
            long tmp = ptrace(PTRACE_PEEKTEXT, PID, from, NULL);
 
            if (tmp == -1)
                if (errno != 0) {
                    printf
                        ("Can't read --------------------  %016llx\n",
                         from);
                    goto end;
                }
 
            for (i = 0; i < 8; i++) {
                if ((tmp >> (i * 8 ) & 0xFF) == find[pos]) {
                    // Matched charcter
                    if (pos == 0) {
                        start = from;
                        starto = i * 8;
                    }
 
                    pos++;
 
                    if (i == 8 - 1 || (find[pos] == 0 && find[pos-1] == 0)) {
                        mash[mval] = tmp;
                        mval++;
                    }
 
                    if (find[pos] == 0 && find[pos-1] == 0) {
                        printf
                            ("-------------- FOUND\n");
 
                        replace1((char *)mash, find,
                             replace);
 
                        int z = 0;
			
			printf("POKING %d\n",mval);
                        for (z = 0; z < mval; z++) {
                            if (ptrace
                                (PTRACE_POKETEXT,
                                 PID,
                                 start + (z * 8),
                                 mash[z]) == -1) {
                                perror("POKE failure");
                                goto end;
                            }
                        }
 
                        mval = 0;
                        pos = 0;
                        break;
                    }
                } else {
                    mval = 0;
                    pos = 0;
                    continue;
                }
 
            }
 
            from += 8;
            if (from >= to)
                break;
        }
    }
 
 end:
    ret = ptrace(PTRACE_DETACH, PID, NULL, 0);
 
}
