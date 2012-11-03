CC=gcc
CFLAGS=
#-Xlint:unchecked

all:
	#indent -linux -brf main.c -cli8
	$(CC) $(CFLAGS) main.c procmodlib.c unicode.c -o procmod

docs:

run:

clean:
	rm procmod
