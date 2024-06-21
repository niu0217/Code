/* ************************************************************************
> File Name:     ThreadPool_test.cpp
> Author:        niu0217
> Created Time:  Fri 21 Jun 2024 04:18:15 PM CST
> Description:   
 ************************************************************************/

#include "base/ThreadPool.h"
#include "base/CountDownLatch.h"
#include "base/CurrentThread.h"

#include <stdio.h>
#include <unistd.h>  // usleep
#include <iostream>
using namespace std;

void print()
{
  printf("tid=%d\n", CurrentThread::tid());
}

void printString(const std::string& str)
{
  // LOG_INFO << str;
  cout << str << endl;
  usleep(100*1000);
}

void test(int maxSize)
{
  // LOG_WARN << "Test ThreadPool with max queue size = " << maxSize;
  ThreadPool pool("MainThreadPool");
  pool.setMaxQueueSize(maxSize);
  pool.start(5);

  // LOG_WARN << "Adding";
  pool.run(print);
  pool.run(print);
  for (int i = 0; i < 100; ++i)
  {
    char buf[32];
    snprintf(buf, sizeof buf, "task %d", i);
    pool.run(std::bind(printString, std::string(buf)));
  }
  // LOG_WARN << "Done";

  CountDownLatch latch(1);
  pool.run(std::bind(&CountDownLatch::countDown, &latch));
  latch.wait();
  pool.stop();
}

/*
 * Wish we could do this in the future.
void testMove()
{
  ThreadPool pool;
  pool.start(2);

  std::unique_ptr<int> x(new int(42));
  pool.run([y = std::move(x)]{ printf("%d: %d\n", CurrentThread::tid(), *y); });
  pool.stop();
}
*/

void longTask(int num)
{
  // LOG_INFO << "longTask " << num;
  CurrentThread::sleepUsec(3000000);
}

void test2()
{
  // LOG_WARN << "Test ThreadPool by stoping early.";
  ThreadPool pool("ThreadPool");
  pool.setMaxQueueSize(5);
  pool.start(3);

  Thread thread1([&pool]()
  {
    for (int i = 0; i < 20; ++i)
    {
      pool.run(std::bind(longTask, i));
    }
  }, "thread1");
  thread1.start();

  CurrentThread::sleepUsec(5000000);
  // LOG_WARN << "stop pool";
  pool.stop();  // early stop

  thread1.join();
  // run() after stop()
  pool.run(print);
  // LOG_WARN << "test2 Done";
}

int main()
{
  test(0);
  test(1);
  test(5);
  test(10);
  test(50);
  test2();
}
