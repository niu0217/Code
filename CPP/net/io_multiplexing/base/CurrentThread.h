/* ************************************************************************
> File Name:     CurrentThread.h
> Author:        niu0217
> Created Time:  Thu 20 Jun 2024 07:07:01 PM CST
> Description:   
 ************************************************************************/

#ifndef BASE_CURRENTTHREAD_H
#define BASE_CURRENTTHREAD_H

#include "base/Types.h"


namespace CurrentThread
{
  // internal
  extern __thread int t_cachedTid;
  extern __thread char t_tidString[32];
  extern __thread int t_tidStringLength;
  extern __thread const char* t_threadName;
  void cacheTid();

  inline int tid()
  {
    // 编译器可以更好地优化分支预测相关的代码
    if (__builtin_expect(t_cachedTid == 0, 0))
    {
      cacheTid();
    }
    return t_cachedTid;
  }

  inline const char* tidString() // for logging
  {
    return t_tidString;
  }

  inline int tidStringLength() // for logging
  {
    return t_tidStringLength;
  }

  inline const char* name()
  {
    return t_threadName;
  }

  bool isMainThread();

  void sleepUsec(int64_t usec);  // for testing

  string stackTrace(bool demangle);

}  // CurrentThread

#endif  // BASE_CURRENTTHREAD_H