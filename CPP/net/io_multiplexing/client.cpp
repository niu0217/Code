/* ************************************************************************
> File Name:     client.cpp
> Author:        niu0217
> Created Time:  Sun 16 Jun 2024 10:06:07 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <vector>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <mutex>
#include <condition_variable>

#define PORT 8888
#define BUFFER_SIZE 1024

void clientThreadFunc(std::condition_variable &cv, std::mutex &mutex, int &count)
{
  int servFd = 0;
  int valread;
  struct sockaddr_in servAddr;
  char buffer[1024] = {0};

  // 创建套接字
  if ((servFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    std::cerr << "Socket creation error" << std::endl;
    return;
  }

  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(PORT);

  // 尝试连接到服务器
  if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0)
  {
    std::cerr << "Invalid address/ Address not supported" << std::endl;
    return;
  }

  if (connect(servFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
  {
    std::cerr << "Connection Failed" << std::endl;
    return;
  }

  const char *msg = "Hello from client";
  send(servFd, msg, strlen(msg), 0);
  std::cout << "Message sent to server" << std::endl;

  {
    std::lock_guard<std::mutex> lock(mutex);
    count++;
    cv.notify_one();
  }

  valread = read(servFd, buffer, 1024);
  std::cout << "Server response: " << buffer << std::endl;

  // sleep(60);

  close(servFd);
}

int originalClient()
{
  const int kConnectTimes = 1000;
  struct sockaddr_in servAddr;
  int servFd = 0;
  int valread;
  char buffer[BUFFER_SIZE] = {0};
  const char *hello = "Hello from client";

  // Create a socket
  if ((servFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    std::cerr << "Socket creation error" << std::endl;
    return -1;
  }
  printf("服务器端套接字： %d\n", servFd);

  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0)
  {
    std::cerr << "Invalid address/ Address not supported" << std::endl;
    return -1;
  }

  // Connect to the server
  // 这里我想实现发起1000个连接，模拟将original服务器的文件描述符用尽，但是实际服务器
  // 并不能处理多个客户连接，所以在发起第二次客户连接的时候，就会返回 "Connection failed"
  for (int i = 0; i < kConnectTimes; i++)
  {
    if (connect(servFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
      std::cerr << "Connection failed" << std::endl;
      return -1;
    }

    // Send a message to the server
    send(servFd, hello, strlen(hello), 0);
    std::cout << "Message sent to server: " << hello << std::endl;

    // Receive message back from the server
    valread = read(servFd, buffer, BUFFER_SIZE);
    std::cout << "Message received from server: " << buffer << std::endl;
  }

  close(servFd);
  return 0;
}

int selectClient_10hello()
{
  const int kConnectTimes = 10;
  struct sockaddr_in servAddr;
  int servFd = 0;
  int valread;
  char buffer[BUFFER_SIZE] = {0};
  const char *hello = "Hello from client";

  // Create a socket
  if ((servFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    std::cerr << "Socket creation error" << std::endl;
    return -1;
  }
  printf("服务器端套接字： %d\n", servFd);

  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0)
  {
    std::cerr << "Invalid address/ Address not supported" << std::endl;
    return -1;
  }

  if (connect(servFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
  {
    std::cerr << "Connection failed" << std::endl;
    return -1;
  }

  // Connect to the server
  // 给服务器发送10条hello信息
  for (int i = 0; i < kConnectTimes; i++)
  {
    // Send a message to the server
    send(servFd, hello, strlen(hello), 0);
    std::cout << "Message sent to server: " << hello << std::endl;

    // Receive message back from the server
    valread = read(servFd, buffer, BUFFER_SIZE);
    std::cout << "Message received from server: " << buffer << std::endl;
  }

  sleep(60);
  close(servFd);
  return 0;
}

/// 创建 threadNum 个线程，然后都向服务器发送hello消息
int launchXConnection(const int threadNum)
{
  std::vector<std::thread> clients;
  std::mutex mutex;
  std::condition_variable cv;
  int count = 0;

  /// 创建 threadNum 个线程
  for (int i = 0; i < threadNum; i++)
  {
    clients.emplace_back(std::thread(clientThreadFunc, std::ref(cv), std::ref(mutex), std::ref(count)));
  }

  {
    /// 等待 threadNum 个线程都完成自己的工作
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [&count, &threadNum] { return count == threadNum; });
  }

  /// 回收 threadNum 个子线程
  for (std::thread &client : clients)
  {
    client.join();
  }

  return 0;
}

int main()
{
  // originalClient();
  // selectClient_10hello();
  launchXConnection(2);
}
