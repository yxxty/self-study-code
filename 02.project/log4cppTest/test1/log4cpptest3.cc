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
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>


using std::cout;
using std::endl;
using namespace log4cpp;
 
void test0() 
{
	PatternLayout* playout1 = new PatternLayout();
	playout1->setConversionPattern("%d %c [%p] %m%n");

	OstreamAppender * pos = new OstreamAppender("console", &cout);
	pos->setLayout(playout1);

	PatternLayout* playout2 = new PatternLayout();
	playout2->setConversionPattern("%d %c [%p] %m%n");

	FileAppender * pfileapp = new FileAppender("wdlog", "wd.log");
	pfileapp->setLayout(playout2);

	Category & root = Category::getRoot();

	//设置日志来源的优先级
	root.setPriority(Priority::DEBUG);
	//设置日志来源的输出目的地
	root.setAppender(pos);
	root.addAppender(pfileapp);

	//子Category是会继承自根Category的设置
	Category & module1 = root.getInstance("module1");
	Category & module2 = root.getInstance("module2");
	module2.setPriority(Priority::NOTICE);
	//设置每一条日志的优先级
	module1.emerg("this is an emerg message");
	module1.fatal("this is a fatal message");
	module1.alert("this is an alert message");
	module1.crit("this is n crit message");
	module2.error("this is an error message");
	module2.warn("this is an warn message");
	module2.notice("this is a notice message");
	module2.info("this is an  info message");
	module2.debug("this is a debug message");

	//log4cpp日志系统退出
	Category::shutdown();
} 
 
int main(void)
{
	test0();
	return 0;
}
