#include "ThreadPool.h"
#include "WorkThread.h"
#include "Task.h"
#include <unistd.h>

ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
: _threadNum(threadNum)
, _queSize(queSize)
, _taskQue(queSize) 
, _isExit(false)
{
    _threads.reserve(threadNum);
}

ThreadPool::~ThreadPool()
{
    if(!_isExit)
    {
        stop();
    }

}

void ThreadPool::addTask(Task *ptask)
{
    if(ptask)
    {
        _taskQue.push(ptask);
    }
}

Task *ThreadPool::getTask()
{
    return _taskQue.pop();
}


void ThreadPool::start()
{
    for(size_t idx = 0; idx != _threadNum; ++idx)
    {
        unique_ptr<Thread> pthread(new WorkThread(*this));
        _threads.push_back(std::move(pthread));
    }

    for(auto &th : _threads)
    {
        th->start();//启动WorkThread
    }
}

void ThreadPool::stop()
{
#if 1
    while(!_taskQue.empty())
    {
        sleep(1);
    }
#endif
    _isExit = true;
    _taskQue.wakeup();

    for(auto &th : _threads)
    {
        th->join();
    }
}


void ThreadPool::threadFunc()
{
    while(!_isExit)
    {
#if 0
        if(_taskQue.empty())
        {
            //执行分支，不去getTask
        }
        else
        {
            //任务队列不为空的时候，才去获取任务
        }
#endif
        Task *ptask = getTask();
        if(ptask)
        {
            ptask->process();
        }

    }
}
