/* ************************************************************************
> File Name:     virtual_function_test3.cpp
> Author:        niu0217
> Created Time:  Mon 29 Jul 2024 03:56:01 PM CST
> Description:   
 ************************************************************************/

#include<iostream>
using namespace std;

class Base1 {
public:
    virtual void f() {
        cout<<"Base1::f()\n";
    }
    virtual void g() {
        cout<<"Base1::g()\n";
    }
};

class Base2 {
public:
    virtual void h() {
        cout<<"Base2::h()\n";
    }
    virtual void k() {
        cout<<"Base2::k()\n";
    }
};

class Derived : public Base1, public Base2 {
public:
    void f() override {
        cout<<"Derived::f()\n";
    }
    void k() override {
        cout<<"Derived::k()\n";
    }
    virtual void vfunc1() {
        cout<<"Derived::vfunc1()\n";
    }
    virtual void vfunc2() {
        cout<<"Derived::vfunc2()\n";
    }
    virtual void vfunc3() {
        cout<<"Derived::vfunc3()\n";
    }
};

int main()
{
    cout<<sizeof(Base1)<<endl;
    cout<<sizeof(Base2)<<endl;
    cout<<sizeof(Derived)<<endl;

    Derived ins;
    Base1& b1 = ins;
    Base2& b2 = ins;
    Derived& d = ins;

    typedef void(*Func)(void);
    //取第一个虚函数表指针
    long* pderived1 = (long*)(&ins);
    long* vptr1 = (long*)(*pderived1);
    //取第二个虚函数表指针
    long* pderived2 = pderived1 + 1;
    long* vptr2 = (long*)(*pderived2);

    Func f0 = (Func)vptr1[0];
    Func f1 = (Func)vptr1[1];
    Func f2 = (Func)vptr1[2];
    Func f3 = (Func)vptr1[3];
    Func f4 = (Func)vptr1[4];
    Func f5 = (Func)vptr1[5];

    Func f00 = (Func)vptr2[0];
    Func f11 = (Func)vptr2[1];

    f0();
    f1();
    f2();
    f3();
    f4();
    f5();

    f00();
    f11();
}