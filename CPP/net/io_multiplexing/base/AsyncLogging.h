/* ************************************************************************
> File Name:     AsyncLogging.h
> Author:        niu0217
> Created Time:  Sat 22 Jun 2024 04:38:05 PM CST
> Description:   
 ************************************************************************/

#ifndef BASE_ASYNCLOGGING_H
#define BASE_ASYNCLOGGING_H

#include "base/BlockingQueue.h"
#include "base/BoundedBlockingQueue.h"
#include "base/CountDownLatch.h"
#include "base/Mutex.h"
#include "base/Thread.h"
#include "base/LogStream.h"

#include <atomic>
#include <vector>

class AsyncLogging : noncopyable
{
 public:

  AsyncLogging(const string& basename,
               off_t rollSize,
               int flushInterval = 3);

  ~AsyncLogging()
  {
    if (running_)
    {
      stop();
    }
  }

  // 供前端的生产者调用（日志数据写到缓冲区）
  void append(const char* logline, int len);

  void start()
  {
    running_ = true;
    thread_.start();  // 日志线程启动
    latch_.wait(); 
  }

  void stop() NO_THREAD_SAFETY_ANALYSIS
  {
    running_ = false;
    cond_.notify();
    thread_.join();
  }

 private:

  // 供后端消费者线程调用（将数据写入到日志文件）
  void threadFunc();  

  typedef detail::FixedBuffer<detail::kLargeBuffer> Buffer;
  typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
  typedef BufferVector::value_type BufferPtr;

  // 超时时间，在flushInterval_秒内，缓冲区没写满，仍将
  // 缓冲区中的数据写到文件
  const int flushInterval_;
  std::atomic<bool> running_;
  const string basename_;
  const off_t rollSize_;
  Thread thread_;  // 日志线程
  CountDownLatch latch_;  // 用于等待线程启动
  MutexLock mutex_;
  Condition cond_ GUARDED_BY(mutex_);
  BufferPtr currentBuffer_ GUARDED_BY(mutex_);  // 当前缓冲区
  BufferPtr nextBuffer_ GUARDED_BY(mutex_);     // 预备缓冲区
  BufferVector buffers_ GUARDED_BY(mutex_);     // 待写入文件的已填满的缓冲区
};

#endif  // BASE_ASYNCLOGGING_H
