#include <myfunc.h>
#include "head.h"

int transFile(int netFd)
{
    train_t train = {5, "file1"};
    int fd = open("file1", O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    int ret = send(netFd, &train, sizeof(train.dataLength) + train.dataLength, MSG_NOSIGNAL);
    ERROR_CHECK(ret, -1, "send");

    bzero(&train, sizeof(train));
    struct stat statBuf;
    fstat(fd, &statBuf);
    train.dataLength = sizeof(statBuf.st_size);
    memcpy(train.buf, &statBuf.st_size, sizeof(statBuf.st_size));
    ret = send(netFd, &train, sizeof(train.dataLength) + train.dataLength, MSG_NOSIGNAL);

    while(1){
        bzero(&train, sizeof(train));
        ret = read(fd, train.buf, sizeof(train.buf));
        ERROR_CHECK(ret, -1, "read");
        if(ret != 1000){
            printf("final send = %d\n", ret);
        }
        if(ret == 0){
            break;
        }
        train.dataLength = ret;
        ret = send(netFd, &train, sizeof(train.dataLength) + train.dataLength, MSG_NOSIGNAL);
        ERROR_CHECK(ret, -1, "send");
    }
    train.dataLength = 0;
    send(netFd, &train, sizeof(train.dataLength), MSG_NOSIGNAL);
    close(netFd);
    return 0;
}