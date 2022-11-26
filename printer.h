#ifndef PRINTER_H
#define PRINTER_H
#include<pthread.h>
#include"queue.h"

#define AVG_START 0.5

typedef struct Printer
{
	Queue *fromAnalyzer;
	pthread_t printerThread;	

} Printer;

Printer* printerInit(Queue*);
int printerRun(Printer*);
void printerDestroy(Printer*);

#endif
