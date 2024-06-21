/* ************************************************************************
> File Name:     Exception.cpp
> Author:        niu0217
> Created Time:  Thu 20 Jun 2024 07:46:00 PM CST
> Description:   
 ************************************************************************/

#include "base/Exception.h"
#include "base/CurrentThread.h"

Exception::Exception(string msg)
  : message_(std::move(msg)),
    stack_(CurrentThread::stackTrace(/*demangle=*/false))
{
}
