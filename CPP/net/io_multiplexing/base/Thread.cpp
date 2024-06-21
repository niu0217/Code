/* ************************************************************************
> File Name:     Thread.cpp
> Author:        niu0217
> Created Time:  Thu 20 Jun 2024 07:41:22 PM CST
> Description:   
 ************************************************************************/

#include "base/Thread.h"
#include "base/CurrentThread.h"
#include "base/Exception.h"
#include "base/Timestamp.h"

#include <type_traits>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

// 获取当前线程ID
pid_t gettid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

void afterFork()
{
  CurrentThread::t_cachedTid = 0;
  CurrentThread::t_threadName = "main";
  CurrentThread::tid();
  // no need to call pthread_atfork(NULL, NULL, &afterFork);
}

/// pthread_atfork参数解释
///   参数1: 在fork调用之前执行的函数，用于准备线程环境以安全地进行fork，比如锁住某些资源。
///   参数2: 在fork调用之后，在父进程中执行的函数，可以用来恢复父进程中的某些状态或解锁资源。
///   参数3: 在fork调用之后，在子进程中执行的函数，可以用于初始化子进程中的线程环境，比如重新初始化线程库。
class ThreadNameInitializer
{
 public:
  ThreadNameInitializer()
  {
    CurrentThread::t_threadName = "main";
    CurrentThread::tid();
    pthread_atfork(NULL, NULL, &afterFork);
  }
};

// 特别注意：
//   init 是一个全局变量，在执行main函数之前就会被调用！！！
//   也就是当我们启动一个可执行文件时，它会先进行所有全局变量的初始化操作，然后才会执行main函数
//   而在全局变量 init 的初始化操作中，我们就可以将当前线程和进程id缓存起来
ThreadNameInitializer init;

struct ThreadData
{
  typedef Thread::ThreadFunc ThreadFunc;
  ThreadFunc func_;
  string name_;
  pid_t* tid_;
  CountDownLatch* latch_;

  ThreadData(ThreadFunc func,
             const string& name,
             pid_t* tid,
             CountDownLatch* latch)
    : func_(std::move(func)),
      name_(name),
      tid_(tid),
      latch_(latch)
  { }

  void runInThread()
  {
    *tid_ = CurrentThread::tid();
    tid_ = NULL;
    // 线程函数启动了，这时候可以通知主线程
    latch_->countDown();
    latch_ = NULL;

    CurrentThread::t_threadName = name_.empty() ? "muduoThread" : name_.c_str();
    // prctl系统调用：
    //   用于更改当前线程的名字；
    //   这个操作对调试特别有帮助，因为线程名字会出现在诸如top、ps命令的输出中，以及在gdb等调试工具里
    //   使得在多线程程序中更容易识别和监控各个线程的状态。
    ::prctl(PR_SET_NAME, CurrentThread::t_threadName);
    try
    {
      func_();
      CurrentThread::t_threadName = "finished";
    }
    catch (const Exception& ex)
    {
      CurrentThread::t_threadName = "crashed";
      fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
      fprintf(stderr, "reason: %s\n", ex.what());
      fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
      abort();
    }
    catch (const std::exception& ex)
    {
      CurrentThread::t_threadName = "crashed";
      fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
      fprintf(stderr, "reason: %s\n", ex.what());
      abort();
    }
    catch (...)
    {
      CurrentThread::t_threadName = "crashed";
      fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
      throw; // rethrow
    }
  }
};

void* startThread(void* obj)
{
  ThreadData* data = static_cast<ThreadData*>(obj);
  data->runInThread();
  delete data;
  return NULL;
}

void CurrentThread::cacheTid()
{
  if (t_cachedTid == 0)
  {
    t_cachedTid = gettid();
    t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
  }
}

bool CurrentThread::isMainThread()
{
  return tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec)
{
  struct timespec ts = { 0, 0 };
  ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
  ::nanosleep(&ts, NULL);  // 让当前线程睡眠
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(ThreadFunc func, const string& n)
  : started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(0),
    func_(std::move(func)),
    name_(n),
    latch_(1)
{
  setDefaultName();
}

Thread::~Thread()
{
  if (started_ && !joined_)
  {
    // 线程结束，系统自动回收资源，不需要主线程调用pthread_join去回收了
    pthread_detach(pthreadId_);
  }
}

void Thread::setDefaultName()
{
  int num = numCreated_.incrementAndGet();
  if (name_.empty())
  {
    char buf[32];
    snprintf(buf, sizeof buf, "Thread%d", num);
    name_ = buf;
  }
}

void Thread::start()
{
  assert(!started_);
  started_ = true;
  // FIXME: move(func_)
  ThreadData* data = new ThreadData(func_, name_, &tid_, &latch_);
  if (pthread_create(&pthreadId_, NULL, &startThread, data))
  {
    started_ = false;
    delete data; // or no delete?
    // LOG_SYSFATAL << "Failed in pthread_create";
  }
  else
  {
    // 阻塞 等待条件满足 在这里主要是等待线程函数启动
    latch_.wait(); 
    assert(tid_ > 0);
  }
}

int Thread::join()
{
  assert(started_);
  assert(!joined_);
  joined_ = true;
  return pthread_join(pthreadId_, NULL);
}
