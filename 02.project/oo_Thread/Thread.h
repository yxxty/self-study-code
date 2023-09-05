#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

class Thread
{
public:
    Thread();
    ~Thread();
    void start();
    void join();

private:
    //需要实现的任务
    virtual void run() = 0;
    //消除this指针的影响
    static void *threadFunc(void *);
private:
    pthread_t _thid;
    bool _isRunning;
};

#endif
