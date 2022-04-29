#include"reader.h"
#include"queue.h"
#include<pthread.h>
#include<stdio.h>


void* readFunction(void *queueArg)
{
	FILE *statFile;

	char statBuffer[DATA_LENGTH];
	Queue *toAnalyzer= (Queue*)queueArg;
	
	while(1)
	{
		statFile = fopen(PATH, "r");
		fread(statBuffer, sizeof(char), DATA_LENGTH, statFile);		
		fclose(statFile);

		enqueue(toAnalyzer, statBuffer);
	}

}
