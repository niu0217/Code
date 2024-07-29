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

int main()
{
    A objA;
    int lengthOfObjA = sizeof(objA);
    cout<<lengthOfObjA<<endl;
}