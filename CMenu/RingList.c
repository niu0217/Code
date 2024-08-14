/* ************************************************************************
> File Name:     RingList.c
> Author:        niu0217
> Created Time:  Mon 12 Aug 2024 02:42:16 PM CST
> Description:   
 ************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 44

typedef struct 
{
  char data[MAX_SIZE];  // 存储队列元素
  int front;            // 队首位置
  int rear;             // 队尾位置
}RingQueue;

void initQueue(RingQueue *pRing)
{
  pRing->front = 0;
  pRing->rear = 0;
}

bool isEmpty(RingQueue *pRing)
{
  return pRing->front == pRing->rear;
}

bool isFull(RingQueue *pRing)
{
  return (pRing->rear + 1) % MAX_SIZE == pRing->front;
}

bool push(RingQueue *pRing, char value)
{
  if (isFull(pRing))
  {
    printf("满了，不要再添加元素了.\n");
    return false;
  }
  pRing->data[pRing->rear] = value;
  pRing->rear = (pRing->rear + 1) % MAX_SIZE;
  return true;
}

bool pop(RingQueue *pRing, char *value)
{
  if (isEmpty(pRing))
  {
    printf("空的.\n");
    return false;
  }
  *value = pRing->data[pRing->front];
  pRing->front = (pRing->front + 1) % MAX_SIZE;
  return true;
}

void printRingQueue(RingQueue *pRing)
{
  if (isEmpty(pRing))
  {
    printf("空的.\n");
    return;
  }
  int i = pRing->front;
  while (i != pRing->rear)
  {
    printf("%c ", pRing->data[i]);
    i = (i + 1) % MAX_SIZE;
  }
  printf("\n");
}

int main()
{
  RingQueue ringQueue;
  initQueue(&ringQueue);
  push(&ringQueue, 'A');
  push(&ringQueue, 'B');
  push(&ringQueue, 'C');
  push(&ringQueue, 'D');
  printRingQueue(&ringQueue);

  char value;
  pop(&ringQueue, &value);
  printRingQueue(&ringQueue);
  printf("value = %c\n", value);

  pop(&ringQueue, &value);
  pop(&ringQueue, &value);
  pop(&ringQueue, &value);
  printRingQueue(&ringQueue);

}