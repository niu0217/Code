/* ************************************************************************
> File Name:     ProcessInfo_test.cpp
> Author:        niu0217
> Created Time:  Sat 22 Jun 2024 02:46:39 PM CST
> Description:   
 ************************************************************************/

#include "base/ProcessInfo.h"
#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

int main()
{
  printf("pid = %d\n", ProcessInfo::pid());
  printf("uid = %d\n", ProcessInfo::uid());
  printf("euid = %d\n", ProcessInfo::euid());
  printf("start time = %s\n", ProcessInfo::startTime().toFormattedString().c_str());
  printf("hostname = %s\n", ProcessInfo::hostname().c_str());
  printf("opened files = %d\n", ProcessInfo::openedFiles());
  printf("threads = %zd\n", ProcessInfo::threads().size());
  printf("num threads = %d\n", ProcessInfo::numThreads());
  printf("status = %s\n", ProcessInfo::procStatus().c_str());
}
