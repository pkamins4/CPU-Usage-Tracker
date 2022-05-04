#ifndef WATCHDOG_H
#define WATCHDOG_H
#include <time.h>

#define TIMEOUT_SEC 2

typedef struct Watchdog
{
	time_t readerLast, readerCurr;
	time_t analyzerLast, analyzerCurr;
	time_t printerLast, printerCurr;

} Watchdog;

void watchdogInit();
void* watchdogRun(void*);
void watchdogDestroy();



#endif