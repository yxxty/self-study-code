 ///
 /// @file    log4cpptest1.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-04-02 11:36:11
 ///

//#include " "      // 自定义头文件

#include <stdio.h>  // C的标准头文件

#include <iostream> // C++的标准头文件

#include <log4cpp/Category.hh> //第三方库的头文件
#include <log4cpp/Priority.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>


using std::cout;
using std::endl;
using namespace log4cpp;
 
void test0() 
{
	BasicLayout * playout1 = new BasicLayout();
	OstreamAppender * pos = new OstreamAppender("console", &cout);
	pos->setLayout(playout1);

	BasicLayout * playout2 = new BasicLayout();
	FileAppender * pfileapp = new FileAppender("wdlog", "wd.log");
	pfileapp->setLayout(playout2);

	Category & root = Category::getRoot();
	//设置日志来源的优先级
	root.setPriority(Priority::ERROR);
	//设置日志来源的输出目的地
	root.setAppender(pos);
	root.addAppender(pfileapp);

	//设置每一条日志的优先级
	root.emerg("this is an emerg message");
	root.fatal("this is a fatal message");
	root.alert("this is an alert message");
	root.crit("this is n crit message");
	root.error("this is an error message");
	root.warn("this is an warn message");
	root.notice("this is a notice message");
	root.info("this is an  info message");
	root.debug("this is a debug message");

	//log4cpp日志系统退出
	Category::shutdown();
} 
 
int main(void)
{
	test0();
	return 0;
}
