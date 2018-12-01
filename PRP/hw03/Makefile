CFLAGS+= -pedantic -Wall -Werror -std=c99 -O3

program: main.c 
	${CC} ${CFLAGS} main.c -o program
	
zip:
	zip hw03-us.zip main.c
	
clean:
	rm -f *.o
	rm -f program
	rm -f hw03-us.zip
