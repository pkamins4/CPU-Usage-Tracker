#include"reader.h"

void* readFunction(void *readerArg)
{
	struct timespec readWait;
	readWait.tv_sec = (long)0;
	readWait.tv_nsec = (long)SLEEP_TIME_NSEC;
	ReaderComm *interThreadComm = (ReaderComm*)readerArg;
	sendLog(interThreadComm->logger, "Reader thread running.");	
	char statBuffer[DATA_LENGTH] = {0};


	while(1)
	{
		nanosleep(&readWait, NULL);
		pthread_kill(interThreadComm->watchdogHandle, READER_SIG);

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		interThreadComm->statFile = fopen(PATH, "r");
		fread(statBuffer, sizeof(char), DATA_LENGTH, interThreadComm->statFile );				
		fclose(interThreadComm->statFile );
		interThreadComm->statFile = NULL;
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		enqueue(interThreadComm->toAnalyzer, statBuffer);		
	}

}

