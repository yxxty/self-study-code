#include <myfunc.h>
#include "threadPool.h"
#include "head.h"

int exitPipes[2];
void sigFunc(int signum)
{
    printf("signal comming! signum = %d\n", signum);
    write(exitPipes[1], "1", 1);
}

int main(int argc, char *argv[])
{
    // ./server 192.168.92.129 1234 3
    ARGS_CHECK(argc, 4);

    pipe(exitPipes);
    if(fork() != 0){
        // 主进程
        close(exitPipes[0]);
        signal(SIGUSR1, sigFunc);
        wait(NULL);
        exit(0);
    }
    close(exitPipes[1]);

    int workerNum = atoi(argv[3]);
    threadPool_t threadPool;
    threadPoolInit(&threadPool, workerNum);
    makeWorker(&threadPool);
    int sockFd;
    tcpInit(&sockFd, argv[1], argv[2]);

    int epFd = epoll_create(10);
    // printf("sockFd = %d, pipeFd = %d\n", sockFd, exitPipes[0]);
    epollAdd(epFd, sockFd);
    epollAdd(epFd, exitPipes[0]);

    struct epoll_event readyList[2];
    while(1){
        int readyNum = epoll_wait(epFd, readyList, 2, -1);
        for(int i = 0; i < readyNum; ++i){
            if(sockFd == readyList[i].data.fd){
                int netFd = accept(sockFd, NULL, NULL);
                pthread_mutex_lock(&threadPool.mutex);
                enQueue(&threadPool.taskQueue, netFd);
                puts("New task");
                pthread_cond_signal(&threadPool.cond);
                pthread_mutex_unlock(&threadPool.mutex);
            }
            else if(exitPipes[0] == readyList[i].data.fd){
                // 粗暴退出
                // for(int j = 0; j < workerNum; ++j){
                //     pthread_cancel(threadPool.tidArr[j]);
                // }

                // 优雅退出
                puts("I am going to die!");
                threadPool.threadIsRunning = 0;
                pthread_cond_broadcast(&threadPool.cond);

                for(int j = 0; j < workerNum; ++j){
                    pthread_join(threadPool.tidArr[j], NULL);
                }
                puts("Exit!");
                exit(0);
            }
            else{}
        }
    }

    return 0;
}