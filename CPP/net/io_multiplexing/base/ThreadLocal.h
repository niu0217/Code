/* ************************************************************************
> File Name:     ThreadLocal.h
> Author:        niu0217
> Created Time:  Fri 21 Jun 2024 02:29:52 PM CST
> Description:   
 ************************************************************************/

/*
在多线程编程中，线程特钥提供了一种机制，允许为每个线程关联独立的数据。这意味着，
即使多个线程访问的是同一个全局变量（特钥），每个线程看到的都是与该线程相关联的唯一数据副本。
*/

#ifndef BASE_THREADLOCAL_H
#define BASE_THREADLOCAL_H

#include "base/Mutex.h"
#include "base/noncopyable.h"

#include <pthread.h>

template<typename T>
class ThreadLocal : noncopyable
{
 public:
  ThreadLocal()
  {
    // 创建线程特钥
    MCHECK(pthread_key_create(&pkey_, &ThreadLocal::destructor));
  }

  ~ThreadLocal()
  {
    // 删除 特钥，必须保证所有使用 特钥 的线程都已终止
    MCHECK(pthread_key_delete(pkey_));
  }

  T& value()
  {
    // 根据特钥获取与其关联的数据
    T* perThreadValue = static_cast<T*>(pthread_getspecific(pkey_));
    if (!perThreadValue)
    {
      T* newObj = new T();
      // 每个线程使用 pthread_setspecific 函数将特定的数据与 特钥 关联起来
      MCHECK(pthread_setspecific(pkey_, newObj));
      perThreadValue = newObj;
    }
    return *perThreadValue;
  }

 private:

  static void destructor(void *x)
  {
    T* obj = static_cast<T*>(x);
    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
    T_must_be_complete_type dummy; (void) dummy;
    delete obj;
  }

 private:
  pthread_key_t pkey_;  // 线程特钥
};


#endif  // BASE_THREADLOCAL_H
