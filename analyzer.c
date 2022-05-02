#include"analyzer.h"
#include"queue.h"
#include <stdio.h> 
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdnoreturn.h>


void* analyzeFunction(void *commArg)
{
	AnalyzerComm *interThreadComm = (AnalyzerComm*)commArg;
	char statBuffer[DATA_LENGTH];
	char *buffPointer = NULL;
	int i;
	double idle, prevIdle, total, prevTotal, totalDiff;

	
	while(1)
	{

		dequeue(interThreadComm->fromReader, statBuffer);
		
		buffPointer = statBuffer;
		for( i = 0 ; i < interThreadComm->coreCount ; i++)
		{
			sscanf(buffPointer, "%s %i %i %i %i %i %i %i %i %i %i",
				 interThreadComm->current[i].core,
				&interThreadComm->current[i].user,
				&interThreadComm->current[i].nice,
				&interThreadComm->current[i].system,
				&interThreadComm->current[i].idle,
				&interThreadComm->current[i].iowait,
				&interThreadComm->current[i].irq,
				&interThreadComm->current[i].softirq,
				&interThreadComm->current[i].steal,
				&interThreadComm->current[i].guest,
				&interThreadComm->current[i].guest_nice);

			buffPointer = strchr(buffPointer, '\n');
			buffPointer++;
		}

		pthread_mutex_lock(&interThreadComm->averageResultsLock);
		for( i = 0 ; i < interThreadComm->coreCount ; i++)
		{
			idle = (double)(interThreadComm->current[i].idle + interThreadComm->current[i].iowait);
			prevIdle = (double)(interThreadComm->previous[i].idle + interThreadComm->previous[i].iowait);

			total =  (double)(interThreadComm->current[i].user
					+interThreadComm->current[i].nice
					+interThreadComm->current[i].system
					+interThreadComm->current[i].idle
					+interThreadComm->current[i].iowait
					+interThreadComm->current[i].irq
					+interThreadComm->current[i].softirq
					+interThreadComm->current[i].steal);

			prevTotal =  (double)(interThreadComm->previous[i].user
					+interThreadComm->previous[i].nice
					+interThreadComm->previous[i].system
					+interThreadComm->previous[i].idle
					+interThreadComm->previous[i].iowait
					+interThreadComm->previous[i].irq
					+interThreadComm->previous[i].softirq
					+interThreadComm->previous[i].steal);

			totalDiff = total - prevTotal;

			
			interThreadComm->averageResults[i] += (totalDiff - (idle - prevIdle))/(totalDiff);
			interThreadComm->averageResults[i] /= 2;			
			interThreadComm->previous[i] = interThreadComm->current[i];
		}
		pthread_mutex_unlock(&interThreadComm->averageResultsLock);
		
	}
}

int analyzerInit(AnalyzerComm *comm)
{
	AnalyzerComm *interThreadComm = (AnalyzerComm*)comm;
	char statBuffer[DATA_LENGTH] = {0};
	char *buffPointer = statBuffer;
	int i = 0;
	pthread_mutex_init(&(interThreadComm->averageResultsLock), NULL);
	
	FILE *statFile = fopen("/proc/stat", "r");
	fread(statBuffer, sizeof(char), DATA_LENGTH, statFile);		
	fclose(statFile);

	interThreadComm->coreCount	= StringOccuranceCount(statBuffer, "cpu");

	interThreadComm->averageResults = malloc((unsigned long)interThreadComm->coreCount * sizeof(double));
	if(interThreadComm->averageResults == NULL)
	{
		return MALLOC_FAILURE;
	}
		
	interThreadComm->current = malloc((unsigned long)interThreadComm->coreCount * sizeof(CpuStat));
	if(interThreadComm->current == NULL)
	{
		free(interThreadComm->averageResults);
		return MALLOC_FAILURE;
	}
		
	interThreadComm->previous = malloc((unsigned long)interThreadComm->coreCount * sizeof(CpuStat));
	if(interThreadComm->previous == NULL)
	{
		free(interThreadComm->averageResults);
		free(interThreadComm->current);
		return MALLOC_FAILURE;
	}	



	for(i=0;i<(int)interThreadComm->coreCount;i++)
	{
		sscanf(buffPointer, "%s %i %i %i %i %i %i %i %i %i %i",
			 interThreadComm->previous[i].core,
			&interThreadComm->previous[i].user,
			&interThreadComm->previous[i].nice,
			&interThreadComm->previous[i].system,
			&interThreadComm->previous[i].idle,
			&interThreadComm->previous[i].iowait,
			&interThreadComm->previous[i].irq,
			&interThreadComm->previous[i].softirq,
			&interThreadComm->previous[i].steal,
			&interThreadComm->previous[i].guest,
			&interThreadComm->previous[i].guest_nice);

		buffPointer = strchr(buffPointer, '\n');
		buffPointer++;
		interThreadComm->averageResults[i] = 0.5;
	}
	
	return 0;
}


void analyzerDestroy(AnalyzerComm *comm)
{
	
	free(comm->averageResults);
	comm->averageResults=NULL;
	
	free(comm->previous);
	comm->previous=NULL;
	
	free(comm->current);
	comm->current=NULL;
			
}

int StringOccuranceCount(char* text, char* searchedStr)
{
	int i, j, count;
	bool found;
	size_t textLen = strlen(text);
	size_t searchedStrLen = strlen(searchedStr);

	count = 0;
	for(i = 0; i < (int)(textLen - searchedStrLen); i++)
	{
		found = true;
		for(j = 0; j < (int)searchedStrLen; j++)
		{
			if(text[i+j] != searchedStr[j])
			{
				found = false;
				break;
			}	
		}
		if(found)
		{
			count++;
		}
	}

	return count;
}
