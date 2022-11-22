#include"reader.h"

void* readerCallback(void *readerArg)
{
	Reader *r = (Reader*)readerArg;
	char statBuffer[DATA_LENGTH] = {0};
	FILE *statFile = NULL;

	while(true)
	{
		const struct timespec sleepTime= {.tv_sec = 0, .tv_nsec = SLEEP_TIME_NSEC};
		nanosleep(&sleepTime, NULL);

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		
		statFile = fopen(PATH, "r");
		fread(statBuffer, sizeof(char), DATA_LENGTH, statFile);				
		fclose(statFile);
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		enqueue(r->toAnalyzer, ANALYZE, statBuffer);		
	}
}

int readerRun(Reader* r)
{
	int retVal = pthread_create( &(r->readerThread),
				NULL,
				&readerCallback,
				r );
	return retVal;
}

Reader* readerInit(Queue *q)
{
	Reader *new =  malloc(sizeof(Reader));
	if(!new)
	{
		return 0;
	}
	new->toAnalyzer = q;
	return new;
}

int readerDestroy(Reader *r)
{
	if(r->toAnalyzer)
	{
		queueDestroy(r->toAnalyzer);
	}

	pthread_join(r->readerThread, NULL);
	
	free(r);
	return 0;
}


