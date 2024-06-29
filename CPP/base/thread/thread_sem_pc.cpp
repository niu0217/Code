/* ************************************************************************
> File Name:     thread_sem_pc.cpp
> Author:        niu0217
> Created Time:  Sat 29 Jun 2024 12:28:59 PM CST
> Description:   
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semp;  // 生产者信号量
sem_t semc;  // 消费者信号量
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
    sem_wait(&semp);
    pthread_mutex_lock(&mutex);  // 不能放在 sem_wait 前面
    // 头插法
    Node *newNode = new Node();
    newNode->number = rand() % 1000;
    newNode->next = head;
    head = newNode;
    printf("Procuder, id = %ld, number = %d\n", pthread_self(), newNode->number);
    pthread_mutex_unlock(&mutex);
    sem_post(&semc);  // 通知消费者去消费
    sleep(rand() % 3);
  }
  return nullptr;
}

void* consumer(void *arg)
{
  while (1)
  {
    sem_wait(&semc);
    pthread_mutex_lock(&mutex);  // 不能放在 sem_wait 前面
    Node *curNode = head;
    printf("Consumer, id = %ld, number = %d\n", pthread_self(), curNode->number);
    head = head->next;
    curNode->next = nullptr;
    delete curNode;
    pthread_mutex_unlock(&mutex);
    sem_post(&semp);
    sleep(rand() % 3);
  }
  return nullptr;
}

int main()
{
  pthread_mutex_init(&mutex, NULL);
  sem_init(&semp, 0, 5);
  sem_init(&semc, 0, 0);

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

  sem_destroy(&semp);
  sem_destroy(&semc);
  pthread_mutex_destroy(&mutex);

  return 0;
}