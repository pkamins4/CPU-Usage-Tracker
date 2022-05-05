#include"printer.h"
#include <unistd.h>
#include <stdio.h>

#define AVG_START 0.5

void* printFunction(void* commArg)
{
	PrinterComm *interThreadComm = (PrinterComm*)commArg;
	int i;
	sendLog(interThreadComm->logger, "Printer thread running.");	

	while(1)
	{
		pthread_kill(interThreadComm->watchdogHandle, PRINTER_SIG);
		sleep(1);

		pthread_mutex_lock(interThreadComm->averageResultsLock);
		printf("\nTOTAL:\t%.2f%%\n", (100*interThreadComm->averageResults[0]));
		interThreadComm->averageResults[0] = AVG_START;
		for( i = 1 ; i < *(interThreadComm->coreCount) ; i++)
		{
			printf("CORE%d\t%.2f%%\n", i,(100*interThreadComm->averageResults[i]));
			interThreadComm->averageResults[i] = AVG_START;
		}
		pthread_mutex_unlock(interThreadComm->averageResultsLock);
	}
	
}
