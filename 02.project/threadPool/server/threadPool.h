#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <myfunc.h>
#include "taskQueue.h"

typedef struct threadPool_s{
    pthread_t *tidArr;
    taskQueue_t taskQueue;
    int threadNum;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int threadIsRunning;
} threadPool_t;

int threadPoolInit(threadPool_t *pThreadPool, int num);
#endif