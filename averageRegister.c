#include "averageRegister.h"
#include <stdlib.h>

#define AVERAGE_INIT 0

averageRegister* averageRegisterInit(void);
int averageRegisterDestroy(averageRegister*);
void averegeNextSample(double, averageRegister*);
double getAverage(averageRegister*);

averageRegister* averageRegisterInit(void)
{
    averageRegister* retVal = malloc(sizeof(averageRegister));
    if(!retVal) {exit(-1);}
    
    pthread_mutex_init(&(retVal->regMutex), NULL);
    retVal->average = AVERAGE_INIT;

    return retVal;
}

int averageRegisterDestroy(averageRegister* reg)
{
    if(reg != NULL)
    {
        free(reg);
        return 0;
    }
    else
    {
        return -1;
    }
}

void averageNextSample(double sample, averageRegister* reg)
{
    pthread_mutex_lock(&(reg->regMutex));
    reg->average += sample;
    reg->average /= 2;
    pthread_mutex_unlock(&(reg->regMutex));
}

double getAverage(averageRegister* reg)
{    
    pthread_mutex_lock(&(reg->regMutex));
    double retVal = reg->average;
    pthread_mutex_unlock(&(reg->regMutex));
    return retVal;
}