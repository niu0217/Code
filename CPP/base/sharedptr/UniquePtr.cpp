/* ************************************************************************
> File Name:     UniquePtr.cpp
> Author:        niu0217
> Created Time:  Mon 17 Jun 2024 09:11:31 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <memory> // 智能指针
#include <vector>
using namespace std;

void createTest()
{
  /// 使用new
  unique_ptr<int> pObj1(new int(5));
  cout << *pObj1 << endl;  // 5
  /// 使用make_unique
  unique_ptr<string> pObj2 = make_unique<string>("hello");
  cout<< *pObj2 << endl;  // hello
  /// 使用reset
  pObj2.reset(new string("byebye"));
  cout << *pObj2 << endl;  // byebye
  /// 使用release释放pObj2之前对象的所有权，并返回一个指向原对象的指针
  string *byeStr = pObj2.release();
  cout << *byeStr << endl; // byebye
  cout << pObj2.get() << endl; // 0
  /// 使用move，转移所有权
  unique_ptr<int> pObj3 = move(pObj1);
  cout << *pObj3 << endl; // 5
  // cout << *pObj1 << endl; // 这里会段错误，因为pObj1的所有权已经交给了pObj3，pObj1被置为了空
  cout << pObj1.get() << endl; // 0
}

void copyassignTest()
{
  unique_ptr<int> pObj1(new int(5));
  /// 下面两行报错，因为它是独占的，只能移动
  // unique_ptr<int> pObj2(pObj1);
  // unique_ptr<int> pObj2 = pObj1;

  /// 正确做法
  unique_ptr<int> pObj2 = move(pObj1);
  cout << *pObj2 << endl;  // 注意，pObj1已经不能使用了，它被自动置为了空
  cout << pObj1.get() << endl; // 0
}

/// unique_ptr不支持拷贝操作，但却有一个例外：可以从函数中返回一个unique_ptr。
unique_ptr<int> clone(int value)
{
  unique_ptr<int> pObj(new int(value));
  return pObj;
}

void returnTest()
{
  int value = 20;
  // 编译器做的事， 将clone中的局部对象pObj移动给了changed
  // 用户不需要做这个事情，感觉和编译器的优化有关吧
  unique_ptr<int> changed = clone(value); 
  cout << *changed << endl;
}

/// 为什么要使用智能指针，给出一些小例子
void goodPointsTest()
{
  {
    int *p = new int(5);

    // ...（可能会抛出异常）

    delete p;  // 前面抛出异常的话，这句话就执行不了了，造成资源泄漏
  }
  {
    // 只要unique_ptr指针创建成功，其析构函数都会被调用。确保动态资源被释放。
    unique_ptr<int> p(new int(5));

    // ...（可能会抛出异常）
  } // 即使里面发生了异常，但是p的生命周期是由智能指针管理的，会保证它的析构函数被调用
}

void vectorTest()
{
  vector<unique_ptr<int>> nums;
  unique_ptr<int> pObj1(new int(10));
  unique_ptr<int> pObj2(new int(20));
  nums.push_back(move(pObj1));
  nums.push_back(move(pObj2));
  for (auto &num : nums)
  {
    cout << *num << endl;
  }

  /// vector<unique_ptr<int>>::value_type 定义为 unique_ptr<int> 这个类型。
  /// 这是 vector 的模板参数 T 实例化后的类型。你可以像操作其他 unique_ptr<int> 对象一样使用 numsPtr。
  vector<unique_ptr<int>>::value_type numsPtr;
  numsPtr = move(nums[0]);
  cout << *numsPtr << endl; // 10
  cout << nums[0].get() << endl; // 0
}

int main()
{
  // createTest();
  // copyassignTest();
  // returnTest();
  vectorTest();
}

/// std::unique_ptr实现了独享所有权的语义
/// 一个非空的std::unique_ptr总是拥有它所指向的资源。
/// 转移一个std::unique_ptr将会把所有权也从源指针转移给目标指针（源指针被置空）