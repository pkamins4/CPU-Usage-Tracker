#ifndef QUEUE_H
#define QUEUE_H
#include<pthread.h>

typedef struct ReaderComm
{
	int *coreCount;
	float *averageResults;
	pthread_mutex_t *averageResultsLock;

} ReaderComm;

void* printFunction(void*);





#endif
