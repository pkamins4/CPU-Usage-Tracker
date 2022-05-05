#ifndef ANALYZER_H
#define ANALYZER_H

#include"queue.h"
#include<stdio.h> 
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include<stdnoreturn.h>
#include"logger.h"
#include"watchdog.h"


#define CORE_STR_LEN 8
#define MALLOC_FAILURE -2


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

typedef struct AnalyzerComm
{	
	Queue *fromReader;
	LoggerComm *logger;
	CpuStat *current;
	CpuStat *previous;
	pthread_mutex_t averageResultsLock;
	long coreCount;
	double *averageResults;
	pthread_t watchdogHandle;

} AnalyzerComm; 

int StringOccuranceCount(char*, char*);
void* analyzeFunction(void*);
int analyzerInit(AnalyzerComm*);
void analyzerDestroy(AnalyzerComm*);

#endif
