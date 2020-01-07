CC=gcc
CFLAGS=-I. -Wall

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

all: prod-cons

prod-cons: prod-cons.o
	gcc -pthread -Wall -g -o prod-cons prod-cons.c $(CFLAGS)

clean:
	rm -f *.o prod-cons osy05.tgz

pack:
	make clean
	tar czf osy05.tgz Makefile *.[ch]

test:
	make
	valgrind ./forkpipe < in.in
	make clean

done:
	tar czf osy05.tgz Makefile *.[ch]
	rm -f *.[ch] Makefile
