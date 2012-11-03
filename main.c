#include "procmodlib.h"

void args() {

	printf("ProcMod - v0.0\n\
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
	char *find = NULL;
	char *replace = NULL;
	char utf16 = 0;

	while ((c = getopt(argc, argv, "d:b:c:p:f:ur:")) != -1)
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
				find = optarg;
				break;
			case 'r':
				replace = optarg;
				break;
			case 'u':
				utf16 = 1;
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

	if (find != NULL && replace != NULL) {
		if (!utf16)
			procreplace(procid, find, strlen(find), replace,
				    strlen(replace));

	}
	//printf("aflag = %d, bflag = %d, cvalue = %s\n", aflag, bflag, cvalue);

	//for (index = optind; index < argc; index++)
	//      printf("Non-option argument %s\n", argv[index]);

}
