#ifndef LOGGER_H
#define LOGGER_H

#include "queue.h"
#include <stdio.h>

#define LOG_FILE "CPU_Tracker.log"
#define LOG_FILE_FAILURE -3

typedef struct Logger
{
	Queue *threadsInfo;
	FILE *logs;

} Logger;

void* loggerRun(void*);
int loggerInit(Logger*);
void loggerDestroy(Logger*);
void sendLog(Logger*, char*);

#endif
