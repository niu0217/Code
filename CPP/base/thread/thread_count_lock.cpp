/* ************************************************************************
> File Name:     thread_count_lock.cpp
> Author:        niu0217
> Created Time:  Sat 29 Jun 2024 09:11:16 AM CST
> Description:   
 ************************************************************************/

#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

#define MAX 50

int number;
pthread_mutex_t mutex;

void* funcACount(void *arg)
{
  for (int i = 0; i < MAX; i++)
  {
    pthread_mutex_lock(&mutex);
    int cur = number;
    cur++;
    usleep(10);  // 10 微秒
    number = cur;
    printf("Thread A, id = %lu, number = %d\n", pthread_self(), number);
    pthread_mutex_unlock(&mutex);
  }
  return nullptr;
}

void* funcBCount(void *arg)
{
  for (int i = 0; i < MAX; i++)
  {
    pthread_mutex_lock(&mutex);
    int cur = number;
    cur++;
    number = cur;
    printf("Thread B, id = %lu, number = %d\n", pthread_self(), number);
    pthread_mutex_unlock(&mutex);
    usleep(5);
  }
  return nullptr;
}

int main()
{
  pthread_t t1Tid;
  pthread_t t2Tid;

  pthread_mutex_init(&mutex, NULL);
  pthread_create(&t1Tid, NULL, funcACount, NULL);
  pthread_create(&t2Tid, NULL, funcBCount, NULL);

  pthread_join(t1Tid, NULL);
  pthread_join(t2Tid, NULL);
  pthread_mutex_destroy(&mutex);
}