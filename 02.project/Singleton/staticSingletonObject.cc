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
	static Singleton * getInstance()
	{
		//局部静态对象只有第一次调用才会进行初始化
		//static int a = 0;
		static Singleton s;//存储在全局静态区
		return &s;
	}

private:
	Singleton() {	cout << "Singleton()" << endl;	}
	~Singleton() {	cout << "~Singleton()" << endl;	}

	int _data;
	//如果数据成员很大，会占用全局静态区太多的空间
	//static Singleton * _pInstance;
};

 
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
