#ifndef PRINTER_H
#define PRINTER_H
#include<pthread.h>
#include"queue.h"
#include"logger.h"
#include"watchdog.h"

typedef struct Printer
{
	long *coreCount;
	double *averageResults;
	pthread_mutex_t *averageResultsLock;
	Logger *loggerHandle;
	pthread_t watchdogHandle;

} Printer;

void* printerRun(void*);

#endif
