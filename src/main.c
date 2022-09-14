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
	pthread_t loggerThread, readerThread, analyzerThread, printerThread, watchdogThread;
	Queue toLogger;
	retValue = queueInit(&toLogger, QUEUE_LENGTH);
	if(retValue < 0)
	{
		fputs("Process ended in failure.\n", stderr);
		return retValue;
	}

	Logger logger;
	logger.threadsInfo = &toLogger;
	loggerInit(&logger);	
	pthread_create(&loggerThread, NULL, &loggerRun, &logger);
	sendLog(&logger, "Programm started working.");

	Watchdog watchdog;
	watchdog.mainHandle = pthread_self();
	watchdogInit(&watchdog);
	pthread_create(&watchdogThread, NULL, &watchdogRun, &watchdog); 

	Queue readerAnalyzerComm;
	retValue = queueInit(&readerAnalyzerComm, QUEUE_LENGTH);
	if(retValue < 0)
	{
		queueDestroy(&toLogger);
		fputs("Process ended in failure.\n", stderr);
		return retValue;
	}

	Reader reader;
	reader.toAnalyzer = &readerAnalyzerComm;
	reader.loggerHandle = &logger;
	reader.watchdogHandle = watchdogThread;
	readerInit(&reader);

	Analyzer analyzer;
	analyzer.fromReader = &readerAnalyzerComm;
	analyzer.loggerHandle = &logger;
	analyzer.watchdogHandle = watchdogThread;	
	retValue = analyzerInit(&analyzer);
	if(retValue < 0)
	{
		queueDestroy(&toLogger);
		queueDestroy(&readerAnalyzerComm);
		fputs("Process ended in failure.\n", stderr);
		return retValue;
	}

	Printer printer;
	printer.coreCount = &(analyzer.coreCount);
	printer.averageResults = analyzer.averageResults;
	printer.averageResultsLock = &(analyzer.averageResultsLock);
	printer.loggerHandle = &logger;
	printer.watchdogHandle = watchdogThread;
	
	signal(SIGTERM, &sigtermHandler);
	sendLog(&logger, "Signal SIGTERM handler registered.");

	
	signal(SIGINT, &sigtermHandler);
	sendLog(&logger, "Signal SIGINT handler registered.");	

	pthread_create(&readerThread, NULL, &readerRun, &reader);
	pthread_create(&analyzerThread, NULL, &analyzerRun, &analyzer);
	pthread_create(&printerThread, NULL, &printerRun, &printer);
	
	sendLog(&logger, "Main waiting for ending signal...");	

	while(!signalReceived)
	{
		sleep(MAIN_SLEEP);
	}
	
	sendLog(&logger, "Signal received by main. Ending...");
	pthread_cancel(readerThread);
	pthread_join(readerThread, NULL);
	pthread_cancel(analyzerThread);
	pthread_join(analyzerThread, NULL);
	pthread_cancel(printerThread);	
	pthread_join(printerThread, NULL);
	pthread_cancel(loggerThread);	
	pthread_join(loggerThread, NULL);
	pthread_cancel(watchdogThread);	
	pthread_join(watchdogThread, NULL);
	
	queueDestroy(&readerAnalyzerComm);
	queueDestroy(&toLogger);
	analyzerDestroy(&analyzer);		
	loggerDestroy(&logger);

	return retValue;
}

void sigtermHandler(int sig)
{
	if(sig == SIGTERM || sig == SIGINT)
	{
		signalReceived = sig;
	}
}
