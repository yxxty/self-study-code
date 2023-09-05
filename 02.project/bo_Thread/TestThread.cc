#include "Thread.h"
#include <unistd.h>
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;

class MyTask
{
public:
    void process(int x) 
    {
        while(1)
        {
            cout << "The thread mytask is running!" << endl;
            sleep(1);
        }
    }
};

void func()
{
    while(1)
    {
        cout << "The thread myfunc is running!" << endl;
        sleep(1);
    }

}

void test()
{
    /* MyTask task; */
    /* unique_ptr<Thread> pthread(new Thread(std::bind(&MyTask::process, task, 1))); */
    unique_ptr<Thread> pthread(new Thread(std::bind(func)));
    pthread->start();
    pthread->join();

}
int main(int argc, char **argv)
{
    test();
    return 0;
}

