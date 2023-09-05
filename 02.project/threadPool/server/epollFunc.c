#include <myfunc.h>
#include "head.h"

int epollAdd(int epFd, int addFd)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = addFd;
    int ret = epoll_ctl(epFd, EPOLL_CTL_ADD, addFd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    return 0;
}
int epollDel(int epFd, int delFd)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = delFd;
    int ret = epoll_ctl(epFd, EPOLL_CTL_DEL, delFd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    return 0;
}