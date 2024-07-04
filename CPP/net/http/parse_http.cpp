/* ************************************************************************
> File Name:     parse_http.cpp
> Author:        niu0217
> Created Time:  Wed 03 Jul 2024 12:19:19 PM CST
> Description:   
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  get /xxx/xx.txt http/1.1
void test1()
{
  char *str = "get /xxx/xx.txt http/1.1\r\n";
  char *method;
  char *url;
  char *version;

  char *start = str;
  void *end = memmem(str, strlen(str), "\r\n", 2);
  int lineSize = (char*)end - start;

  void *space = memmem(start, lineSize, " ", 1);
  int methodSize = (char*)space - start;
  method = (char*)malloc(methodSize + 1);
  strncpy(method, start, methodSize);
  method[methodSize] = '\0';
  printf("method = %s\n", method);

  start = (char*)space + 1;
  space = memmem(start, (char*)end - start, " ", 1);
  int urlSize = (char*)space - start;
  url = (char*)malloc(urlSize + 1);
  strncpy(url, start, urlSize);
  method[urlSize] = '\0';
  printf("url = %s\n", url);

  start = (char*)space + 1;
  version = (char*)malloc((char*)end - start + 1);
  strncpy(version, start, (char*)end - start);
  method[(char*)end - start] = '\0';
  printf("version = %s\n", version);

  free(method);
  free(url);
  free(version);
}

int main()
{
  test1();
}