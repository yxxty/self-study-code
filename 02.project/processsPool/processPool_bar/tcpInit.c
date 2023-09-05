#include <myfunc.h>
#include "head.h"

int tcpInit(int *sockFd, char *ip, char *port)
{
    puts("tcpInit");
    *sockFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ip);
    int reuse = 1;
    int ret = setsockopt(*sockFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    ERROR_CHECK(ret, -1, "setsockopt");
    ret = bind(*sockFd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(*sockFd, 10);
    ERROR_CHECK(ret, -1, "listen");

    return 0;
}
