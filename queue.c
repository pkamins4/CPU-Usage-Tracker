#include"queue.h"

int queueInit(Queue *q, unsigned int queueLength)
{
	q->head = NULL;
	q->tail = NULL;

	if(sem_init(&(q->emptySlots), PSHARED, queueLength) != 0)
	{
		return QUEUE_ERROR;
	}
	if(sem_init(&(q->fullSlots), PSHARED, 0) != 0)
	{
		return QUEUE_ERROR;
	}	
	if(pthread_mutex_init(&(q->bufferLock), NULL) != 0)
	{
		return QUEUE_ERROR;
	}

	return 0;
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

	if(sem_destroy(&(q->emptySlots)) != 0)
	{
		return QUEUE_ERROR;
	}
	if(sem_destroy(&(q->fullSlots)) != 0)
	{
		return QUEUE_ERROR;
	}
	if(pthread_mutex_destroy(&(q->bufferLock)) != 0)
	{
		return QUEUE_ERROR;
	}
	
	return 0;
}

int enqueue(Queue *restrict q, Command command,char *restrict newData)
{
	sem_wait(&(q->emptySlots));
	pthread_mutex_lock(&(q->bufferLock));

	Node *newNode = malloc(sizeof(Node));
	if(newNode == NULL)
	{
		return QUEUE_ERROR;
	}

	memcpy(&(newNode->cmd), &command, sizeof(Command));
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
	
	pthread_mutex_unlock(&(q->bufferLock));
	sem_post(&(q->fullSlots));

	return 0;
}

int dequeue(Queue *q, Node *received, char *buffer)
{

	sem_wait(&(q->fullSlots));
	pthread_mutex_lock(&(q->bufferLock));


	Node *temp = q->head;

	memcpy(received, temp, sizeof(Node));

	q->head = q->head->next;
	if(q->head == NULL)
	{
		q->tail = NULL;
	}
	free(temp);

	pthread_mutex_unlock(&(q->bufferLock));
	sem_post(&(q->emptySlots));

	return 0;
}

bool isEmpty(Queue* q)
{
	int semVal = 0;
	sem_getvalue(&(q->fullSlots), &semVal);
	if(semVal == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}