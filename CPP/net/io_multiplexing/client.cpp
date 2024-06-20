/* ************************************************************************
> File Name:     client.cpp
> Author:        niu0217
> Created Time:  Sun 16 Jun 2024 10:06:07 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <mutex>
#include <condition_variable>

#include <chrono>
#include <sstream>
#include <iomanip>

#include "base/Timestamp.h"

using std::string;

#define PORT 8888
#define BUFFER_SIZE 1024

// void clientThreadFunc(std::condition_variable &cv, std::mutex &mutex, int &count)
// {
//   int servFd = 0;
//   int valread;
//   struct sockaddr_in servAddr;
//   char buffer[1024] = {0};

//   // 创建套接字
//   if ((servFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//   {
//     std::cerr << "Socket creation error" << std::endl;
//     return;
//   }

//   servAddr.sin_family = AF_INET;
//   servAddr.sin_port = htons(PORT);

//   if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0)
//   {
//     std::cerr << "Invalid address/ Address not supported" << std::endl;
//     return;
//   }

//   // 尝试连接到服务器
//   if (connect(servFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
//   {
//     std::cerr << "Connection Failed" << std::endl;
//     return;
//   }

//   const char *msg = "Hello from client";
//   send(servFd, msg, strlen(msg), 0);
//   std::cout << "Message sent to server" << std::endl;

//   {
//     std::lock_guard<std::mutex> lock(mutex);
//     count++;
//     cv.notify_one();
//   }

//   valread = read(servFd, buffer, 1024);
//   std::cout << "Server response: " << buffer << std::endl;

//   // sleep(60);

//   close(servFd);
// }

// /// 创建 numThreads 个线程，然后都向服务器发送hello消息
// int launchXConnection(const int numThreads)
// {
//   std::vector<std::thread> clients;
//   std::mutex mutex;
//   std::condition_variable cv;
//   int count = 0;

//   /// 创建 numThreads 个线程
//   for (int i = 0; i < numThreads; i++)
//   {
//     clients.emplace_back(std::thread(clientThreadFunc, std::ref(cv), std::ref(mutex), std::ref(count)));
//   }

//   {
//     /// 等待 numThreads 个线程都完成自己的工作
//     std::unique_lock<std::mutex> lock(mutex);
//     cv.wait(lock, [&count, &numThreads] { return count == numThreads; });
//   }

//   /// 回收 numThreads 个子线程
//   for (std::thread &client : clients)
//   {
//     client.join();
//   }

//   return 0;
// }

int main()
{
  // launchXConnection(3);
  Timestamp curTime;
  std::cout << curTime.toString() << std::endl;
}
