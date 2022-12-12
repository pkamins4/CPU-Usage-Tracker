CC=gcc
CFLAGS=-Wall -g
LFLAGS=-pthread

all: queue analyzer reader
queue: queue.c
	$(CC) $(CFLAGS) $(LFLAGS) -c -o $@ $^
analyzer: analyzer.c
	$(CC) $(CFLAGS) $(LFLAGS) -c -o $@ $^
reader: reader.c
	$(CC) $(CFLAGS) $(LFLAGS) -c -o $@ $^
