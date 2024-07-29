/* ************************************************************************
> File Name:     virtual_function_test2.cpp
> Author:        niu0217
> Created Time:  Mon 29 Jul 2024 03:52:51 PM CST
> Description:   
 ************************************************************************/

#include<iostream>
using namespace std;

class Base {
public:
    virtual void vtestOne() {
        cout<<"Base::vtestOne()"<<endl;
    }
    virtual void vtestTwo() {
        cout<<"Base::vtestTwo()"<<endl;
    }
    virtual void vtestThree() {
        cout<<"Base::vtestThree()"<<endl;
    }
};

class Derived : public Base {
public:
    void vtestTwo() override {
        cout<<"Derived::vtestTwo()"<<endl;
    }
};

void callVirtualFunction(Base* d) {
    long* pvptr = (long*)d;
    long* vptr = (long*)(*pvptr);

    for(int i = 0; i < 3; i++) {
        printf("vptr[%d] = 0x:%p\n", i, vptr[i]);
    }

    typedef void(*Func)(void);
    Func one = (Func)vptr[0];
    Func two = (Func)vptr[1];
    Func three = (Func)vptr[2];
    one();
    two();
    three();
    cout<<"\n\n";
}

int main()
{
    Base* d = new Base();
    callVirtualFunction(d);

    d = new Derived();
    callVirtualFunction(d);
}