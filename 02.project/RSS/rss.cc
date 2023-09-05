#include "tinyxml2.h"
#include <iostream>
#include <string>
#include <regex>

using std::cout;
using std::endl;
using std::string;

using namespace tinyxml2;

void test()
{
    XMLDocument doc;
    doc.LoadFile("coolshell.xml");
    if(doc.ErrorID())
    {
        std::cerr << "loadFile fail" << endl;
        return;
    }

    XMLElement *itemNode = doc.FirstChildElement("rss")
        ->FirstChildElement("channel")
        ->FirstChildElement("item");
    /* if(itemNode) */
    while(itemNode)
    {
        string title = itemNode->FirstChildElement("title")->GetText();
        string link=  itemNode->FirstChildElement("link")->GetText();
        string description = itemNode->FirstChildElement("description")->GetText();
        string content = itemNode->FirstChildElement("content:encoded")->GetText();

#if 1
        std::regex reg("<[^>]+>");//通用正则表达式
        description = regex_replace(description, reg, "");
        content = regex_replace(content, reg, "");
#endif
    
        cout << "title = " << title << endl;
        cout << "link = " << link << endl;
        cout << "description = " << description << endl;
        cout << "content = " << content << endl;

        itemNode = itemNode->NextSiblingElement("item");

    }
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

