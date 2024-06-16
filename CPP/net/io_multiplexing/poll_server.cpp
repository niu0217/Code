/* ************************************************************************
> File Name:     poll_server.cpp
> Author:        niu0217
> Created Time:  Sun 16 Jun 2024 09:09:06 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/poll.h>

#define MAX_CLI 50

int clients[MAX_CLI];  // 保存所有已经连接的客户端文件描述符
struct sockaddr_in clientaddr;
socklen_t len = sizeof(clientaddr);

int createSocket(int port)
{ 
  /// 创建并绑定网络监听的socket套接字
  int fd;
  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  // Set socket options
  int opt = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    std::cerr << "Setsockopt failed" << std::endl;
    return 1;
  }
  struct sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
  addr.sin_port = htons(port);
  if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  if (listen(fd, 20) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  return fd;
}

int recvMsg(int fd, char *buff, int size)
{
  int rsize = recv(fd, buff, sizeof(char) * size, 0);
  if (rsize <= 0)
  {
    if (rsize == 0)
    {
      // 说明对等端关闭了连接
      return 0;
    }
    return -1;
  }
  printf("Recv : %s\n", buff);
  return 1;
}

int sendMsg(int fd, char *buff, int size)
{
  int ssize = send(fd, buff, sizeof(char) * size, 0);
  if (ssize <= 0)
  {
    if (ssize == 0)
      return 0;
    return -1;
  }
  printf("Send success!\n");
  return 1;
}

void echoWork(int listenFd)
{
  struct pollfd fds[MAX_CLI + 1];
  // 将监听文件描述符作为第一个元素，监听它的可读事件
  fds[0].fd = listenFd;
  fds[0].events = POLLIN;
  int maxFd = listenFd;

  while (1)
  {
    int ret = poll(fds, MAX_CLI + 1, -1);
    if (ret == -1)
    {
      perror("poll");
      exit(EXIT_FAILURE);
    }

    // 接受连接请求
    if (fds[0].revents & POLLIN)
    {
      int connFd;
      if ((connFd = accept(listenFd, (struct sockaddr *)&clientaddr, &len)) < 0)
      {
        /// TODO 处理文件描述符用光的情况
        perror("accept");
        exit(EXIT_FAILURE);
      }
      printf("one client connect success!\n");

      for (int i = 0; i < MAX_CLI; i++)
      {
        if (clients[i] == -1)
        {
          clients[i] = connFd;
          break;
        }
      }

      // 更新pollfd数组
      // 也就是将刚新连接到服务器的客户端文件描述符添加到fds数组中，让poll关注这些
      // 文件描述符
      for (int i = 1; i <= MAX_CLI; i++)
      {
        if (clients[i - 1] != -1)
        {
          fds[i].fd = clients[i - 1];
          fds[i].events = POLLIN;
          maxFd = fds[i].fd > maxFd ? fds[i].fd : maxFd;
        }
      }
    }

    // 检查客户端套接字是否有数据可读
    for (int i = 1; i <= MAX_CLI; i++)
    {
      if (fds[i].revents & POLLIN) // 第i个客户端有数据可读
      {
        char buff[1024] = {0};
        int recvState = recvMsg(fds[i].fd, buff, 1024);
        if (recvState <= 0)
        {
          if (recvState == 0)
          {
            printf("disconnect one client\n");
          }
          else
          {
            printf("读取数据出错！！！\n");
          }
          close(fds[i].fd);
          clients[i - 1] = -1;
          continue;
        }

        int sendState = sendMsg(fds[i].fd, buff, 1024);
        if (sendState <= 0)
        {
          if (sendState == 0)
          {
            printf("disconnect one client\n");
          }
          else
          {
            printf("发送数据出错！！！ disconnect one client\n");
          }
          close(fds[i].fd);
          clients[i - 1] = -1;
          continue;
        }
      }
    }
  }
  
  return;
}

int main()
{
  int listenFd;

  if ((listenFd = createSocket(8888)) < 0)
  {
    perror("createSocket");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < MAX_CLI; i++)
  {
    clients[i] = -1;
  }

  echoWork(listenFd);

  return 0;
}
