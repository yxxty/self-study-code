#include "TaskQueue.h"
#include "Producer.h"
#include "Consumer.h"
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;

void test()
{
    TaskQueue taskQueue(10);
    unique_ptr<Thread> producer1(new Producer(taskQueue));
    unique_ptr<Thread> producer2(new Producer(taskQueue));
    unique_ptr<Thread> consumer(new Consumer(taskQueue));

    producer1->start();
    producer2->start();
    consumer->start();

    producer1->join();
    producer2->join();
    consumer->join();
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

