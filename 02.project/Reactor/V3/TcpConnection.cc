#include "TcpConnection.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ostringstream;

TcpConnection::TcpConnection(int fd)
    : _sock(fd), _sockIO(fd), _localAddr(getLocalAddr()), _peerAddr(getPeerAddr())

{
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::send(const string &msg)
{
    _sockIO.writen(msg.c_str(), msg.size());
}

string TcpConnection::receive()
{
    char buff[65535] = {0};
    _sockIO.readLine(buff, sizeof(buff));

    return string(buff);
}

string TcpConnection::toString()
{
    ostringstream oss;
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "---->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();

    return oss.str();
}

bool TcpConnection::isClosed() const
{
    char buff[10] = {0};
    // 第四个参数写成MSG_PEEK
    // 只关心连接有没有结束，不想对缓冲区中的数据做任何更改
    // 第四个参数如果是0，会将数据从内核缓冲区中移走
    int ret = recv(_sock.fd(), buff, sizeof(buff), MSG_PEEK);
    if (ret) {
        return false;
    }
    else {
        return true;
    }
}

void TcpConnection::setConnectionCallback(const TcpConnectionCallback &cb)
{
    _onConnectinCb = std::move(cb);
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb)
{
    _onMessageCb = std::move(cb);
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb)
{
    _onCloseCb = std::move(cb);
}

void TcpConnection::handleConnectionCallback()
{
    if (_onConnectinCb) {
        // shared_ptr<TcpConnection>
        // TcpConnectionPtr
        _onConnectinCb(shared_from_this());
    }
}

void TcpConnection::handleMessageCallback()
{
    if (_onMessageCb) {
        _onMessageCb(shared_from_this());
    }
}

void TcpConnection::handleCloseCallback()
{
    if (_onCloseCb) {
        _onCloseCb(shared_from_this());
    }
}

InetAddress TcpConnection::getLocalAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getsockname(_sock.fd(), (struct sockaddr *)&addr, &len);
    if (-1 == ret) {
        perror("getsockname");
    }

    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getpeername(_sock.fd(), (struct sockaddr *)&addr, &len);
    if (-1 == ret) {
        perror("getpeername");
    }

    return InetAddress(addr);
}
