#include"printer.h"
#include <unistd.h>
#include <stdio.h>

#define AVG_START 0.5

void* printerRun(void* printerArg)
{
	Printer *p = (Printer*)printerArg;
	int i;
	sendLog(p->loggerHandle, "Printer thread running.");	

	while(1)
	{
		pthread_kill(p->watchdogHandle, PRINTER_SIG);
		sleep(1);

		pthread_mutex_lock(p->averageResultsLock);
		printf("\nTOTAL:\t%.2f%%\n", ( 100 * p->averageResults[0] ));
		p->averageResults[0] = AVG_START;
		for( i = 1 ; i < *(p->coreCount) ; i++)
		{
			printf("CORE%d\t%.2f%%\n", i,( 100 * p->averageResults[i] ));
			p->averageResults[i] = AVG_START;
		}
		pthread_mutex_unlock(p->averageResultsLock);
	}
	
}
