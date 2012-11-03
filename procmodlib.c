#include "procmodlib.h"

#define ATTACH(procid)  \
        long ret = ptrace(PTRACE_ATTACH, procid, NULL, 0); \
        if (ret == -1) { \
                printf("Failed to attach to process\n");\
                return;\
        }\
        int status;\
        int w = waitpid(procid, &status, WUNTRACED);\
        if (w == -1) {\
                perror("Waitpid failure");\
                exit(1);\
        }\
        if (WIFSTOPPED(status)) {\
                printf("OK\n");\
        } else\
                printf("NOT OK\n");\

#define DETACH(procid) \
        ret = ptrace(PTRACE_DETACH, procid, NULL, 0);


void dumpprocess(int procid, char *file) {
        printf("Dumping process....\n");

        char heappath[100];

        sprintf(heappath, "/proc/%d/maps", procid);

        FILE *hp = fopen(heappath, "r");

        if (hp == NULL) {
                printf("Heap not found");
                return;
        }

        ATTACH(procid);

        FILE *dump = fopen(file, "w+");

        WORD to, from;

        while (1) {

                char string[300];
                if (!fgets(string, 300, hp)) {
                        printf("NO LINE FOUND\n");
                        break;
                }

                char perm[10];

                sscanf(string, WORDFORMAT "-" WORDFORMAT " %s ", &from, &to, &perm);

                printf("FROM " WORDFORMAT " TO " WORDFORMAT " %s\n", from, to, perm);

                if (strstr(perm, "---p")) {
                        continue;
                }

                WORD off = 0;

                for (; from < to; from += sizeof(WORD)) {
                        WORD tmp = ptrace(PTRACE_PEEKTEXT, procid, from, NULL);
                        fwrite(&tmp, sizeof(WORD), 1, dump);
                }
        }

        fclose(dump);

        DETACH(procid);

}

