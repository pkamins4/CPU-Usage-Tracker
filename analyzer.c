#include"analyzer.h"
#include"queue.h"
#include <stdio.h> 
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


void analyzeFunction(void *commArg)
{
	AnalyzerComm *interThreadComm = (AnalyzerComm*)commArg;
	char statBuffer[DATA_LENGTH];
	char *buffPointer = statBuffer;
	int i;
	float idle, prevIdle, total, prevTotal, totalDiff;


	

	while(1)
	{
		dequeue(interThreadComm->fromReader, statBuffer);
		
		buffPointer = statBuffer;
		for( i = 0 ; i < *(interThreadComm->coreCount) ; i++)
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

		pthread_mutex_lock(interThreadComm->averageResultsLock);
		for( i = 0 ; i < *(interThreadComm->coreCount) ; i++)
		{
			idle = (float)(interThreadComm->current[i].idle + interThreadComm->current[i].iowait);
			prevIdle = (float)(interThreadComm->previous[i].idle + interThreadComm->previous[i].iowait);

			total =  (float)(interThreadComm->current[i].user
					+interThreadComm->current[i].nice
					+interThreadComm->current[i].system
					+interThreadComm->current[i].idle
					+interThreadComm->current[i].iowait
					+interThreadComm->current[i].irq
					+interThreadComm->current[i].softirq
					+interThreadComm->current[i].steal);

			prevTotal =  (float)(interThreadComm->previous[i].user
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
		pthread_mutex_unlock(interThreadComm->averageResultsLock);
		
	}
}

void analyzerInit(AnalyzerComm *comm)
{
	AnalyzerComm *interThreadComm = (AnalyzerComm*)comm;
	char statBuffer[DATA_LENGTH];
	char *buffPointer = statBuffer;
	int i;

	dequeue(interThreadComm->fromReader, statBuffer);

	*(interThreadComm->coreCount)	= StringOccuranceCount(statBuffer, "cpu");
	interThreadComm->averageResults = malloc(sizeof(float)*(unsigned long)(*interThreadComm->coreCount));
	interThreadComm->current 		= malloc(sizeof(CpuStat)*(unsigned long)(*(interThreadComm->coreCount)));
	interThreadComm->previous 		= malloc(sizeof(CpuStat)*(unsigned long)(*(interThreadComm->coreCount)));

	for( i = 0 ; i < *(interThreadComm->coreCount) ; i++)
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
	}

	pthread_mutex_lock(interThreadComm->averageResultsLock);
	for( i = 0 ; i < *(interThreadComm->coreCount) ; i++)
	{
		*interThreadComm->averageResults = 0;
	}
	pthread_mutex_unlock(interThreadComm->averageResultsLock);
}

void analyzerDestroy(AnalyzerComm *comm)
{
	//free(comm->fromReader);
	free(comm->averageResults);
	free(comm->current);
	free(comm->previous);
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
