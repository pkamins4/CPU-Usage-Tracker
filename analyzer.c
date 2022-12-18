#include"analyzer.h"

CpuStat* setCpuStat(CpuStat*, char[DATA_LENGTH]);
double sumIdle(CpuStat c);
double sumTotal(CpuStat c);
int stringOccuranceCount(char[static 1],char[static 1]);
void* analyzerCallback(void*);
int countCores(void);
double calculateUsage(CpuStat, CpuStat);


int analyzerRun(Analyzer *A)
{
	int retVal = pthread_create( &(A->analyzerThread),
				NULL,
				&analyzerCallback,
				A );
	return retVal;
}

void* analyzerCallback(void *analyzerArg)
{
	Analyzer *a = (Analyzer*)analyzerArg;	
		
	while(true)
	{
		Command cmd;
		char statBuffer[DATA_LENGTH]={0};
		dequeue(a->fromReader, &cmd, statBuffer);
		
		char *buffPointer = statBuffer;
		for( int i = 0 ; i < a->coreCount ; i++)
		{	
			a->previous = a->current;
			setCpuStat(a->current, buffPointer);
			buffPointer = strchr(buffPointer, '\n');
			buffPointer++;
		}
		
		for( int i = 0 ; i < a->coreCount ; i++)
		{
			double usage = calculateUsage(a->current[i], a->previous[i]);
			char usageMsg[32];
			sprintf(usageMsg, "%f", usage);
			enqueue(a->toPrinter, PRINT, usageMsg);
		}
	}
}


Analyzer* analyzerInit(Queue* fromReader, Queue *toPrinter)
{
	char statBuffer[DATA_LENGTH] = {0};
	char *buffPointer = statBuffer;

	Analyzer *A 	= malloc(sizeof(Analyzer));
	if(!A) { return NULL;}
	A->fromReader 	= fromReader;
	A->toPrinter	= toPrinter;
	
	A->coreCount	= countCores(); 
	A->current		= malloc( sizeof(CpuStat) * (A->coreCount + 1) );
	if(!A->current) { return NULL;}
	A->previous		= malloc( sizeof(CpuStat) * (A->coreCount + 1) );
	if(!A->previous) { return NULL;}

	for(int i=0 ; i < A->coreCount ; i++ )
	{
		setCpuStat(A->previous, buffPointer);

		buffPointer = strchr(buffPointer, '\n');
		buffPointer++;
	}
	return A;
}


void analyzerDestroy(Analyzer *A)
{
	pthread_cancel(A->analyzerThread);
	pthread_join(A->analyzerThread, NULL);

	free(A->previous);
	A->previous=NULL;
	
	free(A->current);
	A->current=NULL;

	free(A);
	A=NULL;
}

int stringOccuranceCount(char text[static 1],char searchedStr[static 1])
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
				 c->core,
				&c->user,
				&c->nice,
				&c->system,
				&c->idle,
				&c->iowait,
				&c->irq,
				&c->softirq,
				&c->steal,
				&c->guest,
				&c->guest_nice);
	return c;
}

double sumIdle(CpuStat c)
{
	return (double)(c.idle + c.iowait);
}

double sumTotal(CpuStat c)
{
	return (double)( c.user
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
	char statBuffer[DATA_LENGTH] = {0};
	fread(statBuffer, sizeof(char), DATA_LENGTH, statFile);		
	fclose(statFile);

	return ( stringOccuranceCount(statBuffer, "cpu") - 1 );
}

double usagecalculateUsage(CpuStat current, CpuStat previous)
{
	double idle 		= sumIdle(current);
	double prevIdle 	= sumIdle(previous);
	double idleDiff		= idle - prevIdle;
	double total 		= sumTotal(current);
	double prevTotal 	= sumTotal(previous);
	double totalDiff 	= total - prevTotal;

	return ((totalDiff - idleDiff)/totalDiff);
}