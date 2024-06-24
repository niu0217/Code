/* ************************************************************************
> File Name:     eventfd_test.cpp
> Author:        niu0217
> Created Time:  Mon 24 Jun 2024 03:27:49 PM CST
> Description:
 ************************************************************************/

#include <sys/eventfd.h>
#include <assert.h>
#include <unistd.h>

#include <iostream>
#include <thread>
using namespace std;

void func(int evfd)
{
  uint64_t buffer;
  int res;
  while (1)
  {
    res = read(evfd, &buffer, sizeof(uint64_t));
    assert(res == 8);

    printf("buffer = %lu\n", buffer);
  }
}
int main()
{
  // 创建一个 eventfd 文件描述符，初始计数器的值为1，同时设置为非阻塞模式，
  // 并将返回的文件描述符存储在 evfd 变量中，以便后续事件通知的使用
  //
  // eventfd打开, 读写和关闭都效非常高, 因为它本质并不是文件, 而是kernel在
  // 内核空间(内存中)维护的一个64位计数器而已.
  int evfd = eventfd(1, 1);
  assert(evfd != -1);

  thread t(func, evfd);
  t.detach();  // 主线程不负责该子线程的资源回收了

  int res;
  uint64_t buf = 1;
  while (1)
  {
    res = write(evfd, &buf, sizeof(uint64_t));
    assert(res == 8);
    printf("write = %d\n", res);

    sleep(1);
  }
}
