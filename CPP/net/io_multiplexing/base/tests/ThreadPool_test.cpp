/* ************************************************************************
> File Name:     ThreadPool_test.cpp
> Author:        niu0217
> Created Time:  Fri 21 Jun 2024 04:18:15 PM CST
> Description:   
 ************************************************************************/

#include "base/ThreadPool.h"
#include "base/CountDownLatch.h"
#include "base/CurrentThread.h"
#include "base/Mutex.h"

#include <stdio.h>
#include <unistd.h>  // usleep

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

vector<string> g_fileContexts;
MutexLock g_mutex;

void writeToFile()
{
  string filename = "OutFile.txt";
  ofstream outFile(filename, std::ios_base::out | std::ios_base::app);
  if(! outFile.is_open())
  {
    cerr << "无法打开文件 " << filename << endl;
    return;
  }
  for(const auto &context : g_fileContexts)
  {
    outFile << context << endl;
  }
  outFile.close();
}

void print()
{
  MutexLockGuard lock(g_mutex);
  string str = "tid=" + to_string(CurrentThread::tid()) + "\n";
  g_fileContexts.push_back(str);
}

void printString(const std::string& str)
{
  MutexLockGuard lock(g_mutex);
  // LOG_INFO << str;
  g_fileContexts.push_back(str);
  usleep(100*1000);  // 睡眠100毫秒
}

void test(int maxSize)
{
  // LOG_WARN << "Test ThreadPool with max queue size = " << maxSize;
  string str = "Test ThreadPool with max queue size = " + to_string(maxSize) + "\n";
  g_fileContexts.push_back(str);
  ThreadPool pool("MainThreadPool");
  pool.setMaxQueueSize(maxSize);
  pool.start(5);

  // LOG_WARN << "Adding";
  g_fileContexts.push_back("Adding\n");
  pool.run(print);
  pool.run(print);
  for (int i = 0; i < 100; ++i)
  {
    char buf[32];
    snprintf(buf, sizeof buf, "task %d\n", i);
    pool.run(std::bind(printString, std::string(buf)));
  }
  // LOG_WARN << "Done";
  g_fileContexts.push_back("Done\n");

  CountDownLatch latch(1);
  pool.run(std::bind(&CountDownLatch::countDown, &latch));
  latch.wait();
  pool.stop();

  writeToFile();
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
  g_fileContexts.push_back("Test ThreadPool by stoping early.\n");
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
  g_fileContexts.push_back("stop pool");
  pool.stop();  // early stop

  thread1.join();
  // run() after stop()
  pool.run(print);
  // LOG_WARN << "test2 Done";
  g_fileContexts.push_back("test2 Done");

  writeToFile();
}

int main()
{
  // test(0);
  // test(1);
  // test(5);
  // test(10);
  // test(50);
  test2();
}
