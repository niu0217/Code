/* ************************************************************************
> File Name:     test.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 07:59:02 AM CST
> Description:   
 ************************************************************************/

// g++ -g -o test test.cpp

#include <iostream>
#include <cstring>
using namespace std;

void test_strncpy()
{
  char buffer[4];
  const char* str = "YYYYY";
  cout << sizeof(buffer) - 1 << endl;
  strncpy(buffer, str, sizeof(buffer) - 1);
  buffer[sizeof(buffer) - 1] = '\0';
  cout << buffer << endl;
}

int main()
{
  test_strncpy();
}