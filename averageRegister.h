#ifndef AVERAGE_REGISTER_H
#define AVERAGE_REGISTER_H

#include<pthread.h>


typedef struct averageRegister
{
    pthread_mutex_t regMutex;
    double average;
}averageRegister;

averageRegister* averageRegisterInit(void);
int averageRegisterDestroy(averageRegister*);
void averageNextSample(double, averageRegister*);
double getAverage(averageRegister*);

#endif