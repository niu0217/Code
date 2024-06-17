/* ************************************************************************
> File Name:     NiuString.cpp
> Author:        niu0217
> Created Time:  Mon 17 Jun 2024 07:35:51 PM CST
> Description:   
 ************************************************************************/

#include "NiuString.h"

NiuString::NiuString(const char *str)
{
  if (str == nullptr)
  {
    data_ = new char[1];
    *data_ = '\0';
    len_ = 0;
    cout << "Default constructor\n";
  }
  else
  {
    len_ = strlen(str);
    data_ = new char[len_ + 1];
    strcpy(data_, str);
    cout << "Pass argument constructor\n";
  }
}

NiuString::~NiuString()
{
  delete data_;
  data_ = nullptr;
  len_ = 0;
}

NiuString::NiuString(const NiuString &rhs)
{
  len_ = strlen(rhs.data_);
  data_ = new char[len_ + 1];
  strcpy(data_, rhs.data_);
  cout << "Copy constructor\n";
}

NiuString& NiuString::operator=(const NiuString &rhs)
{
  if(this != &rhs)
  {
    if(! data_)
    {
      delete data_;
    }
    len_ = rhs.len_;
    data_ = new char[len_ + 1];
    strcpy(data_, rhs.data_);
  }
  cout << "Copy assignment\n";
  return *this;
}

NiuString::NiuString(NiuString &&rhs)
{
  data_ = rhs.data_;
  len_ = rhs.len_;
  rhs.data_ = nullptr;
  rhs.len_ = 0;
  cout << "Move constructor\n";
}

NiuString& NiuString::operator=(NiuString &&rhs)
{
  if(this != &rhs)
  {
    delete data_;
    len_ = 0;
    data_ = rhs.data_;
    len_ = rhs.len_;
    rhs.data_ = nullptr;
    rhs.len_ = 0;
  }
  cout << "Move assignment\n";
  return *this;
}

void NiuString::printData()
{
  cout << data_ << endl;
  cout << len_ << endl << endl;
}