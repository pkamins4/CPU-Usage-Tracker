#ifndef ANALYZER_H
#define ANALYZER_H
#include"queue.h"

#define CORE_STR_LEN 8



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
	int *coreCount;
	float *averageResults;
	CpuStat *current;
	CpuStat *previous;
	
	pthread_mutex_t *averageResultsLock;
} AnalyzerComm; 

int StringOccuranceCount(char*, char*);
void analyzeFunction(void*);
void analyzerInit(AnalyzerComm*);
void analyzerDestroy(AnalyzerComm*);

#endif
