#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define LOCAL_IP_ADDR (0x7F000001) // IP 127.0.0.1
#define LOCAL_TCP_PORT (34567)     // 端口

int main(void)
{
  struct sockaddr_in local, peer;
  int ret;
  char buf[128];
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  memset(&local, 0, sizeof(local));
  memset(&peer, 0, sizeof(peer));

  local.sin_family = AF_INET;
  local.sin_port = htons(LOCAL_TCP_PORT);
  local.sin_addr.s_addr = htonl(LOCAL_IP_ADDR);

  peer = local;

  int flag = 1;
  ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
  if (ret == -1)
  {
    printf("Fail to setsocket SO_REUSEADDR: %s\n", strerror(errno));
    exit(1);
  }

  ret = bind(sock, (const struct sockaddr *)&local, sizeof(local));
  if (ret)
  {
    printf("Fail to bind: %s\n", strerror(errno));
    exit(1);
  }

  ret = connect(sock, (const struct sockaddr *)&peer, sizeof(peer));
  if (ret)
  {
    printf("Fail to connect myself: %s\n", strerror(errno));
    exit(1);
  }

  printf("Connect to myself successfully\n");

  // 发送数据
  strcpy(buf, "Hello, myself~");
  send(sock, buf, strlen(buf), 0);

  memset(buf, 0, sizeof(buf));

  // 接收数据
  recv(sock, buf, sizeof(buf), 0);
  printf("Recv the msg: %s\n", buf);

  sleep(1000);
  close(sock);
  return 0;
}
