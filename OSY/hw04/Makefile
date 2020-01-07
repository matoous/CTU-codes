CC=gcc
CFLAGS=-I. -Wall

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

all: nsd forkpipe

forkpipe: forkpipe.o
	gcc -o forkpipe forkpipe.c $(CFLAGS)

nsd: nd.o nsd.o nsd_main.o nd.h nsd.h
	gcc -o nsd nd.o nsd.o nsd_main.o $(CFLAGS)

clean:
	rm -f *.o forkpipe nsd osy04.tgz

pack:
	make clean
	tar czf osy04.tgz Makefile *.[ch]

test:
	make
	./forkpipe
	make clean

done:
	tar czf osy04.tgz Makefile *.[ch]
	rm -f *.[ch] Makefile
