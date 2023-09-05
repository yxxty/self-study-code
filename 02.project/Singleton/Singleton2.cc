 ///
 /// @file    Singleton1.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-04-06 14:52:17
 ///
 
#include <iostream>
using std::cout;
using std::endl;

//单例对象的自动释放方法一：
//嵌套类 + 静态对象

//外部类
class Singleton
{
public:
	class AutoRelease//内部类
	{	//AutoRelease专为Singleton服务
	public:
		//在内部类内部无法直接拿到外部类的
		//非静态的数据成员的
		AutoRelease()
		{	
			//cout << _data << endl;//error
			cout << "AutoRelease()" << endl;
		}

		//在AutoRelease内部是可以直接拿到外部类的
		//静态数据成员_pInstance
		~AutoRelease() {	
			if(_pInstance) {
				delete _pInstance;
				_pInstance = nullptr;
			}
			cout << "~AutoRelease()" << endl;
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
	Singleton()
	{	cout << "Singleton()" << endl;	}
	~Singleton() {	cout << "~Singleton()" << endl;	}

	int _data;
	static AutoRelease _ar;//类对象成员
	static Singleton * _pInstance;
};

Singleton * Singleton::_pInstance = nullptr;
//静态数据成员要在类之外初始化
Singleton::AutoRelease Singleton::_ar;

 
void test0() 
{
	//在getInstance之中创建AutoRelease对象
	Singleton * ps1 = Singleton::getInstance();
	Singleton * ps2 = Singleton::getInstance();
	printf("ps1:%p\n", ps1);
	printf("ps2:%p\n", ps2);
	//需求: 该语句不需要我们手动执行
	//Singleton::AutoRelease ar(ps1);

	//如果忘记了执行回收单例对象的操作，就会发生内存泄漏
	//Singleton::destroy();
} 
 
int main(void)
{
	test0();
	return 0;
}
