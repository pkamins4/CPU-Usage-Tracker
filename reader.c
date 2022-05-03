#include"reader.h"
#include"queue.h"
#include<pthread.h>
#include<stdio.h>
#include <time.h>

#define SLEEP_TIME 50000000 //50 ms

void* readFunction(void *readerArg)
{
	struct timespec readWait;
	readWait.tv_sec = (time_t)0;
	readWait.tv_nsec = (long)SLEEP_TIME;
	ReaderComm *interThreadComm = (ReaderComm*)readerArg;


	char statBuffer[DATA_LENGTH] = {0};	
	while(1)
	{
		nanosleep(&readWait, NULL);

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		interThreadComm->statFile = fopen(PATH, "r");
		fread(statBuffer, sizeof(char), DATA_LENGTH, interThreadComm->statFile );		
		fclose(interThreadComm->statFile );
		interThreadComm->statFile = NULL;
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		enqueue(interThreadComm->toAnalyzer, statBuffer);		
	}

}

