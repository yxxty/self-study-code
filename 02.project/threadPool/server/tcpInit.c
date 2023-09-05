#include <myfunc.h>
#include "head.h"

int tcpInit(int *pSockFd, char *ip, char *port)
{
    *pSockFd = socket(AF_INET, SOCK_STREAM, 0);

    int reuse = 1;
    setsockopt(*pSockFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ip);
    int ret = bind(*pSockFd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");
    listen(*pSockFd, 10);
    return 0;
}
