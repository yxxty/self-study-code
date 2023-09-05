#ifndef __HEAD_H__
#define __HEAD_H__
typedef struct train_s{
    int dataLength;
    char buf[1000];
} train_t;
int tcpInit(int *sockFd, char *ip, char *port);
int sendFd(int pipeFd, int fdToSend, int exitFlag);
int recvFd(int pipeFd, int *pFd, int *exitFlag);
int epollAdd(int epfd, int fd);
int epollDel(int epfd, int fd);
int sendFile(int netFd);

#endif