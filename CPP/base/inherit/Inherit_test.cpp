/* ************************************************************************
> File Name:     Inherit_test.cpp
> Author:        niu0217
> Created Time:  Thu 20 Jun 2024 11:29:42 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <vector>
using namespace std;

class Father
{
 public:
  Father(string name = "niu", int weight = 0, int age = 0)
      : name_(name), weight_(weight), age_(age)
  {
  }
  ~Father()
  {
  }

 public:
  string name_;
 
 protected:
  int weight_;

 private:
  int age_;
};

class ChildA : public Father
{
 public:
  ChildA()
      : Father()
  {
  }
  ChildA(string name, int age, int weight)
      : Father(name, age, weight)
  {
  }
  ~ChildA()
  {
  }

  void printAge()
  {
    // cout << age_ << endl; 父类的私有成员，子类是访问不了的
  }
  void printWeight()
  {
    cout << weight_ << endl;
  }
};

int main()
{
  {
    cout << "Test 1-------------------begin\n";
    Father father("niu0217", 12, 80);
    cout << father.name_ << endl;
    // 私有成员和保护成员对于类对象不可访问
    // cout << father.weight_ << endl;
    // cout << father.age_ << endl;
    cout << "Test 1-------------------end\n\n";
  }
  {
    cout << "Test 2-------------------begin\n";
    Father father("niu0217", 12, 80);
    ChildA childA(string("lisi"), 34, 999);
    cout << childA.name_ << endl;
    // cout << childA.weight_ << endl; 子类对象不可访问父类的保护成员
    // cout << childA.age_ << endl;    子类对象不可访问父类的私有成员
    childA.printWeight();
    cout << "Test 2-------------------end\n\n";
  }
}