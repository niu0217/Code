/* ************************************************************************
> File Name:     maybe_mmleak_test.cpp
> Author:        niu0217
> Created Time:  Fri 12 Jul 2024 09:30:05 AM CST
> Description:   
 ************************************************************************/

// g++ -g -o maybe_mmleak_test maybe_mmleak_test.cpp
// sudo valgrind --leak-check=full ./maybe_mmleak_test

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

namespace TESTCASE1
{

void test1()
{
  struct epoll_event* m_events;
  m_events = new struct epoll_event[120];
}
void test2()
{
  struct epoll_event* m_events;
  m_events = new struct epoll_event[120];
  delete m_events;
}
void test3()
{
  struct epoll_event* m_events;
  m_events = new struct epoll_event[120];
  delete []m_events;
}

}  // TESTCASE1

int main()
{
  TESTCASE1::test3();
}