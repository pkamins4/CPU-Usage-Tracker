CC = gcc

GCC_FLAGS = -Wall -Wextra -Werror
CLANG_FLAGS = -Weverything
DEBUG_FLAG = -g

ifeq ($(CC),gcc)
	CFLAGS=$(GCC_FLAGS)
else
	CFLAGS=$(CLANG_FLAGS)
endif

all: cut clear

cut: main.o
	$(CC) $(CFLAGS) $(DEBUG_FLAG) main.o -o cut
main.o: main.c queue.o
	$(CC) $(CFLAGS) $(DEBUG_FLAG) main.c -c -o main.o
queue.o: queue.c queue.h
	$(CC) $(CFLAGS) $(DEBUG_FLAG) queue.c -c -o queue.o
clear: cut
	rm *.o
