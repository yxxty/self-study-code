#include "TcpServer.h"
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

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
    // deconde
    // compute
    // encode
    //
    con->send(msg);
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
    TcpServer ts("127.0.0.1", 8888);
    ts.setAllCallback(onConnection, onMessage, onClose);
    ts.start();
}

int main(int argc, char **argv)
{
    test2();
    return 0;
}
