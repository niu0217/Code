/* ************************************************************************
> File Name:     function_signature.cpp
> Author:        niu0217
> Created Time:  Sun 04 Aug 2024 03:41:19 PM CST
> Description:   
 ************************************************************************/

// g++ -c function_signature.cpp -o function_signature.o
// nm function_signature.o

#include <iostream>
using namespace std;

// _Z8sayHellov
//   _Z 前缀：表示这是一个修饰后的名称，通常是为了标识这个名称是 C++ 编译所生成的。
//   8sayHello：数字 8 表示后面跟随的名称的长度（8 个字符），即 sayHello
//   v 后缀：表示这个函数没有参数（Void），在 C++ 中，v 是表示没有参数的标识。
void sayHello()
{
  cout << "hello\n";
}

// _Z8sayHelloi
void sayHello(int a)
{
  cout << "hello " << a << endl;
}

class Books
{
public:
  // _ZN5BooksC1Ev 构造函数
  // _ZN5BooksC2Ev 默认构造函数
  Books()
  {
  }
  // _ZN5BooksD1Ev 析构函数
  // _ZN5BooksD2Ev 默认析构函数
  ~Books()
  {
  }

  // _ZN5Books11printNumberEv
  void printNumber()
  {
    cout << m_cnt << endl;
  }

  // _ZN5Books12printAddressEv
  virtual void printAddress()
  {
    cout << "Chengdu\n";
  }

public:
  int m_cnt;
};

int main()
{
  sayHello();
  sayHello(2);
  Books books;
  books.printNumber();
}