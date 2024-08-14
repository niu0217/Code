/* ************************************************************************
> File Name:     String.cpp
> Author:        niu0217
> Created Time:  Sun 11 Aug 2024 10:23:22 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <cstring>
using namespace std;

class String
{
public:
  String();
  ~String();

  String(const char *str);
  String(const String &rhs);
  String& operator=(const String &rhs);
  String(String &&rhs);
  String& operator=(String &&rhs);

  void printData()
  {
    cout << m_data << endl;
  }

public:
  char *m_data;
  int m_len;
};

String::String()
{
  m_data = nullptr;
  m_len = 0;
}

String::~String()
{
  if (m_data)
  {
    delete m_data;
    m_data = nullptr;
  }
  m_len = 0;
}

String::String(const char *str)
{
  if (str == nullptr)
  {
    m_data = new char[1];
    m_data[0] = '\0';
    m_len = 0;
  }
  else
  {
    m_len = strlen(str);
    m_data = new char[m_len + 1];
    strcpy(m_data, str);
  }
}

String::String(const String &rhs)
{

}

String& String::operator=(const String &rhs)
{

}

String::String(String &&rhs)
{

}

String& String::operator=(String &&rhs)
{

}

int main()
{

}