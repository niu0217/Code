/* ************************************************************************
> File Name:     TimeZone_util.cpp
> Author:        niu0217
> Created Time:  Thu 20 Jun 2024 04:15:31 PM CST
> Description:   
 ************************************************************************/

#include "base/TimeZone.h"

#include <assert.h>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>

#include <string>

void printUtcAndLocal(int64_t utc, TimeZone local)
{
  printf("Unix Time: %" PRId64 "\n", utc);
  printf("UTC:       %s\n", TimeZone::toUtcTime(utc).toIsoString().c_str());
  int utcOffset = 0;
  printf("Local:     %s", local.toLocalTime(utc, &utcOffset).toIsoString().c_str());
  printf(" %+03d%02d\n", utcOffset / 3600, utcOffset % 3600 / 60);
}

int main(int argc, char* argv[])
{
  TimeZone local = TimeZone::loadZoneFile("/etc/localtime");
  if (argc <= 1)
  {
    time_t now = ::time(NULL);
    printUtcAndLocal(now, local);
    return 0;
  }
}