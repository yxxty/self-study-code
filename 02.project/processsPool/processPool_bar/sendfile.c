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

    //1
    while(1){
        bzero(&train, sizeof(train));
        ret = read(fd, train.buf, sizeof(train.buf));
        ERROR_CHECK(ret, -1, "read");
        if(ret != 1000){
            printf("ret = %d\n", ret);
        }
        if(ret == 0){
            break;
        }
        train.dataLength = ret;
        send(netFd, &train, sizeof(train.dataLength) + train.dataLength, MSG_NOSIGNAL);
    }

    //2 mmap
    // char *p = (char *)mmap(NULL, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    // ERROR_CHECK(p, MAP_FAILED, "mmap");
    // off_t total = 0;
    // while(total < statbuf.st_size){
    //     // bzero(&train, sizeof(train));
    //     if(statbuf.st_size - total < sizeof(train.buf)){
    //         //最后一次发送
    //         train.dataLength = statbuf.st_size - total;
    //     }
    //     else{
    //         train.dataLength = sizeof(train.buf);
    //     }
    //     memcpy(train.buf, p + total, train.dataLength);
    //     ret = send(netFd, &train, sizeof(train.dataLength) + train.dataLength, MSG_NOSIGNAL);
    //     ERROR_CHECK(ret, -1, "send");
    //     total += train.dataLength;
    // }
    // munmap(p, statbuf.st_size);

    train.dataLength = 0;
    send(netFd, &train, sizeof(train.dataLength), MSG_NOSIGNAL);
    close(netFd);
    return 0;
}