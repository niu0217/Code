/* ************************************************************************
> File Name:     BoundedBlockingQueue.h
> Author:        niu0217
> Created Time:  Sat 22 Jun 2024 10:17:44 AM CST
> Description:   
 ************************************************************************/

#ifndef BASE_BOUNDEDBLOCKINGQUEUE_H
#define BASE_BOUNDEDBLOCKINGQUEUE_H

#include "base/Condition.h"
#include "base/Mutex.h"

#include <boost/circular_buffer.hpp>
#include <assert.h>

template<typename T>
class BoundedBlockingQueue : noncopyable
{
 public:
  explicit BoundedBlockingQueue(int maxSize)
    : mutex_(),
      notEmpty_(mutex_),
      notFull_(mutex_),
      queue_(maxSize)
  {
  }

  void put(const T& x)
  {
    MutexLockGuard lock(mutex_);
    while (queue_.full())
    {
      notFull_.wait();
    }
    assert(!queue_.full());
    queue_.push_back(x);
    notEmpty_.notify();
  }

  void put(T&& x)
  {
    MutexLockGuard lock(mutex_);
    while (queue_.full())
    {
      notFull_.wait();
    }
    assert(!queue_.full());
    queue_.push_back(std::move(x));
    notEmpty_.notify();
  }

  T take()
  {
    MutexLockGuard lock(mutex_);
    while (queue_.empty())
    {
      notEmpty_.wait();
    }
    assert(!queue_.empty());
    T front(std::move(queue_.front()));
    queue_.pop_front();
    notFull_.notify();
    return front;
  }

  bool empty() const
  {
    MutexLockGuard lock(mutex_);
    return queue_.empty();
  }

  bool full() const
  {
    MutexLockGuard lock(mutex_);
    return queue_.full();
  }

  size_t size() const
  {
    MutexLockGuard lock(mutex_);
    return queue_.size();
  }

  size_t capacity() const
  {
    MutexLockGuard lock(mutex_);
    return queue_.capacity();
  }

 private:
  mutable MutexLock          mutex_;
  Condition                  notEmpty_ GUARDED_BY(mutex_);
  Condition                  notFull_ GUARDED_BY(mutex_);
  boost::circular_buffer<T>  queue_ GUARDED_BY(mutex_);
};

#endif  // BASE_BOUNDEDBLOCKINGQUEUE_H

/*
boost::circular_buffer是Boost库中的一个容器类，它提供了一个固定大小的环形缓冲区。
与常规的线性缓冲区不同，环形缓冲区在达到容量限制时，会覆盖最早的元素，从而形成一个循环
利用的空间。这使得boost::circular_buffer非常适合用于实现“最近使用”的数据缓存、队列
或日志记录等场景，特别是在有限的内存空间内需要持续收集数据时。
*/