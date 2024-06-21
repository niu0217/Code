/* ************************************************************************
> File Name:     ThreadPool.cpp
> Author:        niu0217
> Created Time:  Fri 21 Jun 2024 03:38:16 PM CST
> Description:   
 ************************************************************************/

#include "base/ThreadPool.h"

#include "base/Exception.h"

#include <assert.h>
#include <stdio.h>

ThreadPool::ThreadPool(const string& nameArg)
  : mutex_(),
    notEmpty_(mutex_),
    notFull_(mutex_),
    name_(nameArg),
    maxQueueSize_(0),
    running_(false)
{
}

ThreadPool::~ThreadPool()
{
  if (running_)
  {
    stop();
  }
}

void ThreadPool::start(int numThreads)
{
  assert(threads_.empty());
  running_ = true;
  threads_.reserve(numThreads);
  // 启动 numThreads 个线程
  for (int i = 0; i < numThreads; ++i)
  {
    char id[32];
    snprintf(id, sizeof id, "%d", i+1);
    threads_.emplace_back(new Thread(
          std::bind(&ThreadPool::runInThread, this), name_+id));
    threads_[i]->start();
  }
  if (numThreads == 0 && threadInitCallback_)
  {
    threadInitCallback_();
  }
}

void ThreadPool::stop()
{
  {
  MutexLockGuard lock(mutex_);
  running_ = false;
  notEmpty_.notifyAll();
  notFull_.notifyAll();
  }
  for (auto& thr : threads_)
  {
    thr->join();
  }
}

size_t ThreadPool::queueSize() const
{
  MutexLockGuard lock(mutex_);
  return queue_.size();
}

// 生产者：生产任务
void ThreadPool::run(Task task)
{
  if (threads_.empty())
  {
    task();
  }
  else
  {
    MutexLockGuard lock(mutex_);
    while (isFull() && running_)
    {
      // 如果任务队列是满的，则一直等待
      notFull_.wait();
    }
    if (!running_) return;
    assert(!isFull());

    // 将任务放入到任务队列中，然后通知消费者，有任务了
    queue_.push_back(std::move(task));
    notEmpty_.notify();
  }
}

// 消费者：消费队列中的任务
ThreadPool::Task ThreadPool::take()
{
  MutexLockGuard lock(mutex_); // 任务队列需要加锁保护
  // always use a while-loop, due to spurious wakeup
  while (queue_.empty() && running_)
  {
    // 任务队列没有任务，就一直等待，直到消费者通知
    notEmpty_.wait();
  }
  Task task;
  // 取出任务队列队首的任务返回
  if (!queue_.empty())
  {
    task = queue_.front();
    queue_.pop_front();
    if (maxQueueSize_ > 0)
    {
      notFull_.notify();
    }
  }
  return task;
}

bool ThreadPool::isFull() const
{
  mutex_.assertLocked();
  return maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_;
}

// 每个线程执行的函数
void ThreadPool::runInThread()
{
  try
  {
    if (threadInitCallback_)
    {
      threadInitCallback_();
    }
    // 从任务队列中取任务，然后执行
    while (running_)
    {
      Task task(take());  // 获取任务
      if (task)
      {
        task();
      }
    }
  }
  catch (const Exception& ex)
  {
    fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
    fprintf(stderr, "reason: %s\n", ex.what());
    fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
    abort();
  }
  catch (const std::exception& ex)
  {
    fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
    fprintf(stderr, "reason: %s\n", ex.what());
    abort();
  }
  catch (...)
  {
    fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name_.c_str());
    throw; // rethrow
  }
}

