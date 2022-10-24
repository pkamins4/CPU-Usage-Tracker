#include"analyzer.h"


CpuStat* setCpuStat(CpuStat, char[DATA_LENGTH]);
double sumIdle(CpuStat *c);
double sumTotal(CpuStat *c);
int stringOccuranceCount(char[static 1],char[static 1]);
int analyzerCallback(void*);
int countCores(void);


int analyzerRun(Analyzer *A)
{
	int retVal = pthread_create( &(a->analyzerThread),
				NULL,
				&analyzerCallback,
				A );
	return retVal;
}


void* analyzerCallback(void *analyzerArg)
{
	Analyzer *a = (Analyzer*)analyzerArg;
	char statBuffer[DATA_LENGTH];
	char *buffPointer = NULL;
	int i;
	double idle, prevIdle, total, prevTotal, totalDiff;

	
	while(true)
	{
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

Analyzer* analyzerInit(Queue* fromReaderParam, Queue *toPrinterParam)
{
	char statBuffer[DATA_LENGTH] = {0};
	char *buffPointer = statBuffer;

	Analyzer *A 	= malloc(sizeof(Analyzer));
	A->fromReader 	= fromReaderParam;
	A->toPrinter	= toPrinterParam;
	
	int coreCount	= countCores(); 
	A->current		= malloc( sizeof(CpuStat) * (coreCount + 1) );
	A->previous		= malloc( sizeof(CpuStat) * (coreCount + 1) );

	for( i=0 ; i < coreCount ; i++ )
	{
		setCpuStat(A->previous, buffPointer);

		buffPointer = strchr(buffPointer, '\n');
		buffPointer++;
	}
}


void analyzerDestroy(Analyzer *A)
{
	pthread_cancell(A->analyzerThread);
	pthread_join(A->analyzerThread);

	free(A->previous);
	comm->previous=NULL;
	
	free(A->current);
	comm->current=NULL;

	free(A);
	A=NULL;
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

int countCores(void)
{
	FILE *statFile = fopen("/proc/stat", "r");
	fread(statBuffer, sizeof(char), DATA_LENGTH, statFile);		
	fclose(statFile);

	return ( stringOccuranceCount(statBuffer, "cpu") - 1 );
}