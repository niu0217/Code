/* ************************************************************************
> File Name:     Fork_test.cpp
> Author:        niu0217
> Created Time:  Fri 21 Jun 2024 10:32:43 AM CST
> Description:   
 ************************************************************************/

#include "base/CurrentThread.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

namespace
{
__thread int x = 0;
}

void print()
{
  printf("pid=%d tid=%d x=%d\n", getpid(), CurrentThread::tid(), x);
}

int main()
{
  printf("parent %d\n", getpid());
  print();
  x = 1;
  print();
  pid_t p = fork();

  if (p == 0)
  {
    printf("chlid %d\n", getpid());
    // child
    print();
    x = 2;
    print();

    if (fork() == 0)
    {
      printf("grandchlid %d\n", getpid());
      print();
      x = 3;
      print();
    }
  }
  else
  {
    // parent
    print();
  }
}
