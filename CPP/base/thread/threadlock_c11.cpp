/* ************************************************************************
> File Name:     threadlock_c11.cpp
> Author:        niu0217
> Created Time:  Sat 06 Jul 2024 08:55:02 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
using namespace std;

namespace TESTCASE1
{
mutex mtx;
int count = 0;
void threadFunc()
{
  lock_guard<mutex> lock(mtx);
  int temp = count;
  for (int i = 0; i < 100; i++)
  {
    usleep(10);
  }
  count = temp + 1;
}
void test()
{
  thread t1(threadFunc);
  thread t2(threadFunc);

  t1.join();
  t2.join();
  cout << count << endl;
}
}  // TESTCASE1



namespace TESTCASE2
{
mutex mtx;
int count = 0;
void threadFunc()
{
  // defer_lock: 创建unique_lock对象，但是延迟加锁
  unique_lock<mutex> lock(mtx, defer_lock);

  sleep(1);
  
  lock.lock();
  int temp = count;
  for (int i = 0; i < 100; i++)
  {
    usleep(10);
    temp++;
  }
  count = temp + 1;
  lock.unlock();
}
void test()
{
  thread t1(threadFunc);
  thread t2(threadFunc);

  t1.join();
  t2.join();
  cout << count << endl;
}
}  // TESTCASE2



namespace TESTCASE3
{
mutex mtx1;
mutex mtx2;
int count1 = 0;
int count2 = 0;
void threadFunc1()
{
  while (true)
  {
    if (std::try_lock(mtx1, mtx2) == -1)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 避免忙等
      continue;
    }
    count1++;
    count2++;
    mtx1.unlock();
    mtx2.unlock();
    break;
  }
}
void threadFunc2()
{
  while (true)
  {
    if (std::try_lock(mtx2, mtx1) == -1)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 避免忙等
      continue;
    }
    sleep(1);
    count2++;
    sleep(1);
    count1++;
    sleep(1);
    mtx2.unlock();
    mtx1.unlock();
    break;
  }
}
void test()
{
  std::thread t1(threadFunc1);
  std::thread t2(threadFunc2);
  t1.join();
  t2.join();
  std::cout << "Count 1: " << count1 << std::endl;
  std::cout << "Count 2: " << count2 << std::endl;
}
}  // TESTCASE3


namespace TESTCASE4
{
mutex mtx1;
mutex mtx2;
int count1 = 0;
int count2 = 0;
void threadFunc1()
{
  while (true)
  {
    mtx1.lock();
    count1++;
    sleep(1);
    mtx2.lock();
    count2++;
    mtx2.unlock();
    mtx1.unlock();
    break;
  }
}
void threadFunc2()
{
  while (true)
  {
    mtx2.lock();
    count2++;
    mtx1.lock();
    count1++;
    mtx1.unlock();
    mtx2.unlock();
    break;
  }
}
void test()
{
  std::thread t1(threadFunc1);
  std::thread t2(threadFunc2);
  t1.join();
  t2.join();
  std::cout << "Count 1: " << count1 << std::endl;
  std::cout << "Count 2: " << count2 << std::endl;
}
}  // TESTCASE4

int main()
{
  // TESTCASE1::test();
  // TESTCASE2::test();
  TESTCASE3::test();
  // TESTCASE4::test();
}