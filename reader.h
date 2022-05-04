#ifndef READER_H
#define READER_H
#include "queue.h"
#include <stdio.h>
#include"logger.h"

#define PATH "/proc/stat"
#define SLEEP_TIME 50000000 //50 ms

typedef struct ReaderComm
{
	Queue *toAnalyzer;
	LoggerComm *logger;
	FILE *statFile;
} ReaderComm;

void* readFunction(void*);


#endif
