/* ************************************************************************
> File Name:     Condition.h
> Author:        niu0217
> Created Time:  Thu 20 Jun 2024 07:34:11 PM CST
> Description:   
 ************************************************************************/

#ifndef BASE_CONDITION_H
#define BASE_CONDITION_H

#include "base/Mutex.h"

#include <pthread.h>

class Condition : noncopyable
{
 public:
  explicit Condition(MutexLock& mutex)
    : mutex_(mutex)
  {
    // 初始化条件变量
    MCHECK(pthread_cond_init(&pcond_, NULL));
  }

  ~Condition()
  {
    // 销毁条件变量
    MCHECK(pthread_cond_destroy(&pcond_));
  }

  void wait()
  {
    MutexLock::UnassignGuard ug(mutex_);
    // 使当前线程释放互斥锁并等待，直到接收到信号或超时。
    MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));
  }

  // returns true if time out, false otherwise.
  bool waitForSeconds(double seconds);

  void notify()
  {
    // 通知一个线程
    MCHECK(pthread_cond_signal(&pcond_));
  }

  void notifyAll()
  {
    // 通知所有线程
    MCHECK(pthread_cond_broadcast(&pcond_));
  }

 private:
  MutexLock& mutex_;  // 占8字节
  pthread_cond_t pcond_;  // 条件变量，配合 mutex_ 使用
};

#endif  // BASE_CONDITION_H
