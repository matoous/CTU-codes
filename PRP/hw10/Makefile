
CC:=ccache $(CC)
CFLAGS+=-O2 -g
CFLAGS+=-pedantic -Wall -Werror
LDFLAGS+=-lm

OS:=$(shell uname)

ifeq ($(OS),Linux)
   CFLAGS+=-std=gnu99
   LDFLAGS+=-lrt
endif

#OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
OBJS=\
     dijkstra.o\
     graph.o\
     pq_heap.o\

TARGET=tgraph_search

TARGET_TIME=tgraph_search-time

OBJS_ALL=$(OBJS)
OBJS_ALL+=$(TARGET).o 
OBJS_ALL+=$(TARGET_TIME).o

all: bin time

bin: $(TARGET)

time: $(TARGET_TIME)

$(OBJS_ALL): %.o: %.c
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@

$(TARGET): $(OBJS_ALL)
	$(CC) $(OBJS) $@.o $(LDFLAGS) -o $@ 

$(TARGET_TIME): $(OBJS_ALL)
	$(CC) $(OBJS) $@.o $(LDFLAGS) -o $@ 

clean:
	$(RM) $(OBJS_ALL) $(TARGET) $(TARGET_TIME)

