/* ************************************************************************
> File Name:     Timestamp_test.cpp
> Author:        niu0217
> Created Time:  Thu 20 Jun 2024 10:13:50 AM CST
> Description:   测试时间戳中的函数
 ************************************************************************/

#include "base/Timestamp.h"
#include <vector>
#include <stdio.h>

void benchmark()
{
  const int kNumber = 1000*1000;

  // 测试创建1000000个时间戳需要耗时多少秒
  std::vector<Timestamp> stamps;
  stamps.reserve(kNumber);
  for (int i = 0; i < kNumber; ++i)
  {
    stamps.push_back(Timestamp::now());
  }
  printf("%s\n", stamps.front().toString().c_str());
  printf("%s\n", stamps.back().toString().c_str());
  printf("%f\n", timeDifference(stamps.back(), stamps.front()));

  // 统计相差100微秒的数据
  int increments[100] = { 0 };
  int64_t start = stamps.front().microSecondsSinceEpoch();
  for (int i = 1; i < kNumber; ++i)
  {
    int64_t next = stamps[i].microSecondsSinceEpoch();
    int64_t inc = next - start;
    start = next;
    if (inc < 0)
    {
      printf("reverse!\n");
    }
    else if (inc < 100)
    {
      ++increments[inc];
    }
    else
    {
      printf("big gap %d\n", static_cast<int>(inc));
    }
  }

  for (int i = 0; i < 100; ++i)
  {
    printf("%2d: %d\n", i, increments[i]);
  }
}

int main()
{
  Timestamp now(Timestamp::now());
  printf("%s\n", now.toString().c_str());
  printf("%s\n\n", now.toFormattedString().c_str());

  benchmark();
}