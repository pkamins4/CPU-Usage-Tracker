#ifndef ANALYZER_H
#define ANALYZER_H

#include"queue.h"
#include<stdio.h> 
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include<stdnoreturn.h>
#include<pthread.h>


#define CORE_STR_LEN 8
#define MALLOC_FAILURE -2

typedef struct CpuStat CpuStat;

typedef struct Analyzer
{	
	Queue 	*fromReader;
	double *avgRegister;
	CpuStat *current;
	CpuStat *previous;
	pthread_t analyzerThread;
	pthread_mutex_t avgRegisterMutex;
	int coreCount;
} Analyzer; 

typedef struct CpuStat
{
	char core[CORE_STR_LEN];
	int user;
	int nice;
	int system;
	int idle;
	int iowait;
	int irq;
	int softirq;
	int steal;
	int guest;
	int guest_nice;

} CpuStat;

int analyzerRun(Analyzer*);
Analyzer* analyzerInit(Queue*);
void analyzerDestroy(Analyzer*);


#endif
