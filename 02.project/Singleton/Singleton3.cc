#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;


class Singleton
{
public:
	//在多线程环境下，getInstance的实现
	//是一个非线程安全的函数
	//
	//如果两个不同的线程同时调用getInstance函数
	//并且都进入到了if语句之中，那么他们都会创建
	//一个Singleton对象，但只有一个对象的地址
	//会被保留下来,因此有一个对象会发生内存泄漏
	static Singleton * getInstance()
	{
		//加锁, 但会降低程序的执行效率，不推荐使用
		if(_pInstance == nullptr) {
			_pInstance = new Singleton();
			//注册一次即可
			atexit(destroy);
		}
		return _pInstance;
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
};

//懒汉模式(懒加载，延迟加载, 是有可能导致多线程安全问题的)
/* Singleton * Singleton::_pInstance = nullptr; */
//饿汉模式是在进入main函数之前就已经完成了对象的创建
//因此绝对不会创建多个Singleton对象
Singleton * Singleton::_pInstance = getInstance();
 
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
