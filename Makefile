CC=gcc
CFLAGS=
#-Xlint:unchecked

all:
	$(CC) $(CFLAGS) main.c -o procmod

docs:

run:

clean:
	rm procmod
