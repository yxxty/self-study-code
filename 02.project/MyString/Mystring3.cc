 ///
 /// @file    Mystring.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-03-31 17:39:27
 ///

#include <string.h>

#include <iostream>
using std::cout;
using std::endl;
using std::ostream;


// _pstr -->  4个字节ref | 字符串的内容
class String
{
	//CharProxy用来解决下标访问运算符的问题
	//所以将其设计成一个内部类
	class CharProxy //代理类 ==》 代理模式 ==> 引入了一个间接层
	{
	public:
		CharProxy(String & self, int idx)
		: _self(self)
		, _idx(idx)
		{}
	
		char & operator=(char ch);

		operator char() {	
			//cout << "operator char()" << endl;
			return _self._pstr[_idx];	
		}

	private:
		String & _self;
		int _idx;
	};
		
public:
	String()
	: _pstr(new char[1 + 4]() + 4)
	{
		//初始化引用计数
		initRefcount();
		cout << "String()" << endl;
	}

	String(const char *pstr)
	: _pstr(new char[strlen(pstr) + 1 + 4]() + 4)
	{
		cout << "String(const char*)" << endl;
		initRefcount();
		strcpy(_pstr, pstr);
	}


	String(const String & rhs)
	: _pstr(rhs._pstr)
	{
		cout << "String(const String &)" << endl;
		increaseRefcount();
	}

	String & operator=(const String & rhs)
	{//四部曲
		if(this != & rhs) {
			release();//先释放左操作数的空间

			_pstr = rhs._pstr;// 浅拷贝
			increaseRefcount();// 再将引用计数加1
		}
		return *this;
	}

	int size() const {	return strlen(_pstr);}


	CharProxy operator[](int idx)
	{
		return CharProxy(*this, idx);
	}

//问题： 下标访问运算符的实现表示只有引用计数大于1，
//就会进行深拷贝
//即: 下标访问运算符无法区分出是读操作还是写操作

#if 0
	char & operator[](int idx)
	{
		if(idx >=0 && idx < size()) {
			if(getRefcount() > 1) {
				decreaseRefcount();
				//进行深拷贝
				char * ptmp = new char[size() + 1 + 4]() + 4;
				strcpy(ptmp, _pstr);
				_pstr = ptmp;
				initRefcount();
			}
			return _pstr[idx];
		} else {
			static char nullchar = '\0';
			return nullchar;
		}
	}
#endif

	const char & operator[](int idx) const
	{
		cout << "const char & operator[](int) const" << endl;
		if(idx >=0 && idx < size()) {
			return _pstr[idx];
		} else {
			static char nullchar = '\0';
			return nullchar;
		}
	}

	void release()
	{
		//先将引用计数减1， 
		//当引用计数为0时，才真正释放对象
		decreaseRefcount();
		if(getRefcount() == 0) {
			delete [] (_pstr - 4);
			_pstr = nullptr;
			cout << ">>> delete _pstr" << endl;
		}
	}

	~String() {
		release();
		cout << "~String()" << endl;
	}

	int getRefcount() const {	return *(int*)(_pstr - 4);	}

	friend std::ostream & operator<<(std::ostream &os, const String &);
private:
	//inline函数
	void initRefcount()
	{	*(int*)(_pstr - 4) = 1;	}

	void increaseRefcount() //引用计数加1
	{	++*(int*)(_pstr - 4);	}

	void decreaseRefcount() //引用计数减1
	{	--*(int*)(_pstr - 4);	}

private:
	//_pstr所指向的空间的首地址是字符串的第一个字符
	char * _pstr;
};
 
std::ostream & operator<<(std::ostream &os, const String & rhs)
{
	os << rhs._pstr;
	return os;
}

//该函数用来做写操作
char & String::CharProxy::operator=(char ch)
{
	if(this->_idx >=0 && this->_idx < _self.size()) {
		if(_self.getRefcount() > 1) {
			_self.decreaseRefcount();
			//进行深拷贝
			char * ptmp = new char[_self.size() + 1 + 4]() + 4;
			strcpy(ptmp, _self._pstr);
			_self._pstr = ptmp;
			_self.initRefcount();
		}
		_self._pstr[_idx] = ch;//执行写操作
		return _self._pstr[_idx];
	} else {
		static char nullchar = '\0';
		return nullchar;
	}
}

void test0() 
{
	//String s1("hello");
	//String s2("hello");


	String str1;
	cout << "str1:" << str1 << endl;
	cout << "str1's ref:" << str1.getRefcount() << endl;

	String str2 = str1;
	cout << "str2:" << str2 << endl;
	cout << "str2's ref:" << str2.getRefcount() << endl;


	String str3 = "Hello,world";
	cout << "str3: " << str3 << endl;
	String str4 = str3; 
	cout << "str4: " << str4 << endl;
	cout << "str3's ref:" << str3.getRefcount() << endl;
	cout << "str4's ref:" << str4.getRefcount() << endl;
	cout << endl;

	String str5("wangdao");
	cout << "str5:" << str5 << endl;
	cout << "str5's ref:" << str5.getRefcount() << endl;

	String str6 = str5;
	cout << "str6:" << str6 << endl;
	cout << "str6's ref:" << str6.getRefcount() << endl;
	cout << "str5's ref:" << str5.getRefcount() << endl;

	str5 = str4;//
	cout << "执行赋值操作之后:" << endl;
	cout << "str5:" << str5 << endl;
	cout << "str3's ref:" << str3.getRefcount() << endl;
	cout << "str4's ref:" << str4.getRefcount() << endl;
	cout << "str5's ref:" << str5.getRefcount() << endl;

	cout << "\n 执行读操作:" << endl;

	cout << "str5[0]:" << str5[0] << endl;//CharProxy = > char

	cout << "str4: " << str4 << endl;
	cout << "str3: " << str3 << endl;
	cout << "str3's ref:" << str3.getRefcount() << endl;
	cout << "str4's ref:" << str4.getRefcount() << endl;
	cout << "str5:" << str5 << endl;
	cout << "str5's ref:" << str5.getRefcount() << endl;

#if 1
	cout << "\n 执行写操作" << endl;
	//执行写操作
	cout << (str5[0] = 'X') << endl;
	cout << "str4: " << str4 << endl;
	cout << "str3: " << str3 << endl;
	cout << "str3's ref:" << str3.getRefcount() << endl;
	cout << "str4's ref:" << str4.getRefcount() << endl;
	cout << "str5:" << str5 << endl;
	cout << "str5's ref:" << str5.getRefcount() << endl;
#endif
} 
 
int main(void)
{
	test0();
	return 0;
}
