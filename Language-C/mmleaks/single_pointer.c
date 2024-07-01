/* ************************************************************************
> File Name:     single_pointer.c
> Author:        niu0217
> Created Time:  Mon 01 Jul 2024 11:18:26 AM CST
> Description:   
 ************************************************************************/
// 编译：gcc -g -o single single_pointer.c
// 检测：valgrind --leak-check=full ./single

#include <stdio.h>
#include <stdlib.h>

// 会内存泄漏
void test1()
{
  int *arr = (int *)malloc(sizeof(int));
  *arr = 10;
  printf("arr = %d\n", *arr);
  printf("&arr = %p\n", &arr);   // 栈 0x7ffcdc4cf5a0 自动释放
  printf("arr = %p\n", arr);     // 堆 0x55825e5eb2a0 需要我们free
}

// 不会内存泄漏
void test2()
{
  int *arr = (int *)malloc(sizeof(int));
  *arr = 10;
  printf("arr = %d\n", *arr);
  printf("&arr = %p\n", &arr);   // 栈 0x7ffcdc4cf5a0 自动释放
  printf("arr = %p\n", arr);     // 堆 0x55825e5eb2a0 需要我们free
  free(arr); // 释放的就是 0x55825e5eb2a0 这一块空间
}

int main()
{
  // test1();
  test2();
}