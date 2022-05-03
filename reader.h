#ifndef READER_H
#define READER_H
#include "queue.h"
#include <stdio.h>

#define PATH "/proc/stat"
#define SLEEP_TIME 50000000 //50 ms

typedef struct ReaderComm
{
	Queue *toAnalyzer;
	FILE *statFile;
} ReaderComm;

void* readFunction(void*);
void readerDestroy(ReaderComm*);

#endif
