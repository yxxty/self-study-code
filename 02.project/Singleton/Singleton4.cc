 ///
 /// @file    Singleton1.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-04-06 14:52:17
 ///
 

#include <stdlib.h>
#include <pthread.h>

#include <iostream>
using std::cout;
using std::endl;


class Singleton
{
public:
	static Singleton * getInstance()
	{	//多线程安全的实现，但只能在Linux平台使用
		//pthread_once会保证init函数只会被执行一次
		pthread_once(&_once, init);
		return _pInstance;
	}

	static void init()
	{
		atexit(destroy);	
		_pInstance = new Singleton();
	}

	static void destroy()
	{
		if(_pInstance) {
			delete _pInstance;
			_pInstance = nullptr;
		}
	}

private:
	Singleton() {	cout << "Singleton()" << endl;	}
	~Singleton() {	cout << "~Singleton()" << endl;	}

	int _data;
	static Singleton * _pInstance;
	static pthread_once_t _once;
};

Singleton * Singleton::_pInstance = nullptr;
pthread_once_t Singleton::_once = PTHREAD_ONCE_INIT;
 
void test0() 
{
	Singleton * ps1 = Singleton::getInstance();
	Singleton * ps2 = Singleton::getInstance();
	printf("ps1:%p\n", ps1);
	printf("ps2:%p\n", ps2);
} 
 
int main(void)
{
	test0();
	return 0;
}
