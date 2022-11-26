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

void* printerCallback(void *analyzerArg)
{
	
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