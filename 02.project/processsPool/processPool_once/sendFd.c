#include <myfunc.h>
#include "head.h"
int sendFd(int pipeFd, int fdToSend)
{
    struct msghdr hdr = {0};
    char buf[] = "Hello";
    struct iovec iov = {
        .iov_base = buf,
        .iov_len = sizeof(buf)
    };
    hdr.msg_iov = &iov;
    hdr.msg_iovlen = 1;
    struct cmsghdr *phdr = (struct cmsghdr *)calloc(1, sizeof(struct cmsghdr));
    phdr->cmsg_len = CMSG_LEN(sizeof(int));// 传递的是一个文件描述符
    phdr->cmsg_level = SOL_SOCKET;
    phdr->cmsg_type = SCM_RIGHTS;
    *(int *)CMSG_DATA(phdr) = fdToSend;
    hdr.msg_control = phdr;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    int ret = sendmsg(pipeFd, &hdr, 0);
    ERROR_CHECK(ret, -1, "sendmsg");
    return 0;
}
int recvFd(int pipeFd, int *pFd)
{
    struct msghdr hdr = {0};
    char buf[6] = {0};
    struct iovec iov = {
        .iov_base = buf,
        .iov_len = sizeof(buf)
    };
    hdr.msg_iov = &iov;
    hdr.msg_iovlen = 1;
    struct cmsghdr *phdr = (struct cmsghdr *)calloc(1, sizeof(struct cmsghdr));
    phdr->cmsg_len = CMSG_LEN(sizeof(int));// 传递的是一个文件描述符
    phdr->cmsg_level = SOL_SOCKET;
    phdr->cmsg_type = SCM_RIGHTS;
    hdr.msg_control = phdr;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    int ret = recvmsg(pipeFd, &hdr, 0);
    ERROR_CHECK(ret, -1, "sendmsg");
    *pFd = *(int *)CMSG_DATA(phdr);
    return 0;
}