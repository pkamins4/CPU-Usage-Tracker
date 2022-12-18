#include"printer.h"
#include <unistd.h>
#include <stdio.h>


Printer* printerInit(Queue* fromAnalyzer)
{
	Printer *new = malloc(sizeof(Printer));
	if(!new){return NULL;}
	new->fromAnalyzer = fromAnalyzer;
	return new;
} 

void* printerCallback(void *printerArg)
{
	Printer *printerObj = (Printer*)printerArg;
	Command cmd;
	char received[32] = {0};
	while(true)
	{
		dequeue(printerObj->fromAnalyzer, &cmd, );
		switch(cmd)
		{
			case PRINT:

				break;			
			default:


		}
		dequeue(printerObj->fromAnalyzer, &cmd, );
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