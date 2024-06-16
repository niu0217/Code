/* ************************************************************************
> File Name:     select_server.cpp
> Author:        niu0217
> Created Time:  Sun 16 Jun 2024 09:08:55 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

#define MAX_CLIENTS 50
#define BUFFER_SIZE 1024

void printConn(const int *clientArr)
{
  for(int i = 0; i < MAX_CLIENTS; i++)
  {
    printf("conn %d = %d\n", i, clientArr[i]);
  }
  printf("\n");
}

void writeConnToFile(const int * clientArr)
{
  std::ofstream outFile("output.txt", std::ios::out | std::ios::app);
  if(outFile.is_open())
  {
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
      outFile << "conn " << i << " = " << clientArr[i] << std::endl;
    }
    outFile << std::endl;
  }
  else
  {
    std::cerr << "打开文件失败\n";
  }

  outFile.close();
}

int main()
{
  int listenFd;
  int connFd;
  const int kMaxClents = MAX_CLIENTS;
  int clientArr[MAX_CLIENTS] = {0};  // 保存已经连接的客户端文件描述符
  int activity;
  int maxFd;
  int curFd;
  int opt = 1;
  struct sockaddr_in servAddr;
  struct sockaddr_in clieAddr;
  fd_set readfds;
  char buffer[BUFFER_SIZE];

  // Create a socket
  if ((listenFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }
  printf("listenFd = %d\n\n", listenFd);

  // Set socket options
  if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    std::cerr << "Setsockopt failed" << std::endl;
    return 1;
  }

  // Prepare the sockaddr_in structure
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  servAddr.sin_port = htons(8888);

  // Bind
  if (bind(listenFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
  {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  // Listen
  if (listen(listenFd, 3) < 0)
  {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }

  // Accept the incoming connection
  int addrlen = sizeof(servAddr);

  while (true)
  {
    /// 每次处理完之后readfds都会被清除，因此我们每次都必须重新设置
    FD_ZERO(&readfds);
    FD_SET(listenFd, &readfds);
    maxFd = listenFd;

    for (int i = 0; i < kMaxClents; i++)
    {
      curFd = clientArr[i];

      if (curFd > 0)
      {
        FD_SET(curFd, &readfds);
      }

      if (curFd > maxFd)
      {
        maxFd = curFd;
      }
    }
    // printConn(clientArr);
    writeConnToFile(clientArr);

    // 使用select关注多个文件描述符的事件
    // 在这里会阻塞，直到readfds中的文件描述符有事件发生
    activity = select(maxFd + 1, &readfds, NULL, NULL, NULL);

    // 处理新的客户连接
    if (FD_ISSET(listenFd, &readfds))
    {
      /// accept只是从全连接队列中取出一个来处理，没有accept也可以完成tcp三次握手
      if ((connFd = accept(listenFd, (struct sockaddr *)&clieAddr, (socklen_t *)&addrlen)) < 0)
      {
        // 在这里会出现服务器文件描述符用尽的情况
        perror("Accept failed");
        exit(EXIT_FAILURE);
      }

      // 将connFd保存起来
      for (int i = 0; i < kMaxClents; i++)
      {
        if (clientArr[i] == 0)
        {
          clientArr[i] = connFd;
          break;
        }
      }
    }

    // 处理clientArr中客户发生的事件
    for (int i = 0; i < kMaxClents; i++)
    {
      curFd = clientArr[i];

      if (FD_ISSET(curFd, &readfds))
      {
        int valread;
        if ((valread = read(curFd, buffer, BUFFER_SIZE)) == 0)
        {
          /// 特别注意：在这里我们没有数据要处理，因此这个过程如果我们抓包的话，会发现
          ///         FIN和ACK合并在一起发送给了对等端，也是四次挥手合并成了三次挥手
          /// 对端调用了close函数，也就是收到了对端的FIN报文
          getpeername(curFd, (struct sockaddr *)&servAddr, (socklen_t *)&addrlen);
          close(curFd);  // 向对端发送 FIN
          clientArr[i] = 0;
        }
        else
        {
          /// 将客户端发送来的消息发回给客户端
          buffer[valread] = '\0';
          send(curFd, buffer, strlen(buffer), 0);
        }
      }
    }
  }

  close(listenFd);

  return 0;
}