/* ************************************************************************
> File Name:     boost_coroutine2_test.cpp
> Author:        niu0217
> Created Time:  Sun 07 Jul 2024 03:42:56 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <boost/coroutine2/all.hpp>

using namespace boost::coroutines2;

namespace TESTCASE1
{

// 调用者可以使用 pull_type 控制协程的执行，并在需要时传递参数给协程，
// 而协程内部则可以使用 push_type 来产生结果或与调用者进行双向通信。

void simpleCoroutine(coroutine<void>::push_type& yield)
{
  for (int i = 0; i < 5; ++i) 
  {
    std::cout << "In coroutine, i = " << i << std::endl;
    yield();
  }
}

void test()
{
  coroutine<void>::pull_type myCoroutine(simpleCoroutine);
  
  for (int i = 0; i < 5; ++i)
  {
    std::cout << "In main function, i = " << i << std::endl;
    myCoroutine();
  }
}

}  // TESTCASE1

int main() 
{
  TESTCASE1::test();
}