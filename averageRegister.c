#include "averageRegister.h"
#include <stdlib.h>

#define AVERAGE_INIT 0

averageRegister* averageRegisterInit(size_t regSize);
int averageRegisterDestroy(averageRegister*);
void averegeNextSample(double, averageRegister*, int);
double getAverage(averageRegister*, int);

averageRegister* averageRegisterInit(size_t regSize)
{
    averageRegister* retVal = malloc(sizeof(averageRegister));
    if(!retVal) {exit(-1);}

    retVal->average = malloc(sizeof(double) * regSize);
    if(!(retVal->average)) {exit(-1);}
    
    retVal->regSize = regSize;
    for(int i = 0; i < retVal->regSize; i++)
    {
        retVal->average[i] = AVERAGE_INIT;
    }

    pthread_mutex_init(&(retVal->regMutex), NULL);
    
    return retVal;
}

int averageRegisterDestroy(averageRegister* reg)
{
    if(reg != NULL)
    {
        pthread_mutex_destroy(&(reg->regMutex));
        free(reg->average);
        free(reg);
        reg = NULL;
        return 0;
    }
    else
    {
        return -1;
    }
}

void averageNextSample(double sample, averageRegister* reg, int index)
{
    pthread_mutex_lock(&(reg->regMutex));
    reg->average[index] += sample;
    reg->average[index] /= 2;
    pthread_mutex_unlock(&(reg->regMutex));
}

double getAverage(averageRegister* reg, int index)
{    
    if(index < 0 || index > reg->regSize)
    {
        exit(-1);
    }
    pthread_mutex_lock(&(reg->regMutex));
    double retVal = reg->average[index];
    pthread_mutex_unlock(&(reg->regMutex));
    return retVal;
}