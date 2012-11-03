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

#define ATTACH(procid)	\
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
		return 1;
	}

	ATTACH(procid);


	FILE *dump = fopen(file,"w+");

	long to, from;

	while (1) {

        	char string[300];
	        if (!fgets(string, 300, hp)) {
        	    printf("NO LINE FOUND\n");
	            break;
	        }

	        char perm[10];

        	sscanf(string, "%016llx-%016llx %s ", &from, &to, &perm);

	        printf("FROM %016llx TO %016llx %s\n", from, to, perm);

        	if (strstr(perm, "---p")) {
	            continue;
        	}

		long off=0;

		for(;from<to;from+=sizeof(long)){
			long tmp = ptrace(PTRACE_PEEKTEXT, procid, from, NULL);
			fwrite(&tmp, sizeof(long), 1,dump);
		}
	}

	fclose(dump);	

	DETACH(procid);

}

void args() {

	printf("ProcMod - v0.0 \n\
        -p PROCID -- Process ID to work on\n\
        -f        -- Text to find in the process\n\
        -r        -- Text to replace in the proces\n\ 
        -u        -- Convert find/replace parameters to UTF-16 (for JVM)\n\
        -d FILE   -- dumps process to file\n");

}

int main(int argc, char **argv) {

	int aflag = 0;
	int bflag = 0;
	char *cvalue = NULL;

	int index;
	int c;

	opterr = 0;

	if (argc == 1) {
		args();
		return;
	}

	int procid = -1;
	char *dump = NULL;

	while ((c = getopt(argc, argv, "d:b:c:p:f:")) != -1)
		switch (c) {
			case 'd':
				dump = optarg;
				//aflag = 1;
				//printf("%s\n", optarg);
				break;
			case 'b':
				bflag = 1;
				break;
			case 'c':
				cvalue = optarg;
				break;
			case 'p':
				procid = atoi(optarg);
				break;
			case 'f':
				break;
				/*case '?':
				   if (optopt == 'c')
				   fprintf(stderr,
				   "Option -%c requires an argument.\n",
				   optopt);
				   else if (isprint(optopt))
				   fprintf(stderr,
				   "Unknown option `-%c'.\n",
				   optopt);
				   else
				   fprintf(stderr,
				   "Unknown option character `\\x%x'.\n",
				   optopt);
				   return 1; */
			default:
				args();
				abort();
		}

	if (procid == -1) {
		args();
		return;
	}

	if (dump) {
		dumpprocess(procid, dump);
	}
	//printf("aflag = %d, bflag = %d, cvalue = %s\n", aflag, bflag, cvalue);

	//for (index = optind; index < argc; index++)
	//      printf("Non-option argument %s\n", argv[index]);

}
