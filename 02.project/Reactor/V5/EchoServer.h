#ifndef __ECHOSERVER_H__
#define __ECHOSERVER_H__

#include "TcpServer.h"
#include "ThreadPool.h"
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

// 会在线程池中进行执行
class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &conn)
        : _msg(msg)
        , _conn(conn)
    {
    }
    void process()
    {
        // msg
        // 处理业务逻辑
        //...
        // msg + 1
        //...
        //..
        // msg * 1;
        //...
        /* send(newMsg); */
        // send函数需要靠Reactor/EventLoop进行发送
        _conn->sendInLoop(_msg); // 不能在此发送
        // TcpConnection需要知道EventLoop的存在

        // 如何将新的msg，处理完成之后的数据传递给IO线程，也就是Reactor，也就是
        // EventLoop
        //
        // 此时需要线程池（计算线程）与Reactor/EventLoop（IO线程进行通信）
    }

private:
    string _msg;
    TcpConnectionPtr _conn;
};

class EchoServer
{
public:
    EchoServer(size_t threadNum, size_t queSize,
               const string &ip, unsigned short port)
        : _pool(threadNum, queSize)
        , _server(ip, port)
    {
    }

    void start()
    {
        _pool.start();
        _server.setAllCallback(std::bind(&EchoServer::onConnection, this, std::placeholders::_1),
                               std::bind(&EchoServer::onMessage, this, std::placeholders::_1),
                               std::bind(&EchoServer::onClose, this, std::placeholders::_1));
        _server.start();
    }

    void stop()
    {
        _pool.stop();
        _server.stop();
    }

    void onConnection(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has connected!" << endl;
    }

    void onMessage(const TcpConnectionPtr &con)
    {
        // 回显
        string msg = con->receive();
        cout << "recv msg from client : " << msg << endl;
        // 将从客户端发送过来的数据进行逻辑处理
        //
        // decode
        // compute
        // encode
        //
        MyTask task(msg, con);
        _pool.addTask(std::bind(&MyTask::process, task));
        /* con->send(msg); */
    }

    void onClose(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has colsed!" << endl;
    }

private:
    ThreadPool _pool;
    TcpServer _server;
};

#endif
