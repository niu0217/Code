/* ************************************************************************
> File Name:     virtual_function_test4.cpp
> Author:        niu0217
> Created Time:  Sun 04 Aug 2024 08:14:49 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
using namespace std;

class Test
{
public:
  Test(int age)
    : m_age(age)
  {
  }
  virtual void pureFunc() = 0;
public:
  int m_age;
};

int main()
{
  Test *test;
  cout << endl;
}