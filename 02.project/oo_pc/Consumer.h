#ifndef __CONSUMER_H__
#define __CONSUMER_H__

#include "Thread.h"
#include "TaskQueue.h"
#include <unistd.h>

#include <iostream>

using std::cout;
using std::endl;

class Consumer
: public Thread
{
public:
    Consumer(TaskQueue &taskQueue)
    : _taskQueue(taskQueue)
    {

    }

    ~Consumer() {}

    void run()
    {

        /* int cnt = 20; */
        /* while(cnt-- > 0) */
        while(1)
        {
            int number = _taskQueue.pop();
            cout << ">>consume number = " << number << endl;
            sleep(1);
        }
    }

private:
    TaskQueue &_taskQueue;

};

#endif
