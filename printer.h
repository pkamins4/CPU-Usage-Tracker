#ifndef PRINTER_H
#define PRINTER_H
#include<pthread.h>
#include"analyzer.h"

typedef struct Printer
{
	pthread_mutex_t *avgRegisterMutex;
	size_t avgRegisterSize;
	double *avgRegisterPtr;
	pthread_t printerThread;
} Printer;

Printer* printerInit(Analyzer*);
int printerRun(Printer*);
void printerDestroy(Printer*);

#endif
