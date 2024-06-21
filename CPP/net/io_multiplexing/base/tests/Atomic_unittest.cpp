/* ************************************************************************
> File Name:     Atomic_unittest.cpp
> Author:        niu0217
> Created Time:  Thu 20 Jun 2024 08:22:16 PM CST
> Description:   
 ************************************************************************/

#include "base/Atomic.h"
#include <assert.h>
#include <stdio.h>

int main()
{
  {
  AtomicInt64 a0;  // value_ = 0
  assert(a0.get() == 0);
  assert(a0.getAndAdd(1) == 0);  // value_ = 1
  assert(a0.get() == 1);
  assert(a0.addAndGet(2) == 3);  // value_ = 3
  assert(a0.get() == 3);
  assert(a0.incrementAndGet() == 4); // value_ = 4
  assert(a0.get() == 4);
  a0.increment();  // value_ = 5
  assert(a0.get() == 5);
  assert(a0.addAndGet(-3) == 2);  // value_ = 2
  assert(a0.getAndSet(100) == 2); // value_ = 100
  assert(a0.get() == 100);
  }

  {
  AtomicInt32 a1;
  assert(a1.get() == 0);
  assert(a1.getAndAdd(1) == 0);
  assert(a1.get() == 1);
  assert(a1.addAndGet(2) == 3);
  assert(a1.get() == 3);
  assert(a1.incrementAndGet() == 4);
  assert(a1.get() == 4);
  a1.increment();
  assert(a1.get() == 5);
  assert(a1.addAndGet(-3) == 2);
  assert(a1.getAndSet(100) == 2);
  assert(a1.get() == 100);
  }
  printf("All pased\n");
}

