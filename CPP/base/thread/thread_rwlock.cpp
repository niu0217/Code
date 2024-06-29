/* ************************************************************************
> File Name:     thread_rwlock.cpp
> Author:        niu0217
> Created Time:  Sat 29 Jun 2024 10:20:58 AM CST
> Description:   8个线程操作同一个全局变量，3个线程不定时写同一个全局资源，5个线程不定时读同一全局资源
 ************************************************************************/

// 读写锁的特点
//   1、使用读写锁的读锁锁定了临界区，线程对临界区的访问是 并行 的，读锁是共享的；
//   2、使用读写锁的写锁锁定了临界区，线程对临界区的访问是 串行 的，写锁是独占的；
//   3、使用读写锁分别对两个临界区加了读锁和写锁，两个线程要同时访问这两个临界区，访问写锁临界区的线程
//      继续运行，访问读锁临界区的线程阻塞，因为写锁比读锁的优先级高

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define MAX 50

int number = 0;

pthread_rwlock_t rwlock;

void* readNum(void *arg)
{
  for (int i = 0; i < MAX; i++)
  {
    pthread_rwlock_rdlock(&rwlock);  // 加读锁
    printf("Thread A read, id = %lu, number = %d\n", pthread_self(), number);
    pthread_rwlock_unlock(&rwlock);
    usleep(rand() % 5);
  }
  return nullptr;
}

void* writeNum(void *arg)
{
  for (int i = 0; i < MAX; i++)
  {
    pthread_rwlock_wrlock(&rwlock);  // 加写锁
    int cur = number;
    cur++;
    number = cur;
    printf("Thread B write, id = %lu, number = %d\n", pthread_self(), number);
    pthread_rwlock_unlock(&rwlock);
    usleep(5);
  }
  return nullptr;
}

int main()
{
  pthread_t readTid[5];
  pthread_t writeTid[3];

  pthread_rwlock_init(&rwlock, NULL);

  for (int i = 0; i < 5; i++)
  {
    pthread_create(&readTid[i], NULL, readNum, NULL);
  }
  for (int i = 0; i < 3; i++)
  {
    pthread_create(&writeTid[i], NULL, writeNum, NULL);
  }
  
  for (int i = 0; i < 5; i++)
  {
    pthread_join(readTid[i], NULL);
  }
  for (int i = 0; i < 3; i++)
  {
    pthread_join(writeTid[i], NULL);
  }

  pthread_rwlock_destroy(&rwlock);
}