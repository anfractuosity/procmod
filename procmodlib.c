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
                printf("Waitpid failure");\
                return;\
        }\
        if (WIFSTOPPED(status)) {\
        } else {\
		printf("Stop failure");\
                return;\
	}

#define DETACH(procid) \
        ret = ptrace(PTRACE_DETACH, procid, NULL, 0);




void replacebytes(char *haystack, char *replace, int replacelen) {
    int i = 0;
    for (i = 0; i<replacelen; i++) {
        haystack[i] = replace[i];
    }
}





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

		sscanf(string, WORDFORMAT "-" WORDFORMAT " %s ", &from, &to,
		       &perm);

		printf("FROM " WORDFORMAT " TO " WORDFORMAT " %s\n", from, to,
		       perm);

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

int procreplace(int procid, unsigned char *find, int findlen, unsigned char *replace,
		int replacelen, WORD lowerbound, WORD upperbound) {

	printf("Finding bytes within 0x"WORDFORMAT " to 0x"WORDFORMAT "\n",lowerbound,upperbound);

	char heappath[100];

	sprintf(heappath, "/proc/%d/maps", procid);

	FILE *hp = fopen(heappath, "r");

	if (hp == NULL) {
		printf("Heap not found");
		return;
	}

	ATTACH(procid);

	WORD to, from;

	while (1) {

		char string[300];
		if (!fgets(string, 300, hp)) {
			printf("NO LINE FOUND\n");
			break;
		}

		char perm[10];

		sscanf(string, WORDFORMAT "-" WORDFORMAT " %s ", &from, &to,
		       &perm);

		//if(from >= lowerbound && to <= upperbound){
                //
		//}else
		//      continue;
		if(from >= lowerbound && from <= upperbound 
		 || to <= upperbound &&  to >= lowerbound){

		} else 
			continue;
		

		printf("FROM " WORDFORMAT " TO " WORDFORMAT " %s\n", from, to,
		       perm);

		if (strstr(perm, "---p")) {
			continue;
		}

		// Don't attempt to write to read only memory
		if(strstr(perm,"r--p"))
			continue;	


		WORD off = 0;
		WORD start = 0;

		
		// 1 extra WORD because replacelen < 8 needs to give result of 1, and 2 extra words for good measure ;)
		WORD mash[(replacelen/sizeof(WORD))+3];

		
		int pos = 0;
		int mval = 0;
		int starti = 0;

		for (; from < to; from += sizeof(WORD)) {

			WORD tmp = ptrace(PTRACE_PEEKTEXT, procid, from, NULL);

			if (tmp == -1)
				if (errno != 0) {
					printf
					    ("Can't read --------------------  %016llx\n",
					     from);
					return -1;
				}

			int i = 0;
			for (i = 0; i < sizeof(WORD); i++) {
				//printf("%u\n",((tmp >> (i * 8)) & 0xFF));
				if (((tmp >> (i * 8)) & 0xFF) == find[pos]) {
					// Matched charcter
					//
					if (pos == 0) {
						start = from;
						starti = i;
					}

					pos++;

					if (i == sizeof(WORD)-1 || findlen == pos) {
						mash[mval] = tmp;
						mval++;
					}

					if (findlen == pos) {
						printf
						    ("-------------- FOUND @ " WORDFORMAT "\n",start+starti);
						if(replace){
							replacebytes((char *)mash+starti,replace,replacelen);

							int z = 0;

							printf("-------------- POKING\n");
							for (z = 0; z < mval; z++) {
								if (ptrace
								    (PTRACE_POKETEXT,
								     procid,
								     start + (z * sizeof(WORD)),
								     mash[z]) == -1) {
									perror
									    ("POKE failure");
									return;
								}
							}
						}

						mval = 0;
						pos = 0;
						starti = 0;
						break;
					}
				} else {
					mval = 0;
					pos = 0;
					starti = 0;
					continue;
				}

			}

		}
	}

	fclose(hp);

	DETACH(procid);

}
