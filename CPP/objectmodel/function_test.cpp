/* ************************************************************************
> File Name:     function_test.cpp
> Author:        niu0217
> Created Time:  Sun 18 Aug 2024 03:31:00 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
using namespace std;

class Room
{
public:
  int age;

  void func()
  {
    int a = 11;
    int b = 12;
    age = 10;
  }
};

void func(Room *room)
{
  int a = 11;
  int b = 12;
  room->age = 10;
}

int main()
{
  Room room;
  room.func();

  func(&room);
}