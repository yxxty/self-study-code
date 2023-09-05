#include "Thread.h"
#include <stdio.h>

Thread::Thread()
: _thid(0)
, _isRunning(false)
{

}

Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_thid);
        _isRunning = false;
    }
}

//pthread->start();
void Thread::start()
{
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if(ret)
    {
        perror("pthread_create");
        return;
    }

    _isRunning = true;
}

void Thread::join()
{
    if(_isRunning)
    {
        pthread_join(_thid, nullptr);
        _isRunning = false;
    }
}

void *Thread::threadFunc(void *arg)
{
    Thread *pth = (Thread *)arg;
    if(pth)
    {
        pth->run();//调用的run方法,体现多态
    }

    pthread_exit(nullptr);
}
