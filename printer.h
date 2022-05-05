#ifndef PRINTER_H
#define PRINTER_H
#include<pthread.h>
#include"queue.h"
#include"logger.h"
#include"watchdog.h"

typedef struct PrinterComm
{
	long *coreCount;
	double *averageResults;
	pthread_mutex_t *averageResultsLock;
	LoggerComm *logger;
	pthread_t watchdogHandle;

} PrinterComm;

void* printFunction(void*);

#endif
