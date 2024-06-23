/* ************************************************************************
> File Name:     original_server.cpp
> Author:        niu0217
> Created Time:  Sun 16 Jun 2024 09:32:21 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8888
#define BUFFER_SIZE 1024

#pragma GCC diagnostic ignored "-Wold-style-cast"

int main()
{
  int listenFd;
  int connFd;
  ssize_t valread;
  struct sockaddr_in serAddress;
  struct sockaddr_in cliAddress;
  int opt = 1;
  int addrlen = sizeof(serAddress);
  char buffer[BUFFER_SIZE] = {0};

  // Create a socket
  if ((listenFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    std::cerr << "Socket creation failed" << std::endl;
    return 1;
  }
  printf("listenFd = %d\n\n", listenFd);

  // Set socket options
  if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    std::cerr << "Setsockopt failed" << std::endl;
    return 1;
  }

  serAddress.sin_family = AF_INET;
  serAddress.sin_addr.s_addr = INADDR_ANY;
  serAddress.sin_port = htons(PORT);

  // Bind the socket to localhost:PORT
  if (bind(listenFd, (struct sockaddr *)&serAddress, sizeof(serAddress)) < 0)
  {
    std::cerr << "Bind failed" << std::endl;
    return 1;
  }

  // Start listening for incoming connections
  if (listen(listenFd, 3) < 0)
  {
    std::cerr << "Listen failed" << std::endl;
    return 1;
  }

  // Loop to receive messages and echo back
  int cliNumber = 1;
  while (true)
  {
    // Accept an incoming connection
    if ((connFd = accept(listenFd, (struct sockaddr *)&cliAddress, (socklen_t *)&addrlen)) < 0)
    {
      if (errno == EMFILE)
      {
        // 对于这个服务器永远不会发生，因为它一次只能处理一个客户连接，所以
        // 不会存在文件描述符用光的情况
        printf("我是服务器，我已经没有可以用的文件描述符了，呜呜呜\n\n");
        break;
      }
      std::cerr << "Accept failed" << std::endl;
      return 1;
    }
    printf("客户连接%d   connfd = %d\n", cliNumber, connFd);
    cliNumber++;

    valread = read(connFd, buffer, BUFFER_SIZE);
    if (valread <= 0)
    {
      // 说明客户端断开了连接
      close(connFd);
      break;
    }

    std::cout << "Client: " << buffer << std::endl;
    send(connFd, buffer, strlen(buffer), 0);
    memset(buffer, 0, BUFFER_SIZE);

    // sleep(2);
    // close(connFd);
  }

  return 0;
}