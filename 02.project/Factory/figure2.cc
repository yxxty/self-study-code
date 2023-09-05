#include <math.h>
#include <iostream>
#include <string>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;


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
    virtual Figure *create() = 0;
    virtual ~Factory() {  }
};

class RectangleFactory
: public Factory
{
public:
    Figure *create()
   {
       return new Rectangle(10, 20);
   }
};

class CircleFactory
: public Factory
{
public:
    Figure *create()
   {
       return new Circle(10);
   }

};

class TriangleFactory
: public Factory
{
public:
    Figure *create()
   {
       return new Triangle(3, 4, 5);
   }

};

int main(int argc, char **argv)
{
    unique_ptr<Factory> reacFact(new RectangleFactory());
	unique_ptr<Figure> rect(reacFact->create());

	unique_ptr<Factory> circleFact(new CircleFactory());
	unique_ptr<Figure> circle(circleFact->create());

	unique_ptr<Factory> triangleFact(new TriangleFactory());
	unique_ptr<Figure> triangle(triangleFact->create());

	func(*rect.get());
	func(*circle.get());
	func(*triangle.get());
    return 0;
}
