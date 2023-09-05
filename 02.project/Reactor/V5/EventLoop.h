#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "Acceptor.h"
#include "MutexLock.h"
#include "TcpConnection.h"
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>

using std::function;
using std::map;
using std::shared_ptr;
using std::vector;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;

using Functor = function<void()>;

class EventLoop
{
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop();
    void loop();
    void unloop();
    void doPengdingFunctors(); // 执行所有的任务

    void runInLoop(Functor &&cb); // 注册任务
    void setConnectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);

    void handleRead();
    void wakeup();

private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);
    int createEpollFd();
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);
    int createEventFd();

private:
    int _epfd;
    int _evtfd;
    Acceptor &_acceptor;
    bool _isLooping;
    vector<struct epoll_event> _vecList;
    map<int, TcpConnectionPtr> _conns;

    TcpConnectionCallback _onConnectinCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;

    vector<Functor> _pendingsCb;
    MutexLock _mutex;
};

#endif
