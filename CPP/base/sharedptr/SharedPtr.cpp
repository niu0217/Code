/* ************************************************************************
> File Name:     SharedPtr.cpp
> Author:        niu0217
> Created Time:  Mon 17 Jun 2024 09:11:53 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <memory> // 智能指针
#include <vector>
using namespace std;

/// 测试使用 shared_from_this
namespace testspace1
{

class Robot : public enable_shared_from_this<Robot>
{
 public:
  Robot()
  {
    cout << "Robot created\n\n";
  }
  ~Robot()
  {
    cout << "Robot destoryed\n";
  }

  shared_ptr<Robot> clone()
  {
    return shared_from_this();
  }
};

void cloneTest()
{
  shared_ptr<Robot> robotPtr(new Robot);
  shared_ptr<Robot> cloneRobotPtr = robotPtr->clone();

  cout << robotPtr.use_count() << endl;      // 2
  cout << cloneRobotPtr.use_count() << endl; // 2

  cout << endl;
}

}  // testspace1


/// 测试不使用 shared_from_this
namespace testspace2
{

class Robot
{
 public:
  Robot()
  {
    cout << "Robot created\n\n";
  }
  ~Robot()
  {
    cout << "Robot destoryed\n";
  }

  shared_ptr<Robot> clone()
  {
    return shared_ptr<Robot>(this); // 这里不会增加this的引用计数，会出问题
  }
};

void cloneTest()
{
  shared_ptr<Robot> robotPtr(new Robot);
  shared_ptr<Robot> cloneRobotPtr = robotPtr->clone();

  cout << robotPtr.use_count() << endl;      // 1
  cout << cloneRobotPtr.use_count() << endl; // 1

  /// 到这里的时候，robotPtr会被析构两次

  cout << endl;
}

}  // testspace2

class Animal
{ 
 public:
  Animal(int age, string name)
    : age_(age),
      name_(name)
  {
    cout << "Animal created\n\n";
  }
  ~Animal()
  {
    cout << "Animal distoryed\n";
  }

  void eat(const string & food)
  {
    cout << name_ << " eat " << food << endl;
  }

 public:
  int age_;
  string name_;
};

void baseuseTest()
{
  shared_ptr<Animal> pDog = make_shared<Animal>(10, "hh");
  pDog->eat("香蕉");
  cout << pDog.use_count() << endl;  // 1

  shared_ptr<Animal> pDog2 = pDog;
  cout << pDog.use_count() << endl;  // 2
  cout << pDog2.use_count() << endl; // 2

  pDog.reset(); // 现在引用计数为 1 了

  cout << pDog.use_count() << endl;  // 0
  cout << pDog2.use_count() << endl; // 1

  cout<< endl;
}

void moveTest()
{
  shared_ptr<int> pObj1 = make_shared<int>(10);
  shared_ptr<int> pObj2 = pObj1;
  shared_ptr<int> pObj3 = pObj1;
  shared_ptr<int> pObj4 = pObj1;
  shared_ptr<int> pObj5 = pObj1;
  shared_ptr<int> pObj6 = pObj1;

  cout << pObj1.use_count() << endl;  // 6
  cout << pObj6.use_count() << endl;  // 6

  shared_ptr<int> pObj7 = move(pObj1);
  cout << pObj1.use_count() << endl; // 0
  cout << pObj7.use_count() << endl; // 6
}

void resetTest()
{
  shared_ptr<int> pObj1 = make_shared<int>(10);
  shared_ptr<int> pObj2 = pObj1;
  shared_ptr<int> pObj3 = pObj1;
  shared_ptr<int> pObj4 = pObj1;
  shared_ptr<int> pObj5 = pObj1;
  shared_ptr<int> pObj6 = pObj1;

  cout << pObj1.use_count() << endl; // 6
  cout << pObj6.use_count() << endl; // 6

  pObj1.reset();
  pObj2.reset();

  cout << pObj1.use_count() << endl;  // 0
  cout << pObj2.use_count() << endl;  // 0
  cout << pObj5.use_count() << endl;  // 4

  cout << endl;
  pObj6.reset(new int(20));
  cout << pObj5.use_count() << endl; // 3
  cout << *pObj5 << endl;            // 10
  cout << pObj6.use_count() << endl; // 1
  cout << *pObj6 << endl;            // 20
}

// 会内存泄漏 80个字节
class TestLink
{
 public:
  TestLink()
  {
    cout << "constructer" << endl;
    arr = new int[20];
  }
  ~TestLink()
  {
    cout << "destory" << endl;
  }
 private:
  int *arr;
};
// 无内存泄漏
// 特别注意 shared_ptr 一般管理的是单个对象 使用的是delete
// 如果想要管理动态数组
//  std::shared_ptr<int> arr(new int[20], std::default_delete<int[]>());
class TestNoLeaks
{
 public:
  TestNoLeaks()
  {
    cout << "constructer" << endl;
    arr = make_shared<int>(20);
  }
  ~TestNoLeaks()
  {
    cout << "destory" << endl;
  }
 private:
  shared_ptr<int> arr;
};
void testDestory()
{
  // TestLink t;
  TestNoLeaks t;
}

int main()
{
  // baseuseTest();
  // testspace1::cloneTest();
  // testspace2::cloneTest();
  // moveTest();
  // resetTest();
  testDestory();
}