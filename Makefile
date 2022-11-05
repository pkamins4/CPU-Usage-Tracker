CC=gcc
CFLAGS=-Wall -g
LFLAGS=-pthread

analyzer: analyzer.c
	$(CC) $(CFLAGS) $(LFLAGS) -c -o $@ $^