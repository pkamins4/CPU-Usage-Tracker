#ifndef READER_H
#define READER_H

#include"queue.h"
#include"logger.h"
#include"watchdog.h"
#include<pthread.h>
#include<stdio.h>
#include<time.h>


#define PATH "/proc/stat"
#define SLEEP_TIME_NSEC 50000000 //50 ms

typedef struct Reader
{
	Queue *toAnalyzer;
	Logger *loggerHandle;
	FILE *statFile;
	pthread_t watchdogHandle;
	struct timespec waitTime;
} Reader;

void* readerRun(void*);
void readerInit(Reader*);


#endif
