/* ************************************************************************
> File Name:     Exception.h
> Author:        niu0217
> Created Time:  Thu 20 Jun 2024 07:45:50 PM CST
> Description:   
 ************************************************************************/

#ifndef BASE_EXCEPTION_H
#define BASE_EXCEPTION_H

#include "base/Types.h"
#include <exception>

class Exception : public std::exception
{
 public:
  Exception(string what);
  ~Exception() noexcept override = default;

  // default copy-ctor and operator= are okay.

  const char* what() const noexcept override
  {
    return message_.c_str();
  }

  const char* stackTrace() const noexcept
  {
    return stack_.c_str();
  }

 private:
  string message_;
  string stack_;
};

#endif  // BASE_EXCEPTION_H
