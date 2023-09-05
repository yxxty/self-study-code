#include <myfunc.h>
#include "worker.h"
#include "head.h"

int main(int argc, char*argv[])
{
    // ./server 192.168.92.129 1234 3
    ARGS_CHECK(argc, 4);

    int workNum = atoi(argv[3]);
    WorkerData_t *workerDataArr = (WorkerData_t *)calloc(workNum, sizeof(WorkerData_t));
    makeWorker(workerDataArr, workNum);

    int sockFd;
    tcpInit(&sockFd, argv[1], argv[2]);
    int epfd = epoll_create(10);
    epollAdd(epfd, sockFd);
    for(int i = 0; i < workNum; ++i){
        epollAdd(epfd, workerDataArr[i].pipeFd);
    }
    int queueSize = workNum + 1;
    struct epoll_event *readyQueue = (struct epoll_event *)calloc(queueSize, sizeof(struct epoll_event));
    while(1){
        int readyNum = epoll_wait(epfd, readyQueue, queueSize, -1);
        puts("epoll ready");
        for(int i = 0; i < readyNum; ++i){
            if(readyQueue[i].data.fd == sockFd){
                int netFd = accept(sockFd, NULL, NULL);
                for(int j = 0; j < workNum; ++j){
                    if(workerDataArr[j].status == FREE){
                        printf("%d worker get his job, pid = %d\n", j, workerDataArr[j].pid);
                        sendFd(workerDataArr[j].pipeFd, netFd);
                        close(netFd);
                        workerDataArr[j].status = BUSY;
                        break;
                    }
                }
            }
            else{
                puts("One worker finish his job!");
                for(int j = 0; j < workNum; ++j){
                    if(readyQueue[i].data.fd == workerDataArr[j].pipeFd){
                        pid_t pid;
                        read(workerDataArr[j].pipeFd, &pid, sizeof(pid));
                        printf("his pid = %d\n", pid);
                        workerDataArr[j].status = FREE;
                        break;
                    }
                }
            }
        }
    }

    return 0;
}