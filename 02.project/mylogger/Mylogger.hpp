 ///
 /// @file    Mylogger.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-04-06 17:14:56
 ///
 
#ifndef __WD_MYLOGGER_HPP
#define __WD_MYLOGGER_HPP

#include <log4cpp/Category.hh>
using namespace log4cpp;

class Mylogger
{
public:
	static Mylogger * getInstance()
	{
		if(nullptr == _pInstance) {
			_pInstance = new Mylogger();
		}
		return _pInstance;
	}

	void warn(const char * msg);
	void error(const char * msg);
	void info(const char * msg);
	void debug(const char * msg);

private:
	Mylogger();
	~Mylogger();

private:
	static Mylogger * _pInstance;
	Category & _mycat;
	//PatternLayout * ptnLayout1;
};

//如果初始化放在头文件中，
//当不同的*.cc文件都include头文件时
//会造成多次重复定义的问题,
//因此需要将其初始化放入实现文件中
//Mylogger * Mylogger::_pInstance = nullptr;
 
#define addPrefix(msg)	std::string().append("[")\
			.append(__FILE__).append(":")\
			.append(__FUNCTION__).append(":")\
			.append(std::to_string(__LINE__)).append("] ")\
			.append(msg).c_str()


#define LogWarn(msg) Mylogger::getInstance()->warn(addPrefix(msg))
#define LogError(msg) Mylogger::getInstance()->error(addPrefix(msg))
#define LogInfo(msg) Mylogger::getInstance()->info(addPrefix(msg))
#define LogDebug(msg) Mylogger::getInstance()->debug(addPrefix(msg))


#endif
