#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <errno.h>

#define SERV_PORT 8888
#define OPEN_MAX 1024

int main()
{
    int i, j, n, maxi;
    int nready, ret;
    int listenfd, connfd, sockfd;
    char buf[BUFSIZ], str[INET_ADDRSTRLEN]; 
    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    struct pollfd client[OPEN_MAX];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == listenfd)
    {
        perror("socket error");
        exit(-1);
    }

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);//本地字节序port与ip都要转换为网络字节序
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//因为要在网络上传输

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

    client[0].fd = listenfd;
    client[0].events = POLLIN;

    for(i = 1; i < OPEN_MAX; ++i)
    {
        client[i].fd = -1;//将数组初始化为-1
    }

    maxi = 0;

    while(1)
    {
        nready = poll(client, maxi + 1, -1);
        if(nready < 0)
        {
            perror("poll error");
            exit(-1);
        }

        if(client[0].revents & POLLIN)
        {
            clie_addr_len = sizeof(clie_addr);
            connfd = accept(listenfd, (struct sockaddr *)&clie_addr, &clie_addr_len);//立即连接，此时不会阻塞等
            if(-1 == connfd)
            {
                perror("accept error");
                exit(-1);
            }

            printf("received from %s at port %d\n",
                   inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, str, sizeof(str)),
                   ntohs(clie_addr.sin_port));

            for(i = 1; i < OPEN_MAX; ++i)
            {
                if(client[i].fd < 0)//因为初始化为-1,所以在此作为判断条件
                {
                    client[i].fd = connfd;
                    break;//直接跳出，免得继续判断,浪费时间
                }
            }

            if(i == OPEN_MAX)//select监听的文件描述符有上限，最大只能监听1024个
            {
                fputs("too many clients\n", stderr);
                exit(1);
            }

            client[i].events = POLLIN;

            if(i > maxi)
            {
                maxi = i;//因为文件描述符有新增，导致自定义数组有变化，所以需要重新修改maxi的值
            }

            if(--nready == 0)//意思不明确
            {
                continue;
            }
        }

        for(i = 1; i <= maxi; ++i)
        {
            if((sockfd = client[i].fd) < 0)
            {
                continue;
            }

            if(client[i].revents & POLLIN)
            {
                if((n = read(sockfd, buf, sizeof(buf))) < 0)
                {
                    if(errno == ECONNRESET)
                    {
                        printf("client[%d] abort connect\n", i);
                        close(sockfd);
                        client[i].fd = -1;
                    }
                    else
                    {
                        perror("read n = 0 error");
                    }
                }
                else if(n > 0)
                {
                    for(j = 0; j < n; ++j)
                    {
                        buf[j] = toupper(buf[j]);
                    }
                    write(sockfd, buf, n);
                    write(STDOUT_FILENO, buf, n);
                }
                else
                {
                    close(sockfd);
                    client[i].fd = -1;
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
