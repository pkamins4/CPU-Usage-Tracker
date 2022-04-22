#include"queue.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

void queueInit(Queue *q)
{
	q->head = NULL;
	q->tail = NULL;
}

int queueDestroy(Queue *q)
{	
	Node *temp;	
		
	while((q->head) != NULL)
	{
		temp = q->head;
		q->head = q->head->next;
		free(temp);
	}
	return 0;
}

int enqueue(Queue *q, char *newData)
{
	Node *newNode = malloc(sizeof(Node));
	if(newNode == NULL)
	{
		return QUEUE_CRITICAL_FAILURE;
	}

	memcpy(newNode->data, newData, sizeof(char)*DATA_LENGTH);
	newNode->next = NULL;

	if(q->tail != NULL)
	{
		q->tail->next = newNode;
	}
	q->tail = newNode;

	if(q->head == NULL)
	{
		q->head = newNode;
	}

	return 0;
}

int dequeue(Queue *q, char* buffer)
{
	if(q->head == NULL)
	{
		return QUEUE_EMPTY; 
	}
	Node *temp = q->head;

	memcpy(buffer, temp->data, sizeof(char)*DATA_LENGTH);

	q->head = q->head->next;
	if(q->head == NULL)
	{
		q->tail = NULL;
	}
	free(temp);

	return 0;
}