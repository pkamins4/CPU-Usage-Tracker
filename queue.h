#ifndef QUEUE_H
#define QUEUE_H
#include<semaphore.h>
#include<pthread.h>

#define DATA_LENGTH 2048
#define PSHARED 0

#define QUEUE_ERROR -1

typedef struct Node
{
	char data[DATA_LENGTH];
	struct Node *next;
	
} Node;


typedef struct Queue
{
	Node *head;
	Node *tail;

	sem_t emptySlots;
	sem_t fullSlots;

	pthread_mutex_t bufferLock;

} Queue;


int queueInit(Queue*, unsigned int);
int queueDestroy(Queue*);
int enqueue(Queue*, char*);
int dequeue(Queue*, char*);

#endif
