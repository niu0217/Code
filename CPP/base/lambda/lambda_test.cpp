/* ************************************************************************
> File Name:     lambda_test.cpp
> Author:        niu0217
> Created Time:  Mon 17 Jun 2024 07:13:01 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/// 作为vector的sort函数的参数
void sortVector()
{
  vector<int> nums = { 1, 4, 3, 0, -1, 200, 10, -400 };
  sort(nums.begin(), nums.end(),
       [](int a, int b) { return a > b; });
  for(int num : nums)
  {
    cout << num << " ";
  }
  cout << endl;
}

/// 测试值捕获和引用捕获
void captureTest()
{
  int number = 10;

  auto lambdaRef = [&number]() {
    number++;
  };
  lambdaRef();
  cout << number << endl;  // 11

  // 必须加mutable，如果想要修改lambda表达式中变量的值
  // 只对值捕获起效
  auto lambdaVal = [number]() mutable {
    number++;
    cout << "lambdaVal: " << number << endl; // 12
  };
  lambdaVal();
  cout << number << endl; // 11
}

int main()
{
  // sortVector();
  captureTest();
}