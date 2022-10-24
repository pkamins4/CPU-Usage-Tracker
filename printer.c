#include"printer.h"
#include <unistd.h>
#include <stdio.h>

#define AVG_START 0.5

Printer* printerInit(Queue* fromAnalyzer)
{

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
	
}