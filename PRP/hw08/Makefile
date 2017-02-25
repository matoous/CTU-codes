CFLAGS+=-pedantic -Wall -Werror -std=c99 -g -O0

program: main.c queue.o
	${CC} ${CFLAGS} main.c queue.o -o program
	
queue.o: queue.c queue.h
	${CC} ${CFLAGS} -c queue.c -o queue.o
	
zip:
	zip hw08-us.zip queue.h queue.c
	
clean:
	rm -f *.o
	rm -f program
	rm -f hw08-us.zip
