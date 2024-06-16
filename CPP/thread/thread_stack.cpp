/* ************************************************************************
> File Name:     thread_stack.cpp
> Author:        niu0217
> Created Time:  Sun 16 Jun 2024 03:30:09 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <vector>
#include <thread>
#include <cstring>
using namespace std;

void threadFunc()
{
  int a = 10;
  printf("a = %p\n", &a);
}

int main()
{
  vector<thread> workThreads;

  int aa = 100;
  printf("aa = %p\n", &aa);

  for(int i = 0; i < 4; i++)
  {
    workThreads.push_back(thread(threadFunc));
  }

  for (auto &workThread : workThreads)
  {
    workThread.join();
  }
}