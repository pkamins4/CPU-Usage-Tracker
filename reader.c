#include"reader.h"
#include"queue.h"
#include<pthread.h>
#include<stdio.h>
#include <time.h>


void* readFunction(void *readerArg)
{
	struct timespec readWait;
	readWait.tv_sec = (time_t)0;
	readWait.tv_nsec = (long)SLEEP_TIME;
	ReaderComm *interThreadComm = (ReaderComm*)readerArg;
	sendLog(interThreadComm->logger, "Reader thread running.");	

	char statBuffer[DATA_LENGTH] = {0};

	
	while(1)
	{
		nanosleep(&readWait, NULL);

		interThreadComm->statFile = fopen(PATH, "r");
		fread(statBuffer, sizeof(char), DATA_LENGTH, interThreadComm->statFile );
				
		fclose(interThreadComm->statFile );
		interThreadComm->statFile = NULL;
		
		enqueue(interThreadComm->toAnalyzer, statBuffer);		
	}

}

void readerDestroy(ReaderComm* readerArg)
{
	if(readerArg->statFile != NULL)
	{
		fclose(readerArg->statFile );
	}
}
