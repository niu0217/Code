/* ************************************************************************
> File Name:     function_test.cpp
> Author:        niu0217
> Created Time:  Mon 17 Jun 2024 03:53:27 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <functional>
#include <memory>
using namespace std;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

class Person
{
 public:
  typedef std::function<void()> EatCallback;
  typedef std::function<void(const string &name)> PlayCallback;

 public:
  Person();
  ~Person();
 
 public:
  void setEatCallback(EatCallback cb);
  void setPlayCallback(PlayCallback cb);

  void eatApple();
  void playPingPang(const string &name);
  void playBall(const string &name, int age);

  void startLife();

 private:
  EatCallback eatCallback_;
  PlayCallback playCallback_;
};

Person::Person()
{
}

Person::~Person()
{
}

void Person::setEatCallback(EatCallback cb)
{
  eatCallback_ = move(cb);
}

void Person::setPlayCallback(PlayCallback cb)
{
  playCallback_ = move(cb);
}

void Person::eatApple()
{
  cout << "我喜欢吃苹果\n";
}

void Person::playPingPang(const string &name)
{
  cout << name << " 喜欢打乒乓球\n";
}

void Person::playBall(const string &name, int age)
{
  cout << name << " 今年 " << age << " 岁，喜欢玩球\n";
}

void Person::startLife()
{
  eatCallback_();
  playCallback_("niu0217");
}

void eatBanana()
{
  cout << "I like bannana\n";
}

void playBasketball(const string &name)
{
  cout << name << " like basketball\n";
}

int main()
{
  Person niu;
  niu.setEatCallback(bind(&Person::eatApple, &niu));
  // niu.setPlayCallback(bind(&Person::playPingPang, &niu, _1));
  niu.setPlayCallback(bind(&Person::playBall, &niu, _1, 12));

  niu.startLife();
}