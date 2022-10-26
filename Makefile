CC=gcc
CFLAGS=-Wall
LFLAGS=-pthread

analyzer: analyzer.c
	$(CC) $(CFLAGS) $(LFLAGS) -c -o $@ $^