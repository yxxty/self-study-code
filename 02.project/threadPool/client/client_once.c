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

    long fileSize;
    recvn(netFd, &dataLenth, sizeof(int));
    recvn(netFd, &fileSize, sizeof(fileSize));
    printf("fileSize = %ld\n", fileSize);
    ftruncate(fd, fileSize);

    struct timeval begTime, endTime;
    gettimeofday(&begTime, NULL);
    
    // char *p = (char *)mmap(NULL, fileSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    // ERROR_CHECK(p, MAP_FAILED, "mmap");
    // recvn(netFd, p, fileSize);
    // munmap(p, fileSize);    
    
    int pipefds[2];
    pipe(pipefds);
    off_t total = 0;
    while(total < fileSize){
        int ret = splice(netFd, NULL, pipefds[1], NULL, 4096, SPLICE_F_MORE);
        splice(pipefds[0], NULL, fd, NULL, ret, SPLICE_F_MORE);
        total += ret;
    }

    gettimeofday(&endTime, NULL);
    printf("time = %ld\n", 1000000 * (endTime.tv_sec - begTime.tv_sec) + (endTime.tv_usec - begTime.tv_usec));
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
    ret = send(sockFd, "hihihi", 6, 0);

    recvFile(sockFd);

    close(sockFd);
    return 0;
}