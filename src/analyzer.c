#include"analyzer.h"

CpuStat* setCpuStat(CpuStat *c, char statBuffer[DATA_LENgTH])
{
	sscanf(buffPointer, "%s %i %i %i %i %i %i %i %i %i %i",
				 c->current[i].core,
				&c->current[i].user,
				&c->current[i].nice,
				&c->current[i].system,
				&c->current[i].idle,
				&c->current[i].iowait,
				&c->current[i].irq,
				&c->current[i].softirq,
				&c->current[i].steal,
				&c->current[i].guest,
				&c->current[i].guest_nice);
	return c;
}

void* analyzerCallback(void *analyzerArg)
{
	Analyzer *a = (Analyzer*)analyzerArg;
	char statBuffer[DATA_LENGTH];
	char *buffPointer = NULL;
	int i;
	double idle, prevIdle, total, prevTotal, totalDiff;

	
	while(1)
	{
		pthread_kill(a->watchdogHandle, ANALYZER_SIG);
		dequeue(a->fromReader, statBuffer);
		
		buffPointer = statBuffer;
		for( i = 0 ; i < a->coreCount ; i++)
		{
			sscanf(buffPointer, "%s %i %i %i %i %i %i %i %i %i %i",
				 a->current[i].core,
				&a->current[i].user,
				&a->current[i].nice,
				&a->current[i].system,
				&a->current[i].idle,
				&a->current[i].iowait,
				&a->current[i].irq,
				&a->current[i].softirq,
				&a->current[i].steal,
				&a->current[i].guest,
				&a->current[i].guest_nice);

			buffPointer = strchr(buffPointer, '\n');
			buffPointer++;
		}

		pthread_mutex_lock(&a->averageResultsLock);
		for( i = 0 ; i < a->coreCount ; i++)
		{
			idle = (double)(a->current[i].idle + a->current[i].iowait);
			prevIdle = (double)(a->previous[i].idle + a->previous[i].iowait);

			total =  (double)(a->current[i].user
					+a->current[i].nice
					+a->current[i].system
					+a->current[i].idle
					+a->current[i].iowait
					+a->current[i].irq
					+a->current[i].softirq
					+a->current[i].steal);

			prevTotal =  (double)(a->previous[i].user
					+a->previous[i].nice
					+a->previous[i].system
					+a->previous[i].idle
					+a->previous[i].iowait
					+a->previous[i].irq
					+a->previous[i].softirq
					+a->previous[i].steal);

			totalDiff = total - prevTotal;

			
			a->averageResults[i] += (totalDiff - (idle - prevIdle))/(totalDiff);
			a->averageResults[i] /= 2;			
			a->previous[i] = a->current[i];
		}
		pthread_mutex_unlock(&a->averageResultsLock);
	}
}

int analyzerInit(Analyzer *comm)
{
	Analyzer *a = (Analyzer*)comm;
	char statBuffer[DATA_LENGTH] = {0};
	char *buffPointer = statBuffer;
	int i = 0;
	pthread_mutex_init(&(a->averageResultsLock), NULL);
	
	FILE *statFile = fopen("/proc/stat", "r");
	fread(statBuffer, sizeof(char), DATA_LENGTH, statFile);		
	fclose(statFile);

	a->coreCount = StringOccuranceCount(statBuffer, "cpu");

	a->averageResults = malloc((unsigned long)a->coreCount * sizeof(double));
	if(a->averageResults == NULL)
	{
		return MALLOC_FAILURE;
	}
		
	a->current = malloc((unsigned long)a->coreCount * sizeof(CpuStat));
	if(a->current == NULL)
	{
		free(a->averageResults);
		return MALLOC_FAILURE;
	}
		
	a->previous = malloc((unsigned long)a->coreCount * sizeof(CpuStat));
	if(a->previous == NULL)
	{
		free(a->averageResults);
		free(a->current);
		return MALLOC_FAILURE;
	}	



	for(i=0;i<(int)a->coreCount;i++)
	{
		sscanf(buffPointer, "%s %i %i %i %i %i %i %i %i %i %i",
			 a->previous[i].core,
			&a->previous[i].user,
			&a->previous[i].nice,
			&a->previous[i].system,
			&a->previous[i].idle,
			&a->previous[i].iowait,
			&a->previous[i].irq,
			&a->previous[i].softirq,
			&a->previous[i].steal,
			&a->previous[i].guest,
			&a->previous[i].guest_nice);

		buffPointer = strchr(buffPointer, '\n');
		buffPointer++;
		a->averageResults[i] = 0.5;
	}
	
	return 0;
}


void analyzerDestroy(Analyzer *comm)
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
