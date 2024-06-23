/* ************************************************************************
> File Name:     epoll_server.cpp
> Author:        niu0217
> Created Time:  Sun 16 Jun 2024 09:09:15 AM CST
> Description:   
 ************************************************************************/

/// 参考：https://cloud.tencent.com/developer/article/1694517


/*
  select和poll 监听文件描述符list，进行一个线性的查找 O(n);
  epoll 使用了内核文件级别的回调机制O(1);

  epoll_create1: 创建一个epoll实例，返回文件描述符
  epoll_ctl:     将监听的文件描述符添加到epoll实例中，实例代码为将标准输入文件描述符添加到epoll中
  epoll_wait:    等待epoll事件从epoll实例中发生， 并返回事件以及对应文件描述符
*/

/*

typedef union epoll_data
{
  void *ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;

struct epoll_event
{
  uint32_t events;  / Epoll events /
  epoll_data_t data;    / User data variable /
};

*/

#include <string>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <unordered_map>

#pragma GCC diagnostic ignored "-Wold-style-cast"

#define MAX_PENDING 1024
#define BUFFER_SIZE 1024

class Handler
{
public:
  virtual ~Handler() {}
  virtual int handle(epoll_event &e) = 0;
};

/**
 * epoll 事件轮询
 */
class IOLoop
{
public:
  static IOLoop *Instance()
  {
    static IOLoop instance;
    return &instance;
  }
  ~IOLoop()
  {
    for (auto it : handlers_)
    {
      delete it.second;
      it.second = nullptr;
    }
    close(epfd_);
  }

  void start()
  {
    const uint64_t kMAX_EVENTS = 10;
    epoll_event events[kMAX_EVENTS];
    while (true)
    {
      // -1 只要没有事件一直阻塞
      // 我们也可以自己设置一个合理的超时时间
      int nfds = epoll_wait(epfd_, events, kMAX_EVENTS, -1 /*Timeout*/);
      for (int i = 0; i < nfds; ++i)
      {
        int fd = events[i].data.fd;
        Handler *handler = handlers_[fd];
        handler->handle(events[i]);
      }
    }
  }

  void addHandler(int fd, Handler *handler, unsigned int events)
  {
    handlers_[fd] = handler;
    epoll_event event;
    event.data.fd = fd;
    event.events = events;

    if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &event) < 0)
    {
      std::cout << "Failed to insert handler to epoll" << std::endl;
    }
  }

  void modifyHandler(int fd, unsigned int events)
  {
    struct epoll_event event;
    event.data.fd = fd; // 必须必须必须加上这一行!!!
    event.events = events;
    if (epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &event) < 0)
    {
      std::cout << "Failed to modify handler to epoll" << std::endl;
    }
  }

  void removeHandler(int fd)
  {
    Handler *handler = handlers_[fd];
    handlers_.erase(fd);
    delete handler;
    handler = nullptr;
    // 将fd从epoll堆删除
    if (epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL) < 0)
    {
      std::cout << "Failed to remove handler to epoll" << std::endl;
    }
  }

private:
  IOLoop()
  {
    epfd_ = epoll_create1(0); // flag=0 等价于epll_create
    if (epfd_ < 0)
    {
      std::cout << "Failed to create epoll" << std::endl;
      exit(1);
    }
  }

private:
  int epfd_;
  std::unordered_map<int, Handler *> handlers_;
};

class EchoHandler : public Handler
{
public:
  EchoHandler() {}
  virtual int handle(epoll_event &e) override
  {
    int fd = e.data.fd;
    if (e.events & EPOLLHUP)
    {
      /// EPOLLHUP 是 Linux epoll 事件机制中的一个事件标志。它表示关联的套接字
      /// 已经失去了连接（例如服务器关闭了连接），通常与可读和可写事件一同使用来检测和处理异常情况。
      IOLoop::Instance()->removeHandler(fd);
      return -1;
    }

    if (e.events & EPOLLERR)
    {
      return -1;
    }

    if (e.events & EPOLLIN) // 可读事件
    {
      std::cout << "read" << std::endl;
      received = recv(fd, buffer, BUFFER_SIZE, 0);
      if (received < 0)
      {
        std::cout << "Error reading from socket" << std::endl;
      }
      else if (received > 0)
      {
        buffer[received] = '\0';
        std::cout << "Reading: " << buffer << std::endl;
        if (strcmp(buffer, "stop") == 0)
        {
          std::cout << "stop----" << std::endl;
        }
      }

      if (received > 0)
      {
        // 有数据可以发送给对等端，因此注册可写事件
        IOLoop::Instance()->modifyHandler(fd, EPOLLOUT);
      }
      else
      {
        /// 也就是read返回0，对等方关闭了连接
        std::cout << "disconnect fd=" << fd << std::endl;
        IOLoop::Instance()->removeHandler(fd);
      }
    }

    if (e.events & EPOLLOUT) // 可写事件
    {
      if (received > 0)
      {
        std::cout << "Writing: " << buffer << std::endl;
        if (send(fd, buffer, received, 0) != received)
        {
          std::cout << "Error writing to socket" << std::endl;
        }
        IOLoop::Instance()->modifyHandler(fd, EPOLLIN);
      }
    }

    return 0;
  }

private:
  ssize_t received = 0;
  char buffer[BUFFER_SIZE];
};

class ServerHandler : public Handler
{
public:
  ServerHandler(uint16_t port)
  {
    int listenFd;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    if ((listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
      std::cout << "Failed to create server socket" << std::endl;
      exit(1);
    }
    setSocketOpt(listenFd);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(listenFd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
      std::cout << "Failed to bind server socket" << std::endl;
      exit(1);
    }

    if (listen(listenFd, MAX_PENDING) < 0)
    {
      std::cout << "Failed to listen on server socket" << std::endl;
      exit(1);
    }
    setnonblocking(listenFd);

    IOLoop::Instance()->addHandler(listenFd, this, EPOLLIN);
  }

  virtual int handle(epoll_event &e) override
  {
    int fd = e.data.fd;  // 这个其实就是监听套接字
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int connFd = accept(fd, (struct sockaddr *)&clientAddr, &clientAddrLen);

    if (connFd < 0)
    {
      if (errno == EAGAIN)
      {
        std::cout << "目前没有客户连接\n";
      }
      /// TODO 处理文件描述符用尽的情况
      std::cout << "Error accepting connection" << std::endl;
      return -1;
    }

    std::cout << "accept connected: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
    Handler *clientHandler = new EchoHandler();
    // 刚加入时只关注它的可读事件
    IOLoop::Instance()->addHandler(connFd, clientHandler, EPOLLIN);
    return 0;
  }

private:
  void setnonblocking(int fd)
  {
    // 在这种模式下，如果执行的操作不能立即完成（例如读取一个未准备好的文件），
    // 那么调用将不会阻塞调用者，而是立即返回错误或特殊值。这对于那些不希望等待I/O操作完成
    // 的应用程序很有用，因为它们可以继续执行其他任务而不是等待I/O操作完成。
    // 注意：这样实现的是非阻塞同步，文件描述符准备就绪后，从内核中拷贝数据到应用中，
    //      这是同步的
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  }
  int setSocketOpt(int fd)
  {
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
      std::cerr << "Setsockopt failed" << std::endl;
      return 1;
    }
    return 0;
  }
};

int main()
{
  ServerHandler serverhandler(8888);
  IOLoop::Instance()->start();
  return 0;
}