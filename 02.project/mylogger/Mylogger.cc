 ///
 /// @file    Mylogger.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-04-06 17:17:56
 ///
 
#include "Mylogger.hpp"
#include <iostream>

#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Priority.hh>

using std::cout;
using std::endl;
 
Mylogger * Mylogger::_pInstance = nullptr;

//引用只能在构造函数的初始化表达式中进行初始化
Mylogger::Mylogger()
: _mycat(Category::getRoot().getInstance("wd"))
{
	auto ptnLayout1 = new PatternLayout();
	ptnLayout1->setConversionPattern("%d %c [%p] %m%n");

	auto ptnLayout2 = new PatternLayout();
	ptnLayout2->setConversionPattern("%d %c [%p] %m%n");

	auto pOsapp = new OstreamAppender("console", &cout);
	auto pFileapp = new FileAppender("fileapp", "wd.log"); 

	pOsapp->setLayout(ptnLayout1);
	pFileapp->setLayout(ptnLayout2);

	_mycat.setPriority(Priority::DEBUG);
	_mycat.addAppender(pOsapp);
	_mycat.addAppender(pFileapp);
	cout << "Mylogger()" << endl;

}

Mylogger::~Mylogger()
{
	Category::shutdown();
	cout << "~Mylogger()" << endl;
}

void Mylogger::warn(const char * msg)
{	_mycat.warn(msg);	}

void Mylogger::error(const char * msg)
{	_mycat.error(msg);	}

void Mylogger::info(const char * msg)
{	_mycat.info(msg);	}

void Mylogger::debug(const char * msg)
{	_mycat.debug(msg);	}
