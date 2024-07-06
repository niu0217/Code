/* ************************************************************************
> File Name:     common_function_test.cpp
> Author:        niu0217
> Created Time:  Sat 06 Jul 2024 01:22:04 PM CST
> Description:   
 ************************************************************************/

// g++ -g -o common_function_test common_function_test.cpp

#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

namespace TESTCASE1
{
char *httpMessage = "Get /hello.cpp Http/1.1\r\n";
char *start = httpMessage;
char *end = (char*)memmem(start, strlen(httpMessage), "\r\n", 2);
string method = "";
string url = "";
string version = "";

char* splitRequestLine(char *start, char *end, char *sub, string &message)
{
  char *space = end;
  if (sub != nullptr)
  {
    space = static_cast<char*>(memmem(start, end - start, sub, strlen(sub)));
  }
  int length = space - start;
  message = string(start, length);
  return space + 1;
}
void test()
{
  start = splitRequestLine(start, end, " ", method);
  cout << method << endl;

  start = splitRequestLine(start, end, " ", url);
  cout << url << endl;

  splitRequestLine(start, end, nullptr, version);
  cout << version << endl;
}
}  // TESTCASE1

int main()
{
  TESTCASE1::test();
}