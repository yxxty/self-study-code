#include "Thread.h"
#include <unistd.h>
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;

class MyThread
: public Thread
{
public:
    void run() override
    {
        while(1)
        {
            cout << "The thread is running!" << endl;
            sleep(1);
        }
    }
};

void test()
{
    /* Thread *pthread = new MyThread(); */
    unique_ptr<Thread> pthread(new MyThread());
    pthread->start();
    pthread->join();
    /* sleep(10); */

}
int main(int argc, char **argv)
{
    test();
    return 0;
}

