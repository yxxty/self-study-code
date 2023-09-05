 ///
 /// @file    Singleton1.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-04-06 14:52:17
 ///
 
#include <iostream>
using std::cout;
using std::endl;



class Singleton
{
public:
	class AutoRelease
	{
	public:
		AutoRelease(Singleton * p)
		: _p(p) {	}

		~AutoRelease() {	
			if(_p) {
				delete _p;
				_p = nullptr;
			}
		}
		Singleton * _p;
	};
public:
	static Singleton * getInstance()
	{
		if(_pInstance == nullptr) {
			_pInstance = new Singleton();
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

Singleton * Singleton::_pInstance = nullptr;

 
void test0() 
{
	Singleton * ps1 = Singleton::getInstance();
	Singleton * ps2 = Singleton::getInstance();
	printf("ps1:%p\n", ps1);
	printf("ps2:%p\n", ps2);
	Singleton::AutoRelease ar(ps1);

	//如果忘记了执行回收单例对象的操作，就会发生内存泄漏
	//Singleton::destroy();
} 
 
int main(void)
{
	test0();
	return 0;
}
