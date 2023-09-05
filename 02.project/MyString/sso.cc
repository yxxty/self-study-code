 ///
 /// @file    sso.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-04-08 11:09:49
 ///
 
#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
 
void test0() 
{
	int * pint = new int(1);
	cout << "pint:" << pint << endl << endl;

	//在sso的实现中，一个std::string对象的大小占据32个字节的空间
	cout << "sizeof(std::string): " << sizeof(std::string) << endl;

	//当字符串长度小于等于15个字节时，与字符串对象
	//存放在同一空间
	string s1 = "hello,world1111";
	cout << "s1:" << s1 << endl;
	printf("&s1: %p\n", &s1);	//字符串对象s1的首地址
	printf("s1'c_str: %p\n", s1.c_str());//字符串内容的首地址

	//当字符串长度大于15个字节，就存放在堆上
	string s2 = "hello,world11112";
	cout << "s2:" << s2 << endl;
	printf("&s2: %p\n", &s2);	//字符串对象s2的首地址
	printf("s2'c_str: %p\n", s2.c_str());//字符串内容的首地址
	cout << endl;

	//string * pstr = new string("hello,wuhan");

	//对象是在栈上
	string s3 = s1;//执行深拷贝   32个字节的大小, 字符串是一个短字符串
	cout << "s3:" << s3 << endl;
	printf("&s3: %p\n", &s3);	//字符串对象s1的首地址
	printf("s3'c_str: %p\n", s3.c_str());//字符串内容的首地址

	string s4 = s2;//执行深拷贝
	cout << "s4:" << s4 << endl;
	printf("&s4: %p\n", &s4);	//字符串对象s1的首地址
	printf("s4'c_str: %p\n", s4.c_str());//字符串内容的首地址

} 
 
int main(void)
{
	test0();
	return 0;
}
