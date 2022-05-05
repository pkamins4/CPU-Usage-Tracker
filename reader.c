#include"reader.h"



void* readerRun(void *readerArg)
{
	Reader *r = (Reader*)readerArg;
	sendLog(r->loggerHandle, "Reader thread running.");	
	char statBuffer[DATA_LENGTH] = {0};

	while(true)
	{
		nanosleep(&r->waitTime, NULL);
		pthread_kill(r->watchdogHandle, READER_SIG);

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		
		r->statFile = fopen(PATH, "r");
		fread(statBuffer, sizeof(char), DATA_LENGTH, r->statFile);				
		fclose(r->statFile);
		
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		enqueue(r->toAnalyzer, statBuffer);		
	}

}

void readerInit(Reader *r)
{
	(r->waitTime).tv_sec	=(long)0;
	(r->waitTime).tv_nsec	=(long)SLEEP_TIME_NSEC;
}


