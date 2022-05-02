#ifndef READER_H
#define READER_H
#include "queue.h"
#include <stdio.h>

#define PATH "/proc/stat"

typedef struct ReaderComm
{
	Queue *toAnalyzer;
	FILE *statFile;
} ReaderComm;

void* readFunction(void*);
void readerDestroy(ReaderComm*);

#endif
