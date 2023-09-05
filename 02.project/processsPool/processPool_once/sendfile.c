#include <myfunc.h>
#include "head.h"

int sendFile(int netFd)
{
    train_t train = {5, "file1"};
    int fd = open("file1", O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    int ret = send(netFd, &train, sizeof(train.dataLength) + train.dataLength, MSG_NOSIGNAL);
    ERROR_CHECK(ret, -1, "send");

    struct stat statbuf;
    ret = fstat(fd, &statbuf);
    ERROR_CHECK(ret, -1, "fstat");
    train.dataLength = sizeof(statbuf.st_size);
    memcpy(&train.buf, &statbuf.st_size, sizeof(statbuf.st_size));
    send(netFd, &train, sizeof(train.dataLength) + train.dataLength, MSG_NOSIGNAL);

    char *p = (char *)mmap(NULL, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    ERROR_CHECK(p, MAP_FAILED, "mmap");
    send(netFd, p, statbuf.st_size, MSG_NOSIGNAL);
    munmap(p, statbuf.st_size);
    
    //sendfile(netFd, fd, NULL, statbuf.st_size);

    train.dataLength = 0;
    // send(netFd, &train, sizeof(train.dataLength), MSG_NOSIGNAL);
    close(netFd);
    return 0;
}