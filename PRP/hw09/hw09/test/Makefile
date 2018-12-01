CFLAGS+=--std=gnu99 
#CFLAGS+=-g
CFLAGS+=-O2 -Wall -Werror -pedantic

all: txt2bin bin2txt graph_creator

graph_creator: graph_creator.c
	${CC} ${CFLAGS} $< -o $@

graph.o: graph.c graph.h
	${CC} ${CFLAGS} -c graph.c -o graph.o

txt2bin: txt2bin.c graph.o
	${CC} ${CFLAGS} $< graph.o -o $@

bin2txt: bin2txt.c graph.o
	${CC} ${CFLAGS} $< graph.o -o $@
	
clean:
	rm -f *.o
	rm -f txt2bin bin2txt graph_creator

