#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "Thread.h"
#include "TaskQueue.h"
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

class ThreadPool
{
    friend class WorkThread;
public:
    ThreadPool(size_t, size_t);
    ~ThreadPool();
    void addTask(Task *ptask);
    Task *getTask();
    void start();
    void stop();
private:
    //交个工作线程进行实现的任务
    void threadFunc();
private:
    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskQue;
    bool _isExit;
};

#endif
