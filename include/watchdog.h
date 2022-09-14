#ifndef WATCHDOG_H
#define WATCHDOG_H

#include<time.h>
#include<signal.h>
#include<pthread.h>
#include<stdbool.h>
#include<stdio.h>
#include<unistd.h>

#define TIMEOUT_SEC 2

#define READER_SIG 34
#define ANALYZER_SIG 35
#define PRINTER_SIG 36
#define TIMED_OUT -1

typedef struct Watchdog
{
	time_t readerLast;
	time_t analyzerLast;
	time_t printerLast;
	time_t currTime;
	double readerDiff, analyzerDiff, printerDiff;
	pthread_t mainHandle;
	struct timespec timeout;
	sigset_t set;
} Watchdog;

void watchdogInit(Watchdog*);
void* watchdogRun(void*);
void handle(int);



#endif
