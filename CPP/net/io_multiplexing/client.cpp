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
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "base/Thread.h"
#include "base/CountDownLatch.h"
#include "base/Logging.h"
#include "base/TimeZone.h"
#include "base/LogFile.h"

using namespace std;

#pragma GCC diagnostic ignored "-Wold-style-cast"

#define PORT 8888
#define BUFFER_SIZE 1024

unique_ptr<LogFile> g_logFile;

void dummyOutput(const char* msg, int len)
{
  if (g_logFile)
  {
    g_logFile->append(msg, len);
  }
}

void clientThreadFunc()
{
  int servFd = 0;
  ssize_t valread;
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

  if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0)
  {
    std::cerr << "Invalid address/ Address not supported" << std::endl;
    return;
  }

  // 尝试连接到服务器
  if (connect(servFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
  {
    std::cerr << "Connection Failed" << std::endl;
    return;
  }

  const char *msg = "Hello from client";
  send(servFd, msg, strlen(msg), 0);

  valread = read(servFd, buffer, 1024);
  if(valread == 0)
  {
    cout << "服务器已经断开了连接\n";
    close(servFd);
    return;
  }
  // std::cout << "Server response: " << buffer << std::endl;

  close(servFd);
  LOG_INFO << CurrentThread::name() << "  leave";
}

void quickTask()
{
  LOG_INFO << CurrentThread::name() << "  leave";
}

/// 创建 numThreads 个线程，然后都向服务器发送hello消息
void launchXConnection(const int numThreads)
{
  vector<unique_ptr<Thread>> threads;
  threads.reserve(numThreads);
  for(int i = 0; i < numThreads; i++)
  {
    threads.emplace_back(new Thread(clientThreadFunc));
    threads[i]->start();
  }

  for(auto &thread : threads)
  {
    thread->join();
  }
}

int main()
{
  Logger::setOutput(dummyOutput);
  g_logFile.reset(new LogFile("ClientLog", 500*1000*1000, false));
  launchXConnection(10);
}
