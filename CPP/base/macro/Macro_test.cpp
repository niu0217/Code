/* ************************************************************************
> File Name:     Macro_test.cpp
> Author:        niu0217
> Created Time:  Sat 13 Jul 2024 10:07:27 AM CST
> Description:   
 ************************************************************************/

// g++ -g -o Macro_test Macro_test.cpp

// g++ -E Macro_test.cpp -o Macro_test.i

#include <iostream>
#include <vector>
using namespace std;

namespace testcase1
{

class LogLevel
{
public:
  enum Level
  {
    UNKNOW = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5
  };
  static const char* ToString(LogLevel::Level level);

  static LogLevel::Level FromString(const std::string& str);
};

// #name 会把name转化为字符串形式
const char* LogLevel::ToString(LogLevel::Level level)
{
  switch(level)
  {
#define XX(name) \
  case LogLevel::name: \
    return #name; \
    break;

  XX(DEBUG)
  XX(INFO)
  XX(WARN)
  XX(ERROR)
  XX(FATAL)
#undef XX
  default:
    return "UNKNOW";
  }
  return "UNKNOW";
}

LogLevel::Level LogLevel::FromString(const std::string& str)
{
#define XX(level, v) \
  if(str == #v) { \
      return LogLevel::level; \
  }
  
  XX(DEBUG, debug);
  XX(INFO, info);
  XX(WARN, warn);
  XX(ERROR, error);
  XX(FATAL, fatal);

  XX(DEBUG, DEBUG);
  XX(INFO, INFO);
  XX(WARN, WARN);
  XX(ERROR, ERROR);
  XX(FATAL, FATAL);
  return LogLevel::UNKNOW;
#undef XX
}

void test()
{
  LogLevel loglevel;
  cout << loglevel.ToString(LogLevel::INFO) << endl;
  cout << loglevel.FromString("DEBUG") << endl;
}

}  // testcase1

int main()
{
  testcase1::test();
}