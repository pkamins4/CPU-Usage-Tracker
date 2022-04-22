#ifndef QUEUE_H
#define QUEUE_H

#define DATA_LENGTH 1024
#define QUEUE_CRITICAL_FAILURE -1
#define QUEUE_EMPTY -2

typedef struct Node
{
	char data[DATA_LENGTH];
	struct Node *next;
} Node;


typedef struct Queue
{
	Node *head;
	Node *tail;

} Queue;


void queueInit(Queue*);
void queueDestroy(Queue*);

int enqueue(Queue*, char*);

int dequeue(Queue*, char*);


#endif 