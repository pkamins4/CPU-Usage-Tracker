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

void sigtermHandler(int);

int main()
{
	
	signal(SIGTERM, &sigtermHandler); /* because c99...*/

	printf("TOTAL\tB%%\tC\n");
	//for(;;){}
	return 0;
}

void sigtermHandler(int sig)
{
	printf("xd%d\n", sig);
}
