#include "threadPool.h"

int threadPoolInit(threadPool_t *pThreadPool, int num)
{
    bzero(pThreadPool, sizeof(threadPool_t));
    pThreadPool->tidArr = (pthread_t *)calloc(num, sizeof(pthread_t));
    pThreadPool->threadNum = num;
    pThreadPool->taskQueue.pFront = NULL;
    pThreadPool->taskQueue.pRear = NULL;
    pThreadPool->taskQueue.size = 0;

    pthread_mutex_init(&pThreadPool->mutex, NULL);
    pthread_cond_init(&pThreadPool->cond, NULL);

    pThreadPool->threadIsRunning = 1;
    return 0;
}