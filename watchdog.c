#include"watchdog.h"

//static volatile sig_atomic_t int sigReceived = 0;

void watchdogInit(Watchdog *w)
{
	(w->timeout).tv_sec  = (long)TIMEOUT_SEC;
	(w->timeout).tv_nsec = (long)0;

	sigemptyset(&(w->set));
	sigaddset(&(w->set), READER_SIG);
	sigaddset(&(w->set), ANALYZER_SIG);
	sigaddset(&(w->set), PRINTER_SIG);

	w->currTime = time(NULL);
	w->readerLast 	= w->currTime;
	w->analyzerLast = w->currTime;
	w->printerLast 	= w->currTime;
}

void* watchdogRun(void* watchdogArg)
{
	Watchdog *w = (Watchdog*)watchdogArg;
	int sigReceived=0;
	signal(READER_SIG, &handle);
	signal(ANALYZER_SIG, &handle);
	signal(PRINTER_SIG, &handle);
	bool loopContinue = true;
	while(loopContinue)
	{

		sigReceived = sigtimedwait( &(w->set), NULL, &(w->timeout));
		w->currTime = time(NULL);
		switch(sigReceived)
		{
			case READER_SIG:
				w->readerLast 	= w->currTime;
				w->readerDiff 	= 0;
				w->analyzerDiff = difftime(w->currTime, w->analyzerLast);
				w->printerDiff 	= difftime( w->currTime,w->printerLast);
				break;
			case ANALYZER_SIG:
				w->analyzerLast = w->currTime;
				w->analyzerDiff = 0;
				w->readerDiff 	= difftime(w->currTime, w->readerLast);
				w->printerDiff 	= difftime( w->currTime, w->printerLast);
				break;
			case PRINTER_SIG:
				w->printerLast 	= w->currTime;
				w->printerDiff 	= 0;
				w->readerDiff 	= difftime(w->currTime, w->readerLast);
				w->analyzerDiff = difftime(w->currTime, w->analyzerLast);
				break;
			case TIMED_OUT:
				loopContinue = false;
				break;
			default:
				break;
		}
		if( (w->readerDiff 		> TIMEOUT_SEC) ||
		    (w->analyzerDiff 	> TIMEOUT_SEC) ||
		    (w->printerDiff 	> TIMEOUT_SEC))
		{
			loopContinue = false;
		}
		
	}

	fputs("Watchdog timeout. Program closing...\n", stderr);
	pthread_kill(w->mainHandle, SIGTERM);
	while(true)
	{
		sleep(1);
	}
}

void handle(int s)
{
	if(s)
	{
	/*
		Intentionally empty - signals are handled in "watchdogRun" function.
		Without registering this function received signals cause their default handling(f.e. termination).
		Global variable doesn't work (probably access is too slow) - watchdog times out.
	*/
	}
	
}
