/* ************************************************************************
> File Name:     thread_count.cpp
> Author:        niu0217
> Created Time:  Fri 28 Jun 2024 10:17:33 PM CST
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

void* funcA_num(void *arg)
{
  for (int i = 0; i < MAX; i++)
  {
    int cur = number;
    cur++;
    usleep(10);  // 10 微秒
    number = cur;
    printf("Thread A, id = %lu, number = %d\n", pthread_self(), number);
  }
  return nullptr;
}

void* funcB_num(void *arg)
{
  for (int i = 0; i < MAX; i++)
  {
    int cur = number;
    cur++;
    number = cur;
    printf("Thread B, id = %lu, number = %d\n", pthread_self(), number);
    usleep(5);
  }
  return nullptr;
}

int main()
{
  pthread_t t1Tid;
  pthread_t t2Tid;

  pthread_create(&t1Tid, NULL, funcA_num, NULL);
  pthread_create(&t2Tid, NULL, funcB_num, NULL);

  pthread_join(t1Tid, NULL);
  pthread_join(t2Tid, NULL);
}