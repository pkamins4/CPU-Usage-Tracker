#include"logger.h"
#include <stdbool.h>

void* logFunction(void *loggerArg)
{
	LoggerComm *interThreadComm = (LoggerComm*)loggerArg;
	char buffer[DATA_LENGTH]={0};


	while(true)
	{
		dequeue(interThreadComm->threadsInfo, buffer);
		fprintf(interThreadComm->logs, "%s\n", buffer);
		fflush(interThreadComm->logs);
	}

}

int loggerInit(LoggerComm *loggerArg)
{
	loggerArg->logs = fopen(LOG_FILE, "w");
	if(loggerArg->logs == NULL)
	{
		return LOG_FILE_FAILURE;
	}
	 return 0;
}

void loggerDestroy(LoggerComm *loggerArg)
{
	if(loggerArg->logs != NULL)
	{
		fprintf(loggerArg->logs, "%s\n", "Program finished.");
		fclose(loggerArg->logs);
	}
}

void sendLog(LoggerComm *loggerArg, const char *message)
{
	enqueue(loggerArg->threadsInfo, (char*)message);
}