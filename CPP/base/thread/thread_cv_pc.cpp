/* ************************************************************************
> File Name:     thread_cv_pc.cpp
> Author:        niu0217
> Created Time:  Sat 29 Jun 2024 11:10:14 AM CST
> Description:   使用条件变量解决生产者消费者问题
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_cond_t cond;
pthread_mutex_t mutex;

struct Node
{
  int number;
  struct Node *next;
};
Node *head = nullptr;  // 链表的头指针

void* producer(void *arg)
{
  while (1)
  {
    pthread_mutex_lock(&mutex);
    // 头插法
    Node *newNode = new Node();
    newNode->number = rand() % 1000;
    newNode->next = head;
    head = newNode;
    printf("Procuder, id = %ld, number = %d\n", pthread_self(), newNode->number);
    pthread_mutex_unlock(&mutex);
    pthread_cond_broadcast(&cond);  // 通知消费者有数据可读
    sleep(rand() % 3);
  }
  return nullptr;
}

void* consumer(void *arg)
{
  while (1)
  {
    pthread_mutex_lock(&mutex);
    while (nullptr == head)
    {
      // 只要head是空的，那么就会一直等待不空条件的到来
      pthread_cond_wait(&cond, &mutex);
    }
    Node *curNode = head;
    printf("Consumer, id = %ld, number = %d\n", pthread_self(), curNode->number);
    head = head->next;
    curNode->next = nullptr;
    delete curNode;
    pthread_mutex_unlock(&mutex);
    sleep(rand() % 3);
  }
  return nullptr;
}

int main()
{
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  pthread_t t1[5];
  pthread_t t2[5];
  for (int i = 0; i < 5; i++)
  {
    pthread_create(&t1[i], NULL, producer, NULL);
  }
  for (int i = 0; i < 5; i++)
  {
    pthread_create(&t2[i], NULL, consumer, NULL);
  }

  for (int i = 0; i < 5; i++)
  {
    pthread_join(t1[i], NULL);
  }
  for (int i = 0; i < 5; i++)
  {
    pthread_join(t2[i], NULL);
  }

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  return 0;
}