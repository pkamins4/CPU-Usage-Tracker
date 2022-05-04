#ifndef PRINTER_H
#define PRINTER_H
#include<pthread.h>
#include"queue.h"
#include"logger.h"

typedef struct PrinterComm
{
	long *coreCount;
	double *averageResults;
	pthread_mutex_t *averageResultsLock;
	LoggerComm *logger;

} PrinterComm;

void* printFunction(void*);

#endif
