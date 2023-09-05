#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <ctype.h>

#define SERV_PORT 8888
#define OPEN_MAX 5000

int main()
{
    int listenfd, connfd, sockfd, epfd;
    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    int ret, i, j, nready, nByte;
    char buf[BUFSIZ], str[BUFSIZ];
    struct epoll_event evt, ep[OPEN_MAX];


    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == listenfd)
    {
        perror("socket error");
        exit(-1);
    }

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(-1 == ret)
    {
        perror("bind error");
        exit(-1);
    }

    ret = listen(listenfd, 128);
    if(-1 == ret)
    {
        perror("listen error");
        exit(-1);
    }

    epfd = epoll_create(OPEN_MAX);
    if(-1 == epfd)
    {
        perror("epoll_create error");
        exit(-1);
    }

    evt.events = EPOLLIN;
    evt.data.fd = listenfd;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &evt);
    if(-1 == ret)
    {
        perror("epoll_ctl error");
        exit(-1);
    }

    while(1)
    {
        nready = epoll_wait(epfd, ep, OPEN_MAX, -1);
        if(nready < 0)
        {
            perror("select error");
            exit(-1);
        }

        for(i = 0; i < nready; ++i)
        {
            if(!(ep[i].events & EPOLLIN))
            {
                continue;
            }

            if(ep[i].data.fd == listenfd)//如果是连接事件
            {
                clie_addr_len = sizeof(clie_addr);
                connfd = accept(listenfd, (struct sockaddr *)&clie_addr, &clie_addr_len);
                if(-1 == connfd)
                {
                    perror("accept error");
                    exit(-1);
                }
                printf("receive from %s from port %d\n", 
                       inet_ntop(AF_INET, &clie_addr.sin_addr, str, sizeof(str)),
                       ntohs(clie_addr.sin_port));

                evt.events = EPOLLIN;
                evt.data.fd = connfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &evt);
            }
            else //不是连接建立事件，而是读写事件(信息传递事件)
            {
                sockfd = ep[i].data.fd;
                nByte = read(sockfd, buf, sizeof(buf));
                if(nByte == 0)
                {
                    ret = epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
                    if(-1 == ret)
                    {
                        perror("epoll_ctl error");
                    }
                    close(sockfd);
                    printf("client[%d] closed connection\n", sockfd);
                }
                else if(nByte < 0)
                {
                    perror("epoll_ctl error");
                    ret = epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
                    if(-1 == ret)
                    {
                        perror("epoll_ctl error");
                    }
                    close(sockfd);

                }
                else 
                {
                    for(j = 0; j < nByte; ++j)
                    {
                        buf[j] = toupper(buf[j]);
                    }
                    write(sockfd, buf, nByte);
                    write(STDOUT_FILENO, buf, nByte);
                }
            }
        }
    }

    close(listenfd);
    close(connfd);

    return 0;
}
