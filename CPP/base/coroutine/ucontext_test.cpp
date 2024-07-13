/* ************************************************************************
> File Name:     ucontext_test.cpp
> Author:        niu0217
> Created Time:  Sun 07 Jul 2024 04:56:06 PM CST
> Description:   
 ************************************************************************/

#include <ucontext.h>
#include <iostream>
#include <atomic>

// caller --> context1 --> function1
// callee --> context2 --> function2
void function1(ucontext_t* caller, ucontext_t* callee) 
{
  std::cout << "Function 1: Before switch" << std::endl;
  // 恢复callee的上下文，同时将当前的上下文保存到caller中
  // 也就是会跳转到 context2 绑定的函数function2中
  swapcontext(caller, callee);
  std::cout << "Function 1: After switch" << std::endl;
  swapcontext(caller, callee);
}

// caller --> context2 --> function2
// callee --> context1 --> function1
void function2(ucontext_t* caller, ucontext_t* callee) 
{
  std::cout << "Function 2: Before switch" << std::endl;
  swapcontext(caller, callee);
  std::cout << "Function 2: After switch" << std::endl;
}

int main() 
{
  ucontext_t context1;
  ucontext_t context2;
  char stack1[8192];
  char stack2[8192];

  // 获取当前上下文
  getcontext(&context1);
  getcontext(&context2);

  context1.uc_link = nullptr;
  context1.uc_stack.ss_sp = stack1;
  context1.uc_stack.ss_size = sizeof(stack1);
  // 将 context1 与函数 function1 绑定
  makecontext(&context1, (void (*)())function1, 2, &context1, &context2);

  context2.uc_link = &context1;  // 当 context2 执行完毕后会返回到 context1
  context2.uc_stack.ss_sp = stack2;
  context2.uc_stack.ss_size = sizeof(stack2);
  // 将 context2 与函数 function2 绑定
  makecontext(&context2, (void (*)())function2, 2, &context2, &context1);

  // 跳转到与context1绑定的函数function1
  setcontext(&context1);

  return 0;
}