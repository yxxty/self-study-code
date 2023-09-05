 ///
 /// @file    TestMylogger.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-04-06 17:31:09
 ///
 
#include "Mylogger.hpp"
#include <iostream>
using std::cout;
using std::endl;
 
void test0() 
{
	Mylogger::getInstance()->warn("this is a warn message");
	Mylogger::getInstance()->error("this is an error message");
	Mylogger::getInstance()->info("this is an  info message");
	Mylogger::getInstance()->debug("this is a debug message");
} 

void test1()
{
	//C++提供了内置宏
	cout << __FILE__ << endl;
	cout << __FUNCTION__ << endl;
	cout << __func__ << endl;
	cout << __LINE__ << endl;
}
//[TestMylogger.cc:test1:25] %m


void test2()
{
	cout << (addPrefix("this is a test")) << endl;
}
 

void test3() 
{
	Mylogger::getInstance()->warn(addPrefix("this is a warn message"));
	Mylogger::getInstance()->error(addPrefix("this is an error message"));
	Mylogger::getInstance()->info(addPrefix("this is an  info message"));
	Mylogger::getInstance()->debug(addPrefix("this is a debug message"));
} 

void test4()
{
	//可以替换printf/cout
	int number = 1;
	LogWarn("this is a warn message, %d", number);
	LogError("this is a error message");
	LogInfo("this is a info message");
	LogDebug("this is a debug message");
}

int main(void)
{
	/* test0(); */
	/* test1(); */
	/* test2(); */
	/* test3(); */
	test4();
	return 0;
}
