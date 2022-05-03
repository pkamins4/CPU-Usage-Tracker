/*
	author: Piotr Kamiński
*/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<signal.h>
#include<time.h>
#include"queue.h"
#include"reader.h"
#include"analyzer.h"
#include"printer.h"


#define QUEUE_LENGTH 64
#define MAIN_SLEEP 10


static volatile sig_atomic_t done = 0;

void sigtermHandler(int);

int main()
{
	int retValue = 0;
	pthread_t reader, analyzer, printer;
		
	Queue readerAnalyzerComm;
	ReaderComm readerArg;
	AnalyzerComm analyzerArg;
	PrinterComm printerArg;

	signal(SIGTERM, &sigtermHandler);
	signal(SIGINT, &sigtermHandler);	

	retValue = queueInit(&readerAnalyzerComm, QUEUE_LENGTH);
	if(retValue < 0)
	{
		perror("Process ended in failure.\n");
		return retValue;
	}

	readerArg.toAnalyzer = &readerAnalyzerComm;
	analyzerArg.fromReader = &readerAnalyzerComm;
	
	retValue = analyzerInit(&analyzerArg);
	if(retValue < 0)
	{
		queueDestroy(&readerAnalyzerComm);
		perror("Process ended in failure.\n");
		return retValue;
	}

	
	printerArg.coreCount = &(analyzerArg.coreCount);
	printerArg.averageResults = analyzerArg.averageResults;
	printerArg.averageResultsLock = &(analyzerArg.averageResultsLock);

	pthread_create(&reader, NULL, &readFunction, &readerArg);
	pthread_create(&analyzer, NULL, &analyzeFunction, &analyzerArg);
	pthread_create(&printer, NULL, &printFunction, &printerArg);

	while(!done)
	{
		sleep(MAIN_SLEEP);
	}

	pthread_cancel(reader);
	pthread_join(reader, NULL);
	pthread_cancel(analyzer);
	pthread_join(analyzer, NULL);
	pthread_cancel(printer);	
	pthread_join(printer, NULL);
	
	queueDestroy(&readerAnalyzerComm);
	analyzerDestroy(&analyzerArg);
		
	
	return 0;
}

void sigtermHandler(int sig)
{
	if(sig == SIGTERM || sig == SIGINT)
		done = 1;
}
