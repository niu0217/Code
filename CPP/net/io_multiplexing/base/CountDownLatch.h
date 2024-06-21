/* ************************************************************************
> File Name:     CountDownLatch.h
> Author:        niu0217
> Created Time:  Thu 20 Jun 2024 07:37:59 PM CST
> Description:   
 ************************************************************************/

#ifndef BASE_COUNTDOWNLATCH_H
#define BASE_COUNTDOWNLATCH_H

#include "base/Condition.h"
#include "base/Mutex.h"

class CountDownLatch : noncopyable
{
 public:

  explicit CountDownLatch(int count);

  void wait();

  void countDown();

  int getCount() const;

 private:
  mutable MutexLock mutex_;
  Condition condition_ GUARDED_BY(mutex_);
  int count_ GUARDED_BY(mutex_);
};

#endif  // BASE_COUNTDOWNLATCH_H
