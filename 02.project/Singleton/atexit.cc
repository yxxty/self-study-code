 ///
 /// @file    atexit.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-04-06 16:08:22
 ///
 
#include <stdlib.h>

#include <iostream>
using std::cout;
using std::endl;

void display()
{	cout << "display()" << endl;	}
 
void test0() 
{
	cout << "test0() start...." << endl;
	//注册函数会在main函数退出后，进行调用
	//可以注册多次, 注册一次就执行一次
	atexit(display);
	atexit(display);
} 
 
int main(void)
{
	test0();

	cout << "main exit...." << endl;
	return 0;
}
