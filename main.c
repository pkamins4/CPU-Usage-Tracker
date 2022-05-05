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
#include"logger.h"
#include"watchdog.h"


#define QUEUE_LENGTH 64
#define MAIN_SLEEP 10


static volatile sig_atomic_t signalReceived = 0;

void sigtermHandler(int);

int main()
{
	int retValue = 0;
	pthread_t logger;
	Queue toLogger;
	LoggerComm loggerArg;


	retValue = queueInit(&toLogger, QUEUE_LENGTH);
	if(retValue < 0)
	{
		perror("Process ended in failure.\n");
		return retValue;
	}

	loggerArg.threadsInfo = &toLogger;
	loggerInit(&loggerArg);
	pthread_create(&logger, NULL, &logFunction, &loggerArg);
	sendLog(&loggerArg, "Programm started working.");
	

	pthread_t reader, analyzer, printer, watchdogThread;

	Queue readerAnalyzerComm;	
	ReaderComm readerArg;
	AnalyzerComm analyzerArg;
	PrinterComm printerArg;
	Watchdog watchdog;
	watchdog.mainHandle = pthread_self();
	watchdogInit(&watchdog);
	
	
	
	signal(SIGTERM, &sigtermHandler);
	sendLog(&loggerArg, "Signal SIGTERM handler registered.");

	
	signal(SIGINT, &sigtermHandler);
	sendLog(&loggerArg, "Signal SIGINT handler registered.");	

	retValue = queueInit(&readerAnalyzerComm, QUEUE_LENGTH);
	if(retValue < 0)
	{
		queueDestroy(&toLogger);
		perror("Process ended in failure.\n");
		return retValue;
	}

	
	pthread_create(&watchdogThread, NULL, &watchdogRun, &watchdog);
	readerArg.toAnalyzer = &readerAnalyzerComm;
	readerArg.logger = &loggerArg;
	readerArg.watchdogHandle = watchdogThread;
	analyzerArg.fromReader = &readerAnalyzerComm;
	analyzerArg.logger = &loggerArg;
	analyzerArg.watchdogHandle = watchdogThread;
	
	retValue = analyzerInit(&analyzerArg);
	if(retValue < 0)
	{
		queueDestroy(&toLogger);
		queueDestroy(&readerAnalyzerComm);
		perror("Process ended in failure.\n");
		return retValue;
	}

	

	printerArg.coreCount = &(analyzerArg.coreCount);
	printerArg.averageResults = analyzerArg.averageResults;
	printerArg.averageResultsLock = &(analyzerArg.averageResultsLock);
	printerArg.logger = &loggerArg;
	printerArg.watchdogHandle = watchdogThread;

	pthread_create(&reader, NULL, &readFunction, &readerArg);
	pthread_create(&analyzer, NULL, &analyzeFunction, &analyzerArg);
	pthread_create(&printer, NULL, &printFunction, &printerArg);
	
	
	while(!signalReceived)
	{
		sleep(MAIN_SLEEP);
	}
	
	sendLog(&loggerArg, "Signal received. Ending...");
	pthread_cancel(reader);
	pthread_join(reader, NULL);
	pthread_cancel(analyzer);
	pthread_join(analyzer, NULL);
	pthread_cancel(printer);	
	pthread_join(printer, NULL);
	pthread_cancel(logger);	
	pthread_join(logger, NULL);
	pthread_cancel(watchdogThread);	
	pthread_join(watchdogThread, NULL);
	
	queueDestroy(&readerAnalyzerComm);

	queueDestroy(&toLogger);
	analyzerDestroy(&analyzerArg);		
	loggerDestroy(&loggerArg);

	return retValue;
}

void sigtermHandler(int sig)
{
	if(sig == SIGTERM || sig == SIGINT)
		signalReceived = sig;
}
