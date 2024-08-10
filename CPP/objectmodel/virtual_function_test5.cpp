/* ************************************************************************
> File Name:     virtual_function_test5.cpp
> Author:        niu0217
> Created Time:  Sun 04 Aug 2024 02:16:59 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Animal
{
public:
  Animal(int age)
    : m_age(age)
  {
  }

  virtual void speak()
  {
  }

  virtual void eatFood()
  {
  }

  virtual void sleep()
  {
  }

  void printAge()
  {
    cout << m_age << endl;
  }

public:
  int m_age;
};

class Dog : public Animal
{
public:
  Dog(int age)
    : Animal(age)
  {
  }

  void speak() override
  {
    cout << "汪汪汪\n";
  }

  void eatFood() override
  {
    cout << "吃骨头\n";
  }

  void sleep() override
  {
    cout << "睡20分钟\n";
  }
};

int main()
{
  Animal *dogPtr = new Dog(10);
  dogPtr->printAge();
  dogPtr->speak();  // 0x0
  dogPtr->eatFood();  // 0x8
  dogPtr->sleep();  // 0x10
}