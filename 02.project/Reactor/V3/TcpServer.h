#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "EventLoop.h"
#include "Acceptor.h"

class TcpServer
{
public:
    TcpServer(const string &ip, unsigned short port)
        : _acceptor(ip, port), _loop(_acceptor)
    {
    }

    void start()
    {
        _acceptor.ready();
        _loop.loop();
    }

    void stop()
    {
        _loop.unloop();
    }

    void setAllCallback(TcpConnectionCallback &&onConnection,
                        TcpConnectionCallback &&onMessage,
                        TcpConnectionCallback &&onClosed)
    {
        _loop.setConnectionCallback(std::move(onConnection));
        _loop.setMessageCallback(std::move(onMessage));
        _loop.setCloseCallback(std::move(onClosed));
    }

    ~TcpServer() {}

private:
    Acceptor _acceptor;
    EventLoop _loop;
};

#endif
