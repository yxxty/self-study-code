#include "worker.h"
#include "head.h"

int makeWorker(WorkerData_t *data, int num)
{
    puts("makeworker");
    pid_t pid;
    for(int i = 0; i < num; ++i){
        int sockFds[2];
        int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0,sockFds);
        ERROR_CHECK(ret, -1, "socketpair");
        pid = fork();
        if (pid == 0){
            close(sockFds[0]);
            handleEvent(sockFds[1]);
        }
        close(sockFds[1]);
        data[i].pipeFd = sockFds[0];
        data[i].pid = pid;
        data[i].status = FREE;
    }

    return 0;
}

int handleEvent(int pipeFd)
{
    while(1){
        int netFd;
        recvFd(pipeFd, &netFd);
        //开始干活
        char buf[1024] = {0};
        int ret = recv(netFd, buf, sizeof(buf), 0);
        ERROR_CHECK(ret, -1, "recv");
        puts(buf);
        sendFile(netFd);
        sleep(5);
        //干完发送pid
        pid_t pid = getpid();
        write(pipeFd, &pid, sizeof(pid));
    }

    return 0;
}