#ifndef LOGGER_H
#define LOGGER_H

#include "queue.h"
#include <stdio.h>

#define LOG_FILE "CPU_Tracker.log"
#define LOG_FILE_FAILURE -3

typedef struct LoggerComm
{
	Queue *threadsInfo;
	FILE *logs;

} LoggerComm;

void* logFunction(void*);
int loggerInit(LoggerComm*);
void loggerDestroy(LoggerComm*);
void sendLog(LoggerComm*, char*);

#endif
