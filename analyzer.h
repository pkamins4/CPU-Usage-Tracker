#ifndef ANALYZER_H
#define ANALYZER_H

#include"queue.h"
#include<stdio.h> 
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include<stdnoreturn.h>


#define CORE_STR_LEN 8
#define MALLOC_FAILURE -2

typedef struct CpuStat CpuStat;

typedef struct Analyzer
{	
	Queue 	*fromReader;
	Queue 	*toPrinter;
	CpuStat *current;
	CpuStat *previous;
	pthread_t analyzerThread;
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
Analyzer* analyzerInit(Queue*, Queue*);
void analyzerDestroy(Analyzer*);


#endif
