#include "procmodlib.h"
#include "unicode.h"

void args() {

	printf("ProcMod - v0.0\n\
        -p PROCID -- Process ID to work on\n\
        -f        -- Text to find in the process\n\
        -r        -- Text to replace in the proces\n\
	-u        -- Convert find/replace parameters to UTF-16 (for JVM)\n\
        -d FILE   -- dumps process to file\n\
	-l NUM    -- Lower bound of search\n\
	-t NUM    -- Top bound of search\n");

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

	WORD lbound = 0;
	WORD ubound = WORDMAX;

	while ((c = getopt(argc, argv, "d:b:c:p:f:ur:l:t:")) != -1)
		switch (c) {
			case 'd':
				dump = optarg;
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
			case 'l':
				lbound = TOWORD(optarg);	
				break;
			case 't':
				ubound = TOWORD(optarg);
				break;
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
				    strlen(replace),lbound,ubound);
		else{
			char *f_ = asciitou16(find);
			char *r_ = asciitou16(replace);;
			procreplace(procid, f_, u16bytes(f_), r_,
		                                    u16bytes(r_), lbound, ubound);
		}

	} else if(find){
		if(!utf16)
			procreplace(procid,find,strlen(find),NULL,0,lbound,ubound);	
		else{
		       char *f_ = asciitou16(find);
		       procreplace(procid,f_,u16bytes(f_),NULL,0,lbound,ubound);
		}
	}

}
