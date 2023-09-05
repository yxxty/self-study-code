#include "head.h"

int makeWorker(threadPool_t *pThreadPool)
{
    for(int i = 0; i < pThreadPool->threadNum; ++i){
        pthread_create(pThreadPool->tidArr + i, NULL, threadFunc, (void *)pThreadPool);
    }
    return 0;
}
void unlock(void *arg)
{
    pthread_mutex_t *pMutex = (pthread_mutex_t *)arg;
    pthread_mutex_unlock(pMutex);
}
void *threadFunc(void *args)
{
    threadPool_t *pThreadPool = (threadPool_t *)args;
    int netFd;
    while(1){
        pthread_mutex_lock(&pThreadPool->mutex);
        pthread_cleanup_push(unlock, (void *)&pThreadPool->mutex);
        while(pThreadPool->taskQueue.size == 0){
            // puts("thread wait");
            if(pThreadPool->threadIsRunning == 0){
                puts("child quit");
                pthread_exit(NULL);
            }
            pthread_cond_wait(&pThreadPool->cond, &pThreadPool->mutex);
        }
        printf("pid = %ld Get work\n", pthread_self());
        netFd = pThreadPool->taskQueue.pFront->netFd;
        deQueue(&pThreadPool->taskQueue);
        pthread_cleanup_pop(1);

        handleEvent(netFd);
    }
}
int handleEvent(int netFd)
{
    // puts("handelEvent");
    transFile(netFd);
    return 0;
}