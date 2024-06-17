/* ************************************************************************
> File Name:     function_baseuse.cpp
> Author:        niu0217
> Created Time:  Mon 17 Jun 2024 04:28:17 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <functional>
#include <memory>
using namespace std;

/// 使用function存储lambda对象
void storeLambda()
{
  function<int(int, int)> add = [](int a, int b) { return a + b; };
  int result = add(10, 20);
  cout << result << endl;
}

/// 拷贝一个function
void copyFunction()
{
  function<int(int, int)> add = [](int a, int b) { return a + b; };
  function<int(int, int)> copyAdd = add;
  int result = copyAdd(10, 20);
  cout << result << endl;
}

/// 重置和清空
void doResetAndClear()
{
  function<int(int, int)> add = [](int a, int b) { return a + b; };
  add = [](int a, int b) { return a * b; };
  int result = add(10, 20);
  cout << result << endl;

  add = nullptr;
}

/// 测试move操作
void foo(function<void()> func)
{
  func();
}
void doMove()
{
  function<void()> func1 = []() { cout << "nihao\n"; };
  function<void()> func2 = move(func1);
  func2();
  // func1(); 出错，已经被移动给func2了
  foo(move(func2));
  // func2(); 出错
}

/// 和bind配合使用
namespace testcase1
{

class MyClass
{
 public:
  void eat(int value)
  {
    cout << "niu eat " << value << " number apple\n";
  }
};

void sayByeye(int value)
{
  cout << "niu say " << value << " times byebye\n";
}

void doBind()
{
  MyClass myObj;
  function<void(int)> eatFunc = bind(&MyClass::eat, &myObj, std::placeholders::_1);
  eatFunc(23);

  function<void(int)> sayByeyeFunc = bind(&sayByeye, std::placeholders::_1);
  sayByeyeFunc(200);

  function<void(int)> func = move(eatFunc);
  func(20000000);
}

}  // testcase1

int main()
{
  // storeLambda();
  // copyFunction();
  // doResetAndClear();
  // doMove();
  testcase1::doBind();
}