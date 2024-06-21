/* ************************************************************************
> File Name:     Mutex_test.cpp
> Author:        niu0217
> Created Time:  Fri 21 Jun 2024 10:57:50 AM CST
> Description:   
 ************************************************************************/

#include "base/CountDownLatch.h"
#include "base/Mutex.h"
#include "base/Thread.h"
#include "base/Timestamp.h"

#include <vector>
#include <stdio.h>

using namespace std;

MutexLock g_mutex;
vector<int> g_vec;
const int kCount = 10*1000*1000;

void threadFunc()
{
  for (int i = 0; i < kCount; ++i)
  {
    MutexLockGuard lock(g_mutex);
    g_vec.push_back(i);
  }
}

// 告诉编译器，禁止内联 foo 函数
int foo() __attribute__ ((noinline));

int g_count = 0;
int foo()
{
  MutexLockGuard lock(g_mutex);
  if (!g_mutex.isLockedByThisThread())
  {
    printf("FAIL\n");
    return -1;
  }

  ++g_count;
  return 0;
}

int main()
{
  printf("sizeof pthread_mutex_t: %zd\n", sizeof(pthread_mutex_t));  // 40
  printf("sizeof Mutex: %zd\n", sizeof(MutexLock));  // 48
  printf("sizeof pthread_cond_t: %zd\n", sizeof(pthread_cond_t));  // 48
  printf("sizeof Condition: %zd\n", sizeof(Condition));  // 56
  MCHECK(foo());
  if (g_count != 1)
  {
    printf("MCHECK calls twice.\n");
    abort();
  }

  const int kMaxThreads = 8;
  g_vec.reserve(kMaxThreads * kCount);

  Timestamp start(Timestamp::now());
  for (int i = 0; i < kCount; ++i)
  {
    g_vec.push_back(i);
  }

  printf("single thread without lock %f\n", timeDifference(Timestamp::now(), start)); // 0.2s左右

  start = Timestamp::now();
  threadFunc();
  printf("single thread with lock %f\n", timeDifference(Timestamp::now(), start));  // 0.5s左右

  for (int nthreads = 1; nthreads < kMaxThreads; ++nthreads)
  {
    std::vector<std::unique_ptr<Thread>> threads;
    g_vec.clear();
    start = Timestamp::now();
    for (int i = 0; i < nthreads; ++i)
    {
      threads.emplace_back(new Thread(&threadFunc));
      threads.back()->start();
    }
    for (int i = 0; i < nthreads; ++i)
    {
      threads[i]->join();
    }
    printf("%d thread(s) with lock %f\n", nthreads, timeDifference(Timestamp::now(), start));
  }
}

