/* ************************************************************************
> File Name:     main.cpp
> Author:        niu0217
> Created Time:  Tue 20 Aug 2024 08:50:38 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
using namespace std;

int add(int a, int b);
int sub(int a, int b);

int main()
{
  int a = 100;
  int b = 20;
  cout << add(a, b) << endl;
  cout << sub(a, b) << endl;
}

// nm -C main.o  以原始名称显示符号表信息