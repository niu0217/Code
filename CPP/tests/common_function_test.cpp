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

namespace TESTCASE2
{
void test()
{
  string message("nihao \0 niu0217 zaijian\n", 23);
  int length = message.size();
  char *start = const_cast<char*>(message.data());
  char *result = static_cast<char*>(memmem(start, length, "niu0217", 6));
  cout << result << endl;  // niu0217 zaijian\n

  result = strstr(start, "niu0217");
  cout << result << endl;  // 为空
}
}  // TESTCASE2

namespace TESTCASE3
{
int hexToDec(char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;

  return 0;
}
string decodeMsg(string msg)
{
  string str = string();
  const char* from = msg.data();
  for (; *from != '\0'; ++from)
  {
    // isxdigit -> 判断字符是不是16进制格式, 取值在 0-f
    // Linux%E5%86%85%E6%A0%B8.jpg
    if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
    {
      // 将16进制的数 -> 十进制 将这个数值赋值给了字符 int -> char
      // B2 == 178
      // 将3个字符, 变成了一个字符, 这个字符就是原始数据
      str.append(1, hexToDec(from[1]) * 16 + hexToDec(from[2]));

      // 跳过 from[1] 和 from[2] 因此在当前循环中已经处理过了
      from += 2;
    }
    else
    {
      // 字符拷贝, 赋值
      str.append(1, *from);
    }
  }
  str.append(1, '\0');
  return str;
}
void test()
{
  string msg = "Linux%E5%86%85%E6%A0%B8.jpg";
  msg = decodeMsg(msg);
  cout << msg << endl;
}
}  // TESTCASE3

int main()
{
  // TESTCASE1::test();
  // TESTCASE2::test();
  TESTCASE3::test();
}