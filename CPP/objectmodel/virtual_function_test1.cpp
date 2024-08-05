/* ************************************************************************
> File Name:     virtual_function_test1.cpp
> Author:        niu0217
> Created Time:  Mon 29 Jul 2024 03:44:43 PM CST
> Description:   
 ************************************************************************/

#include<iostream>
using namespace std;

class A {
public:
    int m_value;
    A() : m_value{0} { }
    virtual void testVirtualOne() {

    }
};

// 占3个字节，但是编译器决定的
class B
{
public:
  char data1;
  char data2;
  char data3;
};

int main()
{
    A objA;
    int lengthOfObjA = sizeof(objA);
    cout<<lengthOfObjA<<endl;
    cout << alignof(objA) << endl;

    cout << "$$$$$$$$$$$$$$$$$$$\n";

    cout << sizeof(B) << endl;
    B objB;
    cout << sizeof(objB) << endl;
}