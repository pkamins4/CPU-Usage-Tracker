#ifndef AVERAGE_REGISTER_H
#define AVERAGE_REGISTER_H

#include<pthread.h>


typedef struct averageRegister
{
    pthread_mutex_t regMutex;
    double *average;
    size_t regSize;
}averageRegister;

averageRegister* averageRegisterInit(size_t);
int averageRegisterDestroy(averageRegister*);
void averageNextSample(double, averageRegister*, int);
double getAverage(averageRegister*, int);

#endif