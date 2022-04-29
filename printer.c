#include"printer.h"
#include <unistd.h>
#include <stdio.h>


void* printFunction(void* commArg)
{
	ReaderComm *interThreadComm = (ReaderComm*)commArg;
	int i;


	while(1)
	{
		sleep(1);

		pthread_mutex_lock(interThreadComm->averageResultsLock);
		printf("TOTAL:\t%.2f%%\n", (double)(interThreadComm->averageResults[0]));
		for( i = 1 ; i < *(interThreadComm->coreCount) ; i++)
		{
			printf("CORE%d\t%.2f%%\n", i,(double)(interThreadComm->averageResults[0]));
		}
		pthread_mutex_unlock(interThreadComm->averageResultsLock);
	}
	
}
