#include"reader.h"



void* readerCallback(void *readerArg)
{
	Reader *r = (Reader*)readerArg;
	char statBuffer[DATA_LENGTH] = {0};

	while(true)
	{
		nanosleep(SLEEP_TIME_NSEC, NULL);

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		
		r->statFile = fopen(PATH, "r");
		fread(statBuffer, sizeof(char), DATA_LENGTH, r->statFile);				
		fclose(r->statFile);
		
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		enqueue(r->toAnalyzer, statBuffer);		
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
		queueDestroy(reader->toAnalyzer);
	}
	
	free(r);
	return 0;
}


