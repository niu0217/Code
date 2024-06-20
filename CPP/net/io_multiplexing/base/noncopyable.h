/* ************************************************************************
> File Name:     noncopyable.h
> Author:        niu0217
> Created Time:  Thu 20 Jun 2024 01:00:31 PM CST
> Description:   
 ************************************************************************/

#ifndef BASE_NOCOPYABLE_H
#define BASE_NOCOPYABLE_H

///
/// 是一个工具类，用于防止其他类意外地继承并启用复制操作，确保了某些对象的状态是
/// 不可复制的，并保护它们不受拷贝操作的破坏
///
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