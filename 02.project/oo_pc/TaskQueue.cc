#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t sz)
: _queSize(sz)
, _que()
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
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

void TaskQueue::push(const int &value)
{
    //RAII的思想
    MutexLockGuard autoLock(_mutex);//利用栈对象的生命周期管理资源
    /* _mutex.lock(); */
    //虚假唤醒
    /* if(full()) */
    while(full())
    {
        _notFull.wait();
    }
    //else return;

    _que.push(value);

    _notEmpty.notify();

    /* _mutex.unlock(); */
}

int TaskQueue::pop()
{
    MutexLockGuard autoLock(_mutex);//利用栈对象的生命周期管理资源
    /* _mutex.lock(); */
    /* if(empty()) */
    while(empty())
    {
        _notEmpty.wait();
    }

    int number = _que.front();
    _que.pop();

    _notFull.notify();
    /* _mutex.unlock(); */

    return number;

}
