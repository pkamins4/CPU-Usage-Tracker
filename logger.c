#include"logger.h"

void* logFunction(void *loggerArg)
{
	LoggerComm *interThreadComm = (LoggerComm*)loggerArg;
	char buffer[DATA_LENGTH]={0};

	while(true)
	{
		dequeue(interThreadComm->threadsInfo, buffer);
		fprintf(interThreadComm->logs, "%s\n", buffer);
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
		fclose(loggerArg->logs);
	}
}

void sendLog(LoggerComm *loggerArg, char *message)
{
	enqueue(loggerArg->threadsInfo, message);
}