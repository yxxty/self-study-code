#ifndef __HEAD_H__
#define __HEAD_H__
#include "threadPool.h"
int makeWorker(threadPool_t *pThreadPool);
void *threadFunc(void *args);
int handleEvent(int netFd);
int tcpInit(int *pSockFd, char *ip, char *port);
int epollAdd(int epFd, int addFd);
int epollDel(int epFd, int delFd);
int transFile(int netFd);

typedef struct train_s{
    int dataLength;
    char buf[1000];
} train_t;
#endif