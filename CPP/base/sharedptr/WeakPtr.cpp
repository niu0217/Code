/* ************************************************************************
> File Name:     WeakPtr.cpp
> Author:        niu0217
> Created Time:  Mon 17 Jun 2024 02:44:16 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <memory> // 智能指针
#include <vector>
using namespace std;

/// shared_ptr 存在的双向引用问题
namespace testcase1
{

class B;
class A
{
 public:
  shared_ptr<B> b;

 public:
  A()
  {
  }
  ~A()
  {
    cout << "destoryed A\n";
  }
};

class B
{
 public:
  shared_ptr<A> a;

 public:
  B()
  {
  }
  ~B()
  {
    cout << "destoryed B\n";
  }
};

void test()
{
  auto pA = make_shared<A>();
  auto pB = make_shared<B>();

  pA->b = pB;  // 此时 b和pB的引用计数都为2
  pB->a = pA;  // 此时 a和pA的引用计数都为2

  cout << pA.use_count() << endl; // 2
  cout << pB.use_count() << endl; // 2
}

} // testcase1


/// 使用weak_ptr解决循环引用
namespace testcase2
{

class B;
class A
{
 public:
  shared_ptr<B> b;

 public:
  A()
  {
  }
  ~A()
  {
    cout << "destoryed A\n";
  }
};

class B
{
 public:
  weak_ptr<A> a;

 public:
  B()
  {
  }
  ~B()
  {
    cout << "destoryed B\n";
  }
};

void test()
{
  auto pA = make_shared<A>();
  auto pB = make_shared<B>();

  pA->b = pB; // 此时 b和pB的引用计数都为2
  pB->a = pA; // 此时 a和pA的引用计数都为1

  cout << pA.use_count() << endl; // 1
  cout << pB.use_count() << endl; // 2
}

} // testcase2

void baseTest()
{
  // 创建 weak_ptr
  shared_ptr<int> numPtr = make_shared<int>(20);
  weak_ptr<int> weaknumPtr(numPtr);
  cout << numPtr.use_count() << endl;     // 1
  cout << weaknumPtr.use_count() << endl; // 1
  cout << "--------------\n\n";

  // numPtr.reset();

  // 检查对象是否存在
  if (! weaknumPtr.expired())
  {
    cout << "我还在！！\n";
  }
  else
  {
    cout << "byebye\n";
  }

  // 获取 shared_ptr
  if(! weaknumPtr.expired())
  {
    shared_ptr<int> weakToSharedPtr = weaknumPtr.lock();
    if (weakToSharedPtr)
    {
      cout << weakToSharedPtr.use_count() << endl;  // 2
      cout << numPtr.use_count() << endl;           // 2
    }
  }
  cout << numPtr.use_count() << endl;  // 1
}

int main()
{
  // testcase1::test();
  // testcase2::test();
  baseTest();
}