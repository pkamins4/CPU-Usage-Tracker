#include"analyzer.h"
#include <unistd.h>
#define DATA_LENGTH 2048

CpuStat* 	setCpuStat(CpuStat*, char[DATA_LENGTH]);
double 		sumIdle(CpuStat c);
double 		sumTotal(CpuStat c);
int 		stringOccuranceCount(char[static 1],char[static 1]);
void* 		analyzerCallback(void*);
int 		countCores(void);
double 		calculateUsage(CpuStat, CpuStat);
void 		getAvgRegisterValue(Analyzer*, double[static 1]);


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
		//sleep(1);
		Command cmd;
		char statBuffer[DATA_LENGTH]={0};
		dequeue(a->fromReader, &cmd, statBuffer);
		//puts("From analyzer");
		//printf("%s\n", statBuffer);
		char *buffPointer = statBuffer;
		for( int i = 0 ; i < a->coreCount+1  ; i++)
		{	
			a->previous[i] = a->current[i];
			setCpuStat(&(a->current[i]), buffPointer);
			buffPointer = strchr(buffPointer, '\n');
			buffPointer++;
		}
		pthread_mutex_lock(&(a->avgRegisterMutex));
		for( int i = 0 ; i < a->coreCount+1 ; i++)
		{
			double usage = calculateUsage(a->current[i], a->previous[i]);
			//printf("\tCore%d: %f\n", i, usage);
			a->avgRegister[i] += usage;
			a->avgRegister[i] /= 2;
			//printf("Average at core%d: %f\n",i, a->avgRegister[i]);
		}
		pthread_mutex_unlock(&(a->avgRegisterMutex));
		
	}
}


Analyzer* analyzerInit(Queue* fromReader)
{
	//char statBuffer[DATA_LENGTH] = {0};
	//char *buffPointer = statBuffer;

	Analyzer *A 	= malloc(sizeof(Analyzer));
	if(!A) { exit(-1);}
	A->fromReader 	= fromReader;	
	pthread_mutex_init(&(A->avgRegisterMutex), NULL);
	A->coreCount	= countCores(); 
	A->current		= malloc( sizeof(CpuStat) * (A->coreCount + 1) );
	if(!A->current) { exit(-1);}
	A->previous		= malloc( sizeof(CpuStat) * (A->coreCount + 1) );
	if(!A->previous) { exit(-1);}
	A->avgRegister 	= malloc( sizeof(double) * (A->coreCount + 1) );
	if(!A->avgRegister) { exit(-1);}
/*
	for(int i=0 ; i < A->coreCount ; i++ )
	{
		setCpuStat(A->previous, buffPointer);

		buffPointer = strchr(buffPointer, '\n');
		buffPointer++;
	}
*/
	return A;
}


void analyzerDestroy(Analyzer *A)
{
	pthread_mutex_destroy(&(A->avgRegisterMutex));
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
	double retval = (c.idle + c.iowait);

	return retval;
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

double calculateUsage(CpuStat current, CpuStat previous)
{
	double idle 		= sumIdle(current);	
	double prevIdle 	= sumIdle(previous);	
	double idleDiff		= idle - prevIdle;	
	double total 		= sumTotal(current);	
	double prevTotal 	= sumTotal(previous);	
	double totalDiff 	= total - prevTotal;	
	double retval = ((totalDiff - idleDiff)/totalDiff);
	/*
	printf("idle\t\t=%f\n", idle);
	printf("prevIdle\t=%f\n", prevIdle);	
	printf("total\t\t=%f\n", total);
	printf("prevTotal\t=%f\n", prevTotal);

	printf("totalDiff\t=%f\n", totalDiff);
	printf("idleDiff\t=%f\n", idleDiff);

	printf("retval\t=%f\n", retval);
	*/


	return retval;
}

void getAvgRegisterValue(Analyzer *A, double buffer[static 1])
{
	pthread_mutex_lock(&(A->avgRegisterMutex));
	for(int i = 0; i < A->coreCount; i++)
	{
		buffer[i] = A->avgRegister[i];
	}
	pthread_mutex_unlock(&(A->avgRegisterMutex));
}