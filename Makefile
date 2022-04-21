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
main.o: main.c
	$(CC) $(CFLAGS) main.c -c -o main.o
clear: cut
	rm *.o