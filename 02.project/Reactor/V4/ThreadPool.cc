#include "ThreadPool.h"
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
    if (!_isExit) {
        stop();
    }
}

// using Task = std::function<void()>
void ThreadPool::addTask(Task &&task)
{
    if (task) {
        _taskQue.push(std::move(task));
    }
}

/* function<void()> ThreadPool::getTask() */
Task ThreadPool::getTask()
{
    return _taskQue.pop();
}

void ThreadPool::start()
{
    for (size_t idx = 0; idx != _threadNum; ++idx) {
        // 基于对象的写法，将所有的任务写成一个返回类型是void，参数是void一个函数
        unique_ptr<Thread> pthread(new Thread(std::bind(&ThreadPool::threadFunc, this)));
        _threads.push_back(std::move(pthread));
    }

    for (auto &th : _threads) {
        th->start(); // 启动WorkThread
    }
}

void ThreadPool::stop()
{
    while (!_taskQue.empty()) {
        sleep(1);
    }

    _isExit = true;
    _taskQue.wakeup();

    for (auto &th : _threads) {
        th->join();
    }
}

void ThreadPool::threadFunc()
{
    while (!_isExit) {
        Task taskcb = getTask();
        if (taskcb) {
            taskcb(); // 执行了回调函数
        }
    }
}
