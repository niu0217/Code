/* ************************************************************************
> File Name:     function_test.cpp
> Author:        niu0217
> Created Time:  Sun 30 Jun 2024 01:03:50 PM CST
> Description:   
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

// 使用 sscanf 解析字符串
void test1()
{
  const char *context = "http://www.baidu.com:1234";
  char protocol[32] = { 0 };
  char host[128] = { 0 };
  char port[8] = { 0 };
  sscanf(context, "%[^:]://%[^:]:%[1-9]", protocol, host, port);
  printf("%s\n", protocol);
  printf("%s\n", host);
  printf("%s\n", port);
}
void test2()
{
  const char *context = "123456 abcdef";
  char number[100];
  char letter[100];
  sscanf(context, "%[^ ] %[^ ]", number, letter);
  printf("%s\n", number);
  printf("%s\n", letter);
}
void test3()
{
  const char *context = "123456abcdef";
  char buf[100];
  sscanf(context, "%[^A-Z]", buf);
  printf("%s\n", buf);
}
void test4()
{
  const char *context = "Get /root/hello.cpp HTTP/1.1";
  char method[32] = { 0 };
  char path[128] = { 0 };
  char version[10] = { 0 };
  sscanf(context, "%[^ ] %[^ ] %[^ ]", method, path, version);
  printf("%s\n", method);
  printf("%s\n", path);
  printf("%s\n", version);
}

int main()
{
  
}