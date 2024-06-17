/* ************************************************************************
> File Name:     NiuString_test.cpp
> Author:        niu0217
> Created Time:  Mon 17 Jun 2024 08:31:06 PM CST
> Description:   
 ************************************************************************/

#include "NiuString.h"

int main()
{
  {
    // 默认构造
    NiuString s1;
    s1.printData();

    // 传参构造
    NiuString s2("hello niu0217");
    s2.printData();

    // 拷贝构造
    NiuString s3(s2);
    s3.printData();

    // 移动构造
    NiuString s4(move(s3));
    s4.printData();

    // 拷贝赋值
    NiuString s5;
    s5 = s4;
    s5.printData();

    // 移动赋值
    NiuString s6;
    s6 = move(s5);
    s6.printData();
  }  // 在这里全部析构

  cout << endl;
}