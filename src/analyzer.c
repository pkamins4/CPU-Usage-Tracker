#include"analyzer.h"


CpuStat* setCpuStat(CpuStat, char[DATA_LENGTH]);
double sumIdle(CpuStat *c);
double sumTotal(CpuStat *c);
int stringOccuranceCount(char[static 1],char[static 1]);

void* analyzerCallback(void *analyzerArg)
{
	Analyzer *a = (Analyzer*)analyzerArg;
	char statBuffer[DATA_LENGTH];
	char *buffPointer = NULL;
	int i;
	double idle, prevIdle, total, prevTotal, totalDiff;

	
	while(true)
	{
		pthread_kill(a->watchdogHandle, ANALYZER_SIG);
		dequeue(a->fromReader, statBuffer);
		
		buffPointer = statBuffer;
		for( i = 0 ; i < a->coreCount ; i++)
		{	
			setCpuStat(a->current, buffPointer);
			buffPointer = strchr(buffPointer, '\n');
			buffPointer++;
		}

		pthread_mutex_lock(&a->averageResultsLock);
		for( i = 0 ; i < a->coreCount ; i++)
		{
			idle 		= sumIdle(a->current[i]);
			prevIdle 	= sumIdle(a->previous[i]);
			total 		= sumTotal(a->current[i]);
			prevTotal 	= sumTotal(a->previous[i]);
			totalDiff 	= total - prevTotal;
			
			a->averageResults[i] += (totalDiff - (idle - prevIdle))/(totalDiff);
			a->averageResults[i] /= 2;			
			a->previous[i] 	= a->current[i];
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

	a->coreCount = stringOccuranceCount(statBuffer, "cpu");

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
		setCpuStat(a, buffPointer);

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

int int stringOccuranceCount(char text[static 1],char searchedStr[static 1])
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


CpuStat* setCpuStat(CpuStat *c, char statBuffer[DATA_LENGTH])
{
	sscanf(statBuffer, "%s %i %i %i %i %i %i %i %i %i %i",
				 c->current.core,
				&c->current.user,
				&c->current.nice,
				&c->current.system,
				&c->current.idle,
				&c->current.iowait,
				&c->current.irq,
				&c->current.softirq,
				&c->current.steal,
				&c->current.guest,
				&c->current.guest_nice);
	return c;
}

double sumIdle(CpuStat c)
{
	return (double)(c.idle + c.iowait);
}

double sumTotal(CpuStat c)
{
	return (double)(c.user
					+c.nice
					+c.system
					+c.idle
					+c.iowait
					+c.irq
					+c.softirq
					+c.steal);
}