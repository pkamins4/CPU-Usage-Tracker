CC = gcc

GCC_FLAGS = -Wall -Wextra -Werror
CLANG_FLAGS = -Weverything

ifeq ($(CC),gcc)
	CFLAGS=$(GCC_FLAGS)
else
	CFLAGS=$(CLANG_FLAGS)
endif

all: cut clear

cut: main.o
	$(CC) $(CFLAGS) main.o -o cut
main.o: main.c queue.o
	$(CC) $(CFLAGS) main.c -c -o main.o
queue.o: queue.c queue.h
	$(CC) $(CFLAGS) queue.c -c -o queue.o
clear: cut
	rm *.o