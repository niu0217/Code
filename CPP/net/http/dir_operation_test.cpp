/* ************************************************************************
> File Name:     dir_operation_test.cpp
> Author:        niu0217
> Created Time:  Sun 30 Jun 2024 03:22:39 PM CST
> Description:   测试目录操作
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

// 解析目录
// 参考：https://subingwen.cn/linux/directory/
// 解析单层目录
int singleDir()
{
  const char *dirPath = "./data/";
  // 1. 打开目录
  DIR* dir = opendir(dirPath);
  if(dir == NULL)
  {
    perror("opendir");
    return -1;
  }
  // 2. 遍历当前目录中的文件
  int count = 0;
  while(1)
  {
    struct dirent* ptr = readdir(dir);
    if(ptr == NULL)
    {
      printf("目录读完了...\n");
      break;
    }
    // 读到了一个文件
    // 判断文件类型
    if(ptr->d_type == DT_REG)
    {
      char* p = strstr(ptr->d_name, ".mp3");
      if(p != NULL && *(p+4) == '\0')
      {
        count++;
        printf("file %d: %s\n", count, ptr->d_name);
      }
    }
  }

  printf("%s 目录中mp3文件的个数: %d\n", dirPath, count);

  // 关闭目录
  closedir(dir);
  return 0;
}

// 遍历多层目录
int getMp3Num(const char *path)
{
  // 1. 打开目录
  DIR* dir = opendir(path);
  if(dir == NULL)
  {
    perror("opendir");
    return 0;
  }
  // 2. 遍历当前目录
  struct dirent* ptr = NULL;
  int count = 0;
  while((ptr = readdir(dir)) != NULL)
  {
    // 如果是目录 . .. 跳过不处理
    if(strcmp(ptr->d_name, ".") == 0 ||
       strcmp(ptr->d_name, "..") == 0)
    {
      continue;
    }
    // 假设读到的当前文件是目录
    if(ptr->d_type == DT_DIR)
    {
      // 目录
      char newPath[1024];
      sprintf(newPath, "%s/%s", path, ptr->d_name);
      // 读当前目录的子目录
      count += getMp3Num(newPath);
    }
    else if(ptr->d_type == DT_REG)
    {
      // 普通文件
      char* p = strstr(ptr->d_name, ".mp3");
      // 判断文件后缀是不是 .mp3
      if(p != NULL && *(p+4) == '\0')
      {
        count++;
        printf("%s/%s\n", path, ptr->d_name);
      }
    }
  }

  closedir(dir);
  return count;
}

// ******************** 使用scandir操作目录 ******************************
// 文件过滤函数
int isMp3(const struct dirent *ptr)
{
  if(ptr->d_type == DT_REG)
  {
    const char* p = strstr(ptr->d_name, ".mp3");
    if(p != NULL && *(p+4) == '\0')
    {
      return 1;
    }
  }
  return 0;
}
void testScandir()
{
  struct dirent **namelist = NULL;
  const char *path = "./data";
  int num = scandir(path, &namelist, isMp3, alphasort);
  for(int i = 0; i < num; ++i)
  {
    printf("file %d: %s\n", i, namelist[i]->d_name);
    free(namelist[i]);
  }
  free(namelist);
}


int main()
{
  // singleDir();
  // getMp3Num("./data");
  testScandir();
}