#ifndef PRINTER_H
#define PRINTER_H
#include<pthread.h>

typedef struct PrinterComm
{
	long *coreCount;
	double *averageResults;
	pthread_mutex_t *averageResultsLock;

} PrinterComm;

void* printFunction(void*);

#endif
