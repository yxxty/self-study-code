#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t sz)
: _queSize(sz)
, _que()
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
, _flag(true)
{

}

TaskQueue::~TaskQueue()
{

}

bool TaskQueue::empty() const
{
    return _que.size() == 0;
}

bool TaskQueue::full() const
{
    return _queSize == _que.size();
}

void TaskQueue::push(ElemType ptask)
{
    //RAII的思想
    MutexLockGuard autoLock(_mutex);//利用栈对象的生命周期管理资源
    while(full())
    {
        _notFull.wait();
    }
    //else return;

    _que.push(ptask);

    _notEmpty.notify();

    /* _mutex.unlock(); */
}

ElemType TaskQueue::pop()
{
    MutexLockGuard autoLock(_mutex);//利用栈对象的生命周期管理资源
    //如果不加_flag此时即使将非空条件变量上线程唤醒，也没有用处
    //继续在while进行循环，不会跳出
    while(_flag && empty())
    {
        _notEmpty.wait();
    }

    if(_flag)
    {
        ElemType number = _que.front();
        _que.pop();

        _notFull.notify();

        return number;
    }
    else
    {
        return nullptr;
    }

}

void TaskQueue::wakeup()
{
    _flag = false;
    _notEmpty.notifyAll();
}
