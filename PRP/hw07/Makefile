CFLAGS+=-pedantic -Wall -Werror -std=c99 

program: main.c lock.o
	${CC} ${CFLAGS} main.c lock.o -o program
	
lock.o: lock.c lock.h
	${CC} ${CFLAGS} -c lock.c -o lock.o
	
zip:
	zip hw07-us.zip main.c
	
clean:
	rm -f *.o
	rm -f program
	rm -f *.zip
