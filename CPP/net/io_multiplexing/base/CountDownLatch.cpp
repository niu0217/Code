/* ************************************************************************
> File Name:     CountDownLatch.cpp
> Author:        niu0217
> Created Time:  Thu 20 Jun 2024 07:38:06 PM CST
> Description:   
 ************************************************************************/

#include "base/CountDownLatch.h"

CountDownLatch::CountDownLatch(int count)
  : mutex_(),
    condition_(mutex_),
    count_(count)
{
}

void CountDownLatch::wait()
{
  MutexLockGuard lock(mutex_);
  while (count_ > 0)
  {
    condition_.wait();
  }
}  // RAII 在这里自动释放 mutex_

void CountDownLatch::countDown()
{
  MutexLockGuard lock(mutex_);
  --count_;
  if (count_ == 0)
  {
    // 唤醒所有等待的线程
    condition_.notifyAll();
  }
}

int CountDownLatch::getCount() const
{
  MutexLockGuard lock(mutex_);
  return count_;
}
