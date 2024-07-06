/* ************************************************************************
> File Name:     common_functions_test.c
> Author:        niu0217
> Created Time:  Thu 04 Jul 2024 10:52:20 AM CST
> Description:   
 ************************************************************************/

// gcc -g -o common_functions_test common_functions_test.c

#include <stdio.h>
#include <stdlib.h>

void sprintfTest()
{
  char buf[1024];
  int a = 10;
  int b = 20;
  sprintf(buf, "%d %d\n", a, b);
  printf("buf = %s\n", buf);

  int c = 30;
  int d = 40;
  sprintf(buf, "%d %d\n", c, d);
  printf("buf = %s\n", buf);
}

// 可变参数
#define my_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)
void vaargsTest()
{
  int num = 42;
  my_printf("The number is: %d\n", num);
}

int main()
{
  vaargsTest();
}