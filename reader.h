#ifndef READER_H
#define READER_H
#include"queue.h"
#include<stdio.h>
#include"logger.h"
#include<pthread.h>
#include<time.h>
#include"watchdog.h"


#define PATH "/proc/stat"
#define SLEEP_TIME_NSEC 50000000 //50 ms

typedef struct ReaderComm
{
	Queue *toAnalyzer;
	LoggerComm *logger;
	FILE *statFile;
	pthread_t watchdogHandle;
} ReaderComm;

void* readFunction(void*);


#endif
