#include "Task.h"
#include "TaskQueue.h"
#include "ThreadPool.h"
#include <stdlib.h>
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;

class MyTask
{
public:
    void process()
    {
        srand(clock());
        int number = rand() % 100;
        cout << "bo_threadpool.number = " << number << endl;
    }

};

void run()
{
    srand(clock());
    int number = rand() % 100;
    cout << "bo_threadpool222.number = " << number << endl;
}

void test()
{
    unique_ptr<MyTask> ptask(new MyTask());
    ThreadPool threadPool(5, 10);

    threadPool.start();

    int cnt = 20;
    while(cnt-- > 0)
    {
        threadPool.addTask(std::bind(&MyTask::process, ptask.get()));
        cout << "cnt = " << cnt << endl;
    }

    threadPool.stop();
}
void test2()
{
    ThreadPool threadPool(5, 10);
    threadPool.start();

    int cnt = 20;
    while(cnt-- > 0)
    {
        threadPool.addTask(run);
        cout << "cnt = " << cnt << endl;
    }

    threadPool.stop();
}
int main(int argc, char **argv)
{
    test2();
    return 0;
}

