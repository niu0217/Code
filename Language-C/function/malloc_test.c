/* ************************************************************************
> File Name:     malloc_test.c
> Author:        niu0217
> Created Time:  Tue 02 Jul 2024 01:32:18 PM CST
> Description:   
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

void test1()
{
  int *arr = (int*)malloc(sizeof(int) * 4);
  arr[0] = 11;
  arr[1] = 12;
  arr[2] = 13;
  arr[3] = 14;
  printf("arr[0] = %d\n", arr[0]);
  printf("arr[1] = %d\n", arr[1]);
  printf("arr[2] = %d\n", arr[2]);
  printf("arr[3] = %d\n", arr[3]);
}

void test2()
{
  int *arr = (int*)malloc(4 * sizeof(int*));
  arr[0] = 11;
  arr[1] = 12;
  arr[2] = 13;
  arr[3] = 14;
  printf("arr[0] = %d\n", arr[0]);
  printf("arr[1] = %d\n", arr[1]);
  printf("arr[2] = %d\n", arr[2]);
  printf("arr[3] = %d\n", arr[3]);
}

void test3()
{
  int **arr = (int**)malloc(4 * sizeof(int*));
  arr[0] = (int*)malloc(sizeof(int));
  *arr[0] = 12;
  // arr[1] = 12;
  // arr[2] = 13;
  // arr[3] = 14;
  printf("&arr = %p\n", &arr);
  printf("arr = %p\n", arr);
  printf("(arr + 0) = %p\n", (arr + 0));
  printf("*(arr + 0) = %p\n", *(arr + 0));
  printf("&arr[0] = %p\n", &arr[0]);
  printf("arr[0] = %p\n", arr[0]);
  printf("*arr[0] = %d\n", *arr[0]);
  // printf("arr[1] = %d\n", arr[1]);
  // printf("arr[2] = %d\n", arr[2]);
  // printf("arr[3] = %d\n", arr[3]);
}

int main()
{
  // test1();
  // test2();
  test3();
}