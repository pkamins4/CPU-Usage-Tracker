CC=gcc
CFLAGS=-Wall -g
LFLAGS=-pthread

all: queue analyzer
queue: queue.c
	$(CC) $(CFLAGS) $(LFLAGS) -c -o $@ $^
analyzer: analyzer.c
	$(CC) $(CFLAGS) $(LFLAGS) -c -o $@ $^
