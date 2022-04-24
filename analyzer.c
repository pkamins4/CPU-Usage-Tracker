#include"analyzer.h"
#include"queue.h"
#include <stdio.h> 
#include <stdbool.h>

int StringOccuranceCount(char* text, char* searchedStr)
{
	int i, j, count;
	bool found;
	size_t textLen = strlen(text);
	size_t searchedStrLen = strlen(searchedStr);

	count = 0;
	for(i = 0; i < textLen - searchedStrLen; i++)
	{
		found = true;
		for(j = 0; j < searchedStrLen; j++)
		{
			if(text[i+j] != searchedStr[j])
			{
				found = false;
				break;
			}	
		}
		if(found)
		{
			count++;
		}
	}

	return count;

}

void analyzerInit(AnalyzerComm *comm)
{
	comm->fromReader = NULL;
	comm->averageResults = NULL;
	comm->current = NULL;
	comm->previous = NULL;
}

void analyzeFunction(void *commArg)
{
	AnalyzerComm *interThreadComm = (AnalyzerComm*)commArg
	char statBuffer[DATA_LENGTH];

	dequeue(interThreadComm->fromReader, statBuffer);
	interThreadComm->coreCount = StringOccuranceCount(statBuffer, "car")-1;
	averageResults = malloc(sizeof(float)*coreCount);


}
