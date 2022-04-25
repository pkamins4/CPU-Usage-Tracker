#include"analyzer.h"
#include"queue.h"
#include <stdio.h> 
#include <stdbool.h>


void analyzeFunction(void *commArg)
{
	AnalyzerComm *interThreadComm = (AnalyzerComm*)commArg
	char statBuffer[DATA_LENGTH];
	char *buffPointer = statBuffer;
	int i;
	float idle, prevIdle, total, prevTotal, totalDiff;

	interThreadComm->coreCount 		= StringOccuranceCount(statBuffer, "cpu");
	interThreadComm->averageResults = malloc(sizeof(float)*coreCount);
	interThreadComm->current 		= malloc(sizeof(CpuStat)*interThreadComm->coreCount);
	interThreadComm->previous 		= malloc(sizeof(CpuStat)*interThreadComm->coreCount);

	pthread_mutex_lock(interThreadComm->averageResultsLock);
	for( i = 0 ; i < interThreadComm->coreCount ; i++)
	{
		*interThreadComm->averageResults = 0;
	}
	pthread_mutex_unlock(interThreadComm->averageResultsLock);

	dequeue(interThreadComm->fromReader, statBuffer);	

	for( i = 0 ; i < interThreadComm->coreCount ; i++)
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

		buffPointer = strchar(buffPointer, '\n');
		buffPointer++;
	}
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

			buffPointer = strchar(buffPointer, '\n');
			buffPointer++;
		}

		pthread_mutex_lock(interThreadComm->averageResultsLock);
		for( i = 0 ; i < interThreadComm->coreCount ; i++)
		{
			idle = interThreadComm->current[i].idle + interThreadComm->current[i].iowait;
			prevIdle = interThreadComm->previous[i].idle + interThreadComm->previous[i].iowait;

			total =  interThreadComm->current[i].user
					+interThreadComm->current[i].nice
					+interThreadComm->current[i].system
					+interThreadComm->current[i].idle
					+interThreadComm->current[i].iowait
					+interThreadComm->current[i].irq
					+interThreadComm->current[i].softirq
					+interThreadComm->current[i].steal;

			prevTotal =  interThreadComm->previous[i].user
					+interThreadComm->previous[i].nice
					+interThreadComm->previous[i].system
					+interThreadComm->previous[i].idle
					+interThreadComm->previous[i].iowait
					+interThreadComm->previous[i].irq
					+interThreadComm->previous[i].softirq
					+interThreadComm->previous[i].steal;

			totalDiff = total - prevTotal;
			averageResults[i] += (totalDiff - (idle - prevIdle))/(totalDiff * 2);
		}
		pthread_mutex_unlock(interThreadComm->averageResultsLock);
	}

	
}

void analyzerInit(AnalyzerComm *comm)
{
	//comm->fromReader 		= NULL; /* nie potrzebe?? */
	comm->averageResults 	= NULL;
	comm->current 			= NULL;
	comm->previous 			= NULL;
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
	for(i = 0; i < textLen - searchedStrLen; i++)
	{
		found = true;
		for(j = 0; j < searchedStrLen; j++)
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