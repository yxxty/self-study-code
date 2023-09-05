#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define SERV_PORT 8888

int main()
{
    int listenfd, connfd, sockfd;
    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    int ret, maxfd, maxi, i, j, nready, nByte;
    fd_set rset, allset;
    int client[FD_SETSIZE];
    char buf[BUFSIZ], str[BUFSIZ];

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

    maxfd = listenfd;

    maxi = -1;
    for(i = 0; i < FD_SETSIZE; ++i)
    {
        client[i] = -1;
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    while(1)
    {
        rset = allset;
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if(nready < 0)
        {
            perror("select error");
            exit(-1);
        }

        if(FD_ISSET(listenfd, &rset))//有新的请求进来
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

            for(i = 0; i < FD_SETSIZE; ++i)
            {
                if(client[i] < 0)
                {
                    client[i] = connfd;
                    break;
                }
            }

            if(i == FD_SETSIZE)
            {
                fputs("too many clients\n", stderr);
                exit(1);
            }

            FD_SET(connfd, &allset);

            if(connfd > maxfd)
            {
                maxfd = connfd;
            }

            if(i > maxi)
            {
                maxi = i;
            }

            if(--nready == 0)
            {
                continue;
            }
        }

        for(i = 0; i <= maxi; ++i)
        {
            if((sockfd = client[i]) < 0)
            {
                continue;
            }

            if(FD_ISSET(sockfd, &rset))
            {
                if((nByte = read(sockfd, buf, sizeof(buf))) == 0)
                {
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }
                else if(nByte > 0)
                {
                    for(j = 0; j < nByte; ++j)
                    {
                        buf[j] = toupper(buf[j]);
                    }
                    write(sockfd, buf, nByte);
                    write(STDOUT_FILENO, buf, nByte);
                }

                if(--nready == 0)
                {
                    break;
                }
            }
        }
    }

    close(listenfd);
    close(connfd);
    return 0;
}

