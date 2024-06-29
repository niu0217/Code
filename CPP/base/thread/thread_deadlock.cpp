/* ************************************************************************
> File Name:     thread_deadlock.cpp
> Author:        niu0217
> Created Time:  Sat 29 Jun 2024 09:26:20 AM CST
> Description:   
 ************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// 加锁之后忘了解锁，造成死锁
void test1()
{
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);

  for (int i = 0; i < 100; i++)
  {
    pthread_mutex_lock(&mutex);
    // 没有解锁，下一个循环就会造成死锁
  }

  pthread_mutex_destroy(&mutex);
}

// 隐晦的加锁之后忘了解锁情形
void test2()
{
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);

  for (int i = 0; i < 100; i++)
  {
    pthread_mutex_lock(&mutex);
    if (1)
    {
      // 虽然我们在后面进行了解锁，但是在这里函数已经返回了
      // 因此后面的解锁操作没有执行，也会造成死锁
      return;
    }
    pthread_mutex_unlock(&mutex);
  }

  pthread_mutex_destroy(&mutex);
}

// 重复加锁，造成死锁
void test3()
{
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);

  for (int i = 0; i < 100; i++)
  {
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex);  // 重复加锁, 会一直阻塞在这里, 不会执行下面的解锁操作
    pthread_mutex_unlock(&mutex);
  }

  pthread_mutex_destroy(&mutex);
}

// 隐晦的重复加锁
void funcA(pthread_mutex_t &mutex)
{
  for (int i = 0; i < 100; i++)
  {
    pthread_mutex_lock(&mutex);  // 重复加锁  会一直阻塞在这里

    // do something

    pthread_mutex_unlock(&mutex);
  }
}
void funcB(pthread_mutex_t &mutex)
{
  for (int i = 0; i < 100; i++)
  {
    pthread_mutex_lock(&mutex);
    funcA(mutex);  // 在funcA中重复加锁
    pthread_mutex_unlock(&mutex);
  }
}
void test4()
{
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  funcB(mutex);
  pthread_mutex_destroy(&mutex);
}

// 访问多个资源死锁：
//    访问资源x需要加锁 mutexX，访问资源y需要加锁 mutexY
//    线程A先访问资源x，给mutexX加锁；然后线程B访问资源资源y，给mutexY加锁
//    此时，如果线程A还要访问资源y，线程B还要访问资源x，那么就会给相应的mutex加锁
//    这样就会出现死锁。
//    
//    线程A给 mutexX 加了锁, 然后又去给 mutexY 加锁, 因为线程B占着 mutexY, 所以会线程A会阻塞, 等待线程B 释放 mutexY
//    线程B给 mutexY 加了锁, 然后又去给 mutexX 加锁, 因为线程A占着 mutexX, 所以线程B会阻塞, 等待线程A 释放 mutexX
//    而线程A已经被阻塞在加锁 mutexY 这里，只要线程B没有释放，线程A就永远阻塞在这里，也就永远不会释放 mutexX
//    这时就会发生死锁

int main()
{
  // test1();
  // test2();
  // test3();
  test4();
}

// 如何避免死锁？
//   1、避免多次锁定，多检查代码
//   2、对共享资源访问完毕之后，一定要解锁，或者在加锁的时候使用trylock
//   3、如果程序中有多把锁，可以控制对锁的访问顺序（顺序访问共享资源，但在有些情况下是做不到的），另外
//      可以在对其他互斥锁做加锁操作之前，先释放当前线程拥有的互斥锁
//   4、项目程序中引入一些专门用于死锁检测的模块