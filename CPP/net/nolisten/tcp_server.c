/* ************************************************************************
> File Name:     tcp_server.c
> Author:        niu0217
> Created Time:  Fri 14 Jun 2024 11:54:49 AM CST
> Description:   
 ************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
  int sockfd, ret;
  struct sockaddr_in server_addr;

  /* 服务器端创建 tcp socket 描述符 */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    fprintf(stderr, "Socket error:%s\n\a", strerror(errno));
    exit(1);
  }

  /* 服务器端填充 sockaddr 结构 */
  bzero(&server_addr, sizeof(struct sockaddr_in));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(8888);

  /* 绑定 ip + 端口 */
  ret = bind(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr));
  if (ret < 0)
  {
    fprintf(stderr, "Bind error:%s\n\a", strerror(errno));
    exit(1);
  }

  // 没有调用 listen

  sleep(1000);  // 睡眠1000s
  close(sockfd);
  return 0;
}