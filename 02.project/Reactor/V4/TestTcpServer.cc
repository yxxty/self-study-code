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
        : _msg(msg), _conn(conn)
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

ThreadPool *gThreadPool = nullptr;

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
    gThreadPool->addTask(std::bind(&MyTask::process, task));
    /* con->send(msg); */
}

void onClose(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has colsed!" << endl;
}

void test()
{
    Acceptor acceptor("127.0.0.1", 8888);
    acceptor.ready();

    EventLoop loop(acceptor);

    loop.setConnectionCallback(std::move(onConnection));
    loop.setMessageCallback(std::move(onMessage));
    loop.setCloseCallback(std::move(onClose));
    loop.loop();
}

void test2()
{
    ThreadPool threadPool(5, 10);
    threadPool.start();
    gThreadPool = &threadPool;

    TcpServer ts("127.0.0.1", 8888);
    ts.setAllCallback(onConnection, onMessage, onClose);
    ts.start();
}

int main(int argc, char **argv)
{
    test2();
    return 0;
}
