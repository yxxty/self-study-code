#include <iostream>
#include <list>
#include <algorithm>
#include <string>
#include <memory>

using std::cout;
using std::endl;
using std::list;
using std::find;
using std::string;
using std::unique_ptr;

class Observer;

class Subject
{
public:
    virtual void attach(Observer *pObserver) = 0;
    virtual void detach(Observer *pObserver) = 0;
    virtual void notify() = 0;

    virtual ~Subject() {}
};

class ConcreteSubject
: public Subject
{
public:
    void attach(Observer *pObserver) override;
    void detach(Observer *pObserver) override; 
    void notify() override;

    void setStatus(int status)
    {
        _status = status;
    }

    int getStatus() const
    {
        return _status;
    }

private:
    list<Observer *> _obList;
    int _status;
};

class Observer
{
public:
    virtual void update(int) = 0;
    virtual ~Observer() { }
};

class ConcreteObserverA
: public Observer
{
public:
    ConcreteObserverA(const string &name)
    : _name(name)
    {

    }
    void update(int value) 
    {
        cout << "ConcreteObserverA " << _name << ", value = " << value << endl;
    }

private:
    string _name;

};

class ConcreteObserverB
: public Observer
{
public:
    ConcreteObserverB(const string &name)
    : _name(name)
    {

    }
    void update(int value) 
    {
        cout << "ConcreteObserverB " << _name << ", value = " << value << endl;
    }

private:
    string _name;

};

void ConcreteSubject::attach(Observer *pObserver) 
{
    if(pObserver)
    {
        _obList.push_back(pObserver);
    }
}

void ConcreteSubject::detach(Observer *pObserver) 
{
    for(auto it = _obList.begin(); it != _obList.end(); ++it)
    {
        if(*it == pObserver)
        {
            _obList.remove(pObserver);
            break;
        }
    }
}

void ConcreteSubject::notify() 
{
    for(auto &ob : _obList)
    {
        ob->update(_status);
    }
}

int main(int argc, char **argv)
{
    unique_ptr<ConcreteSubject> pSubject(new ConcreteSubject());
    unique_ptr<Observer> pObserverA(new ConcreteObserverA("lili"));
    unique_ptr<Observer> pObserverB(new ConcreteObserverB("lucy"));

    pSubject->setStatus(2);

    pSubject->attach(pObserverA.get());
    pSubject->attach(pObserverB.get());

    pSubject->notify();

    cout << endl;
    pSubject->detach(pObserverB.get());
    pSubject->setStatus(3);
    pSubject->notify();

    return 0;
}
