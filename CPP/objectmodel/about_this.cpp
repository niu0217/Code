/* ************************************************************************
> File Name:     about_this.cpp
> Author:        niu0217
> Created Time:  Mon 29 Jul 2024 04:10:40 PM CST
> Description:   
 ************************************************************************/

#include<iostream>
using namespace std;

class A {
public:
    int a;
    A() {
        printf("A::A()的this指针是：%p\n", this);
    }
    void funcA() {
        printf("A::funcA()的this指针是：%p\n", this);
    }
};

class B {
public:
    int b;
    B() {
        printf("B::B()的this指针是：%p\n", this);
    }
    void funcB() {
        printf("B::funcB()的this指针是：%p\n", this);
    }
};

class C : public A, public B {
public:
    int c;
    C() {
        printf("C::C()的this指针是：%p\n", this);
    }
    void funcC() {
        printf("C::funcC()的this指针是：%p\n", this);
    }
};

int main()
{
    cout<<sizeof(A)<<endl;
    cout<<sizeof(B)<<endl;
    cout<<sizeof(C)<<endl<<endl;

    C objectC;
    objectC.funcA();
    objectC.funcB();
    objectC.funcC();
}