/* ************************************************************************
> File Name:     double_pointer.c
> Author:        niu0217
> Created Time:  Mon 01 Jul 2024 01:18:57 PM CST
> Description:   
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

void test1()
{
  int size = 10;
  int **arr = (int**)malloc(size * sizeof(int));
  arr[0] = (int*)malloc(sizeof(int));
  *arr[0] = 1;
  printf("&arr = %p\n", &arr);        // 栈 0x7ffd646d1c40
  printf("arr = %p\n", arr);          // 堆 0x5616532282a0
  printf("arr[0] = %p\n", arr[0]);    // 堆 0x5616532282d0
  printf("*arr[0] = %d\n", *arr[0]);  // 1
}

// 无内存泄漏
void test2()
{
  int size = 10;
  int **arr = (int**)malloc(size * sizeof(int));

  arr[0] = (int*)malloc(sizeof(int));
  *arr[0] = 11;
  arr[1] = (int*)malloc(sizeof(int));
  *arr[1] = 22;
  arr[2] = (int*)malloc(sizeof(int));
  *arr[2] = 33;

  printf("&arr = %p\n", &arr);  // 栈
  printf("arr = %p\n\n", arr);  // 堆

  printf("(arr + 0) = %p\n", (arr + 0));  // 堆
  printf("&arr[0] = %p\n\n", &arr[0]);  // 堆

  printf("(arr + 1) = %p\n", (arr + 1));  // 堆
  printf("&arr[1] = %p\n\n", &arr[1]);  // 堆

  printf("(arr + 2) = %p\n", (arr + 2));  // 堆
  printf("&arr[2] = %p\n\n", &arr[2]);  // 堆

  printf("*(arr + 0) = %p\n", *(arr + 0));  // 堆
  printf("arr[0] = %p\n\n", arr[0]);          // 堆

  printf("*(arr + 1) = %p\n", *(arr + 1));  // 堆
  printf("arr[1] = %p\n\n", arr[1]);          // 堆

  printf("*(arr + 2) = %p\n", *(arr + 2));  // 堆
  printf("arr[2] = %p\n\n", arr[2]);          // 堆

  printf("**(arr + 2) = %d\n", **(arr + 2));
  printf("*arr[2] = %d\n\n", *arr[2]);

  free(arr);
  free(arr[0]);
  free(arr[1]);
  free(arr[2]);
}

// arr[0] arr[1] arr[2] 内存泄露
// 一共泄漏 12 字节
void test3()
{
  int size = 10;
  int **arr = (int**)malloc(size * sizeof(int));

  arr[0] = (int*)malloc(sizeof(int));
  *arr[0] = 11;
  arr[1] = (int*)malloc(sizeof(int));
  *arr[1] = 22;
  arr[2] = (int*)malloc(sizeof(int));
  *arr[2] = 33;

  free(arr);
}

// 无内存泄漏
void test4()
{
  int size = 10;
  int **arr = (int**)malloc(size * sizeof(int));

  free(arr);
}

int main()
{
  // test1();
  // test2();
  // test3();
  test4();
}