/* ************************************************************************
> File Name:     noncopyable.h
> Author:        niu0217
> Created Time:  Thu 20 Jun 2024 01:00:31 PM CST
> Description:   
 ************************************************************************/

#ifndef BASE_NOCOPYABLE_H
#define BASE_NOCOPYABLE_H

class noncopyable
{
 public:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

#endif  // BASE_NOCOPYABLE_H