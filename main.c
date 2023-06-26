
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

#define QUEUE_LENGTH 64
#define MAIN_SLEEP 10


int main()
{	
	//printf("Start\n");
	Queue queue_;
	queueInit(&queue_, QUEUE_LENGTH);	
	//printf("Queue initialized\n");
	Reader 		*reader_	= readerInit(&queue_);
	//printf("Reader initialized\n");
	Analyzer 	*analyzer_	= analyzerInit(&queue_);
	//printf("Analyzer initialized\n");
	Printer 	*printer_	= printerInit(analyzer_);
	//printf("Printer initialized\n");
	//printf("Init finished\n");
	
	readerRun(reader_);
	analyzerRun(analyzer_);
	printerRun(printer_);
	//printf("Threads running\n");
	while(1){
		sleep(10);
	}
	printerDestroy(printer_);
	analyzerDestroy(analyzer_);
	readerDestroy(reader_);
	printf("Start\n");
	return 0;
}
