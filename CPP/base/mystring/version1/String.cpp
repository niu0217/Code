/* ************************************************************************
> File Name:     String.cpp
> Author:        niu0217
> Created Time:  Sat 10 Aug 2024 04:11:37 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <cstring>
using namespace std;

class String
{
public:
  struct Data
  {
    char buffer[24];
    Data()
    {
      buffer[0] = '\0';
    }
  };

public:
  String()
    : m_data(new Data)
  {}

  String(const char *str)
    : m_data(new Data)
  {
    cout << "转换构造函数\n";
    set(str);
  }

  ~String()
  {
    delete m_data;
  }

  String(const String &rhs)
    : m_data(new Data)
  {
    cout << "拷贝构造函数\n";
    strcpy(m_data->buffer, rhs.m_data->buffer);
  }

  String& operator=(const String &rhs)
  {
    cout << "拷贝赋值函数\n";
    if (this == &rhs)
    {
      return *this;
    }
    delete m_data;
    m_data = new Data;
    strcpy(m_data->buffer, rhs.m_data->buffer);
    return *this;
  }

  String(String &&rhs)
  {
    cout << "移动构造函数\n";
    m_data = rhs.m_data;
    rhs.m_data = nullptr;
  }

  String& operator=(String &&rhs)
  {
    cout << "移动赋值函数\n";
    if (this == &rhs)
    {
      return *this;
    }
    delete m_data;
    m_data = rhs.m_data;
    rhs.m_data = nullptr;
    return *this;
  }

  /// @brief 转换函数，将String转换为const char*
  operator const char*() const
  {
    cout << "转换函数 将String转换为const char*\n";
    return m_data->buffer;
  }

public:
  const char* c_str() const
  {
    return m_data->buffer;
  }

  void set(const char* str)
  {
    strncpy(m_data->buffer, str, sizeof(m_data->buffer) - 1);
    m_data->buffer[sizeof(m_data->buffer) - 1] = '\0';
  }

public:
  Data *m_data;
};

int main()
{
  const char* str = "nihao";
  String s1("AAAA");   // 转换构造函数
  cout << s1.c_str() << endl << endl;

  String s2(s1);  // 拷贝构造函数
  cout << s2.c_str() << endl << endl;

  String s3;
  s3 = s2;  // 拷贝赋值函数
  cout << s3.c_str() << endl << endl;

  const char *str2 = s3;  // 转换函数
  cout << str2 << endl << endl;

  String s5(move(s3));  // 移动构造函数
  cout << s5.c_str() << endl << endl;

  String s6;
  s6 = move(s2);  // 移动赋值函数
  cout << s6.c_str() << endl << endl;
}