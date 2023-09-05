#include <myfunc.h>
#include "taskQueue.h"

int enQueue(taskQueue_t *pTaskQueue, int netFd)
{
    task_t *pNew = (task_t *)calloc(1, sizeof(task_t));
    pNew->netFd = netFd;
    if(pTaskQueue->size == 0){
        pTaskQueue->pFront = pNew;
        pTaskQueue->pRear = pNew;
    }
    else{
        pTaskQueue->pRear->pNext = pNew;
        pTaskQueue->pRear = pNew;
    }
    ++pTaskQueue->size;
    return 0;
}
int deQueue(taskQueue_t *pTaskQueue)
{
    task_t *pFree = pTaskQueue->pFront;
    pTaskQueue->pFront = pTaskQueue->pFront->pNext;
    free(pFree);
    --pTaskQueue->size;
    return 0;
}
