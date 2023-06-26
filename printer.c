#include"printer.h"
#include <unistd.h>
#include <stdio.h>


Printer* printerInit(Analyzer *A)
{
	if(!A)
	{
		exit(-1);
	}
	Printer *new = malloc(sizeof(Printer));
	if(!new){exit(-1);}
	new->avgRegisterSize = (size_t)A->coreCount;
	new->avgRegisterPtr = A->avgRegister;
	new->avgRegisterMutex = &(A->avgRegisterMutex);

	return new;
} 

void* printerCallback(void *printerArg)
{
	Printer *printerObj = (Printer*)printerArg;
	
	while(true)
	{		
		pthread_mutex_lock(printerObj->avgRegisterMutex);
		for(int i = 0; i < printerObj->avgRegisterSize; i++)
		{
			printf("Core %d usage = %2.2f%%", i,  100*printerObj->avgRegisterPtr[i]);
			printf("\n");
			printerObj->avgRegisterPtr[i] = 0;
		}
		pthread_mutex_unlock(printerObj->avgRegisterMutex);
		printf("--------###--------\n");
		sleep(1);
	}
}

int printerRun(Printer *P)
{
	int retVal = pthread_create( &(P->printerThread),
				NULL,
				&printerCallback,
				P );
	return retVal;
}

void printerDestroy(Printer *P)
{
	pthread_cancel(P->printerThread);
	pthread_join(P->printerThread, NULL);
	free(P);
}