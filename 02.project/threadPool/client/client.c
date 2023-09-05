#include <myfunc.h>
int recvn(int netFd, void *p, int len)
{
    int total = 0;
    int ret;
    while(total < len){
        ret = recv(netFd, p + total, len - total, 0);
        ERROR_CHECK(ret, -1, "recv");
        total += ret;
    }
    return 0;
}

int recvFile(int netFd)
{
    int dataLenth;
    char name[1024] = {0};
    int ret = recvn(netFd, &dataLenth, sizeof(int));
    ERROR_CHECK(ret, -1, "recv");
    ret = recvn(netFd, name, dataLenth);
    ERROR_CHECK(ret, -1, "recv");
    int fd = open(name, O_RDWR | O_CREAT, 0666);
    ERROR_CHECK(fd, -1, "open");

    char buf[1024] = {0};
    while(1){
        ret = recvn(netFd, &dataLenth, sizeof(int));
        ERROR_CHECK(ret, -1, "recv");
        if(dataLenth == 0){
            break;
        }
        if(dataLenth != 1000){
            printf("dataLenth = %d\n", dataLenth);
        }
        bzero(buf, sizeof(buf));
        ret = recvn(netFd, buf, dataLenth);
        ERROR_CHECK(ret, -1, "recv");
        ret = write(fd, buf, dataLenth);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // ./client 192.168.92.129 1234
    ARGS_CHECK(argc, 3);
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = connect(sockFd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");

    recvFile(sockFd);

    close(sockFd);
    return 0;
}