/* ************************************************************************
> File Name:     type_conversion.cpp
> Author:        niu0217
> Created Time:  Fri 02 Aug 2024 05:59:20 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Animal
{
public:
  Animal(string name = "")
    : m_name(name)
  {
  }
  void eat()
  {
    cout << m_name << " eat...\n";
  }
  virtual void sayhello()
  {
    cout << "nihao\n";
  }
protected:
  string m_name;
};

class Dog : public Animal
{
public:
  Dog(string name, int age)
    : Animal(name), m_age(age)
  {
  }
public:
  int m_age;
};

class Fruits
{
public:
  Fruits(string name)
    : m_name(name)
  {
  }
  virtual void address()
  {
    cout << "Unkwon\n";
  }
public:
  string m_name;
};

int main()
{
  Dog dog("niu", 12);
  cout << dog.m_age << endl;

  Animal *animal = nullptr;
  animal = &dog;
  // cout << animal->m_age << endl;

  Dog *dogPtr = dynamic_cast<Dog*>(animal);
  if (dogPtr == nullptr)
  {
    cout << "Failed\n";
  }
  else
  {
    cout << dogPtr->m_age << endl;
  }

  Fruits *fruits = nullptr;
  dogPtr = dynamic_cast<Dog*>(fruits);
  if (dogPtr == nullptr)
  {
    cout << "Failed\n";
  }
  else
  {
    cout << dogPtr->m_age << endl;
  }
}