#include <math.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

class Figure
{
public:
    virtual
    void display() const = 0;// const * const this

    virtual 
    double area() const  = 0;
};

class Rectangle
: public Figure
{
public:
    Rectangle(double length = 0, double width = 0)
    : _length(length)
    , _width(width)
    {
        cout << "Rectangle(double  = 0, double = 0)" << endl;
    }

    virtual
    void display() const override
    {
        cout << "Rectangle";
    }

    virtual 
    double area() const override
    {
        return _length * _width;
    }

    ~Rectangle()
    {
        cout << "~Rectangle()" << endl;
    }
private:
    double _length;
    double _width;

};

class Circle
: public Figure
{
public:
    Circle(double radius = 0)
    : _radius(radius)
    {
        cout << "Circle(doubel = 0)" <<endl;
    }
    virtual
    void display() const  override
    {
        cout << "Circle";
    }

    virtual 
    double area() const override
    {
        return 3.14 * _radius * _radius;
    }

    ~Circle()
    {
        cout << "~Circle()" << endl;
    }
private:
    double _radius;

};

class Triangle
: public Figure
{
public:
    Triangle(double a = 0, double b = 0, double  c = 0)
    : _a(a)
    , _b(b)
    , _c(c)
    {
        cout << "Triangle(double = 0, double = 0, double = 0)" << endl;
    }

    virtual
    void display() const override
    {
        cout << "Triangle";

    }

    virtual 
    double area() const override
    {
        //海伦公式
        double tmp = (_a +_b + _c)/2;

        return sqrt(tmp * (tmp - _a) * (tmp - _b) * (tmp - _c));
    }

    ~Triangle()
    {
        cout << "~Triangle()" << endl;
    }

private:
    double _a;
    double _b;
    double _c;

};

void func(const Figure &fig)
{
    fig.display();
    cout << "'s area : " << fig.area() << endl;
}

//静态工厂（简单工厂模式）
//
//存在几个问题
//1、不满足开放闭合原则（扩展性比较差）
//2、违背了单一职责原则
class Factory
{
public:
    static Figure *create(const string &figName)
    {
        if(figName == "rectangle")
        {
            return new Rectangle(10, 20);
        }
        else if(figName == "circle")
        {
            return new Circle(10);
        }
        else if(figName == "triangle")
        {
            return new Triangle(3, 4, 5);
        }
        else if(figName == " ")
        {

        }

        return nullptr;
    }
#if 0
    static Figure *createRectangle()
    {
        //读取配置信息 xml rss
        return new Rectangle(10, 20);
    }
    
    static Figure *createCircle()
    {
        //读取配置信息 xml rss
        return new Circle(10);
    }
    
    static Figure *createTriangle()
    {
        //读取配置信息 xml rss
        return new Triangle(3, 4, 5);
    }
#endif
};

int main(int argc, char **argv)
{
    //读取配置信息 xml rss
    /* Rectangle rectangle(10, 20); */
    /* Circle circle(10); */
    /* Triangle triangle(3, 4, 5); */

    /* Figure *rect = Factory::createRectangle(); */
    /* Figure *circle = Factory::createCircle(); */
    /* Figure *triange = Factory::createTriangle(); */
    Figure *reac = Factory::create("rectangle");
    Figure *circle= Factory::create("circle");
    Figure *triangle = Factory::create("triangle");

    cout << endl;
    func(*reac);
    func(*circle);
    func(*triangle);

    return 0;
}
