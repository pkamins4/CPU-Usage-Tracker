#include"logger.h"
#include <stdbool.h>

void* loggerRun(void *loggerArg)
{
	Logger *l = (Logger*)loggerArg;
	char buffer[DATA_LENGTH]={0};

	while(true)
	{
		dequeue(l->threadsInfo, buffer);
		fprintf(l->logs, "%s\n", buffer);
		fflush(l->logs);
	}
}

int loggerInit(Logger *loggerArg)
{
	loggerArg->logs = fopen(LOG_FILE, "w");
	if(loggerArg->logs == NULL)
	{
		return LOG_FILE_FAILURE;
	}
	 return 0;
}

void loggerDestroy(Logger *loggerArg)
{
	if(loggerArg->logs != NULL)
	{
		fprintf(loggerArg->logs, "%s\n", "Program finished.");
		fclose(loggerArg->logs);
	}
}

void sendLog(Logger *loggerArg, char *message)
{
	enqueue(loggerArg->threadsInfo, message);
}
