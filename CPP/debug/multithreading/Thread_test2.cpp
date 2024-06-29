/* ************************************************************************
> File Name:     Thread_test2.cpp
> Author:        niu0217
> Created Time:  Fri 28 Jun 2024 07:56:36 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;

void foo()
{
  for (int i = 0; i < 100; i++)
  {
    int flag = 100000;
    int k = 0;
    // sleep(10);
    cout << "foo() i: " << i << endl;
  }
  cout << endl;
}

void bar(int x)
{
  for (int j = x; j < 200; j++)
  {
    cout << "bar() j: " << j << endl;
  }
  cout << endl;
}

int main()
{
  thread t1(foo);
  thread t2(bar, 100);

  t1.join();
  t2.join();
}