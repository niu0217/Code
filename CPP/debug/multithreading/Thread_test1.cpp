/* ************************************************************************
> File Name:     Thread_test1.cpp
> Author:        niu0217
> Created Time:  Fri 28 Jun 2024 05:34:19 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;

void func1()
{
  cout << "func1线程开始\n";
  for (int i = 0; i < 10; i++)
  {
    this_thread::sleep_for(1ms);
    cout << "func1 的第 " << i << " 次执行, 线程id为 " << this_thread::get_id() << endl;
  }
  cout << "func1线程结束\n";
}

void func2()
{
  cout << "func2线程开始\n";
  for (int i = 0; i < 10; i++)
  {
    cout << "func2 的第 " << i << " 次执行, 线程id为 " << this_thread::get_id() << endl;
  }
  cout << "func2线程结束\n";
}

int main()
{
  thread t1(func1);
  thread t2(func2);

  t1.join();
  t2.join();
}