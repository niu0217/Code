/* ************************************************************************
> File Name:     NiuString.h
> Author:        niu0217
> Created Time:  Mon 17 Jun 2024 07:36:09 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <string.h>
using namespace std;

class NiuString
{
 public:
  NiuString(const char *str = nullptr);
  ~NiuString();

  NiuString(const NiuString &rhs);
  NiuString& operator=(const NiuString &rhs);
  NiuString(NiuString &&rhs);
  NiuString& operator=(NiuString &&rhs);

  void printData();

private:
  char *data_;
  int len_;
};