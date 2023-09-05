#include "EventLoop.h"
#include <sys/eventfd.h>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

EventLoop::EventLoop(Acceptor &acceptor)
    : _epfd(createEpollFd())
    , _evtfd(createEventFd())
    , _acceptor(acceptor)
    , _isLooping(false)
    , _vecList(1024)
{
    addEpollReadFd(acceptor.fd());
    addEpollReadFd(_evtfd);
}

EventLoop::~EventLoop()
{
    if (_epfd) {
        close(_epfd);
    }

    if (_evtfd) {
        close(_evtfd);
    }
}

void EventLoop::loop()
{
    _isLooping = true;
    while (_isLooping) {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    _isLooping = false;
}

// 执行所有在vector中注册的任务
void EventLoop::doPengdingFunctors()
{
    // 先将老的任务取出来，使用vector暂存
    vector<Functor> tmp;
    {
        MutexLockGuard autoLock(_mutex);
        tmp.swap(_pendingsCb);
    }

    // 直接执行任务,可以同时向老的vector里面添加任务
    for (auto &cb : tmp) {
        cb();
    }
}

// 向vector中添加任务
void EventLoop::runInLoop(Functor &&cb)
{
    {
        MutexLockGuard autoLock(_mutex);
        _pendingsCb.push_back(std::move(cb));
    }

    wakeup();
}

void EventLoop::waitEpollFd()
{
    int nready = -1;
    do {
        nready = ::epoll_wait(_epfd, &*_vecList.begin(), _vecList.size(), 5000);
    } while (-1 == nready && errno == EINTR);

    if (-1 == nready) {
        perror("epoll_wait -1 == nready");
        return;
    }
    else if (0 == nready) {
        printf("epoll_wait timeout\n");
    }
    else {
        if ((int)_vecList.size() == nready) {
            _vecList.resize(2 * nready);
        }

        for (int idx = 0; idx < nready; ++idx) {
            int fd = _vecList[idx].data.fd;
            if (fd == _acceptor.fd()) {// 新的连接请求进来了 
                if (_vecList[idx].events & EPOLLIN) {
                    handleNewConnection();
                }
            }
            else if (fd == _evtfd) {
                handleRead(); // read eventfd
                doPengdingFunctors();
            }
            else {
                if (_vecList[idx].events & EPOLLIN) {
                    handleMessage(fd);
                }
            }
        }
    }
}

void EventLoop::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    addEpollReadFd(peerfd);

    // 创建的TcpConnection
    // 通过this将EventLoop的存在告知TcpConnection
    TcpConnectionPtr con(new TcpConnection(peerfd, this));

    // 三个回调函数的注册
    con->setConnectionCallback(_onConnectinCb);
    con->setMessageCallback(_onMessageCb);
    con->setCloseCallback(_onCloseCb);

    _conns.insert(std::make_pair(peerfd, con));

    // 执行回调函数
    con->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd)
{
    auto it = _conns.find(fd);
    if (it != _conns.end()) {
        bool flag = it->second->isClosed();
        if (flag) {
            it->second->handleCloseCallback();
            delEpollReadFd(fd); // 从红黑树上删除j
            _conns.erase(it);   // 从map中删除
        }
        else {
            it->second->handleMessageCallback();
        }
    }
    else {
        cout << "该连接是不存在" << endl;
    }
}
int EventLoop::createEpollFd()
{
    int fd = epoll_create1(0);
    if (fd == -1) {
        perror("epoll_create1");
    }

    return fd;
}

void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
    if (ret < 0) {
        perror("epoll_ctl add");
        return;
    }
}
void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &ev);
    if (ret < 0) {
        perror("epoll_ctl add");
        return;
    }
}
void EventLoop::setConnectionCallback(TcpConnectionCallback &&cb)
{
    _onConnectinCb = std::move(cb);
}

void EventLoop::setMessageCallback(TcpConnectionCallback &&cb)
{
    _onMessageCb = std::move(cb);
}

void EventLoop::setCloseCallback(TcpConnectionCallback &&cb)
{
    _onCloseCb = std::move(cb);
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    int ret = read(_evtfd, &one, sizeof(one));
    if (ret != sizeof(one)) {
        perror("read");
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    int ret = write(_evtfd, &one, sizeof(one));
    if (ret != sizeof(one)) {
        perror("write");
    }
}

int EventLoop::createEventFd()
{
    int fd = eventfd(0, 0);
    if (-1 == fd) {
        perror("eventfd");
    }

    return fd;
}
