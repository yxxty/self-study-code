#ifndef __PRODUCER_H__
#define __PRODUCER_H__

#include "Thread.h"
#include "TaskQueue.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

class Producer
: public Thread
{
public:
    Producer(TaskQueue &taskQueue)
    : _taskQueue(taskQueue)
    {

    }

    ~Producer()
    {

    }

    void run()
    {
        ::srand(clock());
        int cnt = 20;
        while(cnt-- > 0)
        {
            int number = ::rand()%100;
            _taskQueue.push(number);
            cout << "--->pruducer number " << number << endl;
            sleep(1);
        }
    }

private:
    TaskQueue &_taskQueue;

};

#endif
