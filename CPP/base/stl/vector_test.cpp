/* ************************************************************************
> File Name:     vector_test.cpp
> Author:        niu0217
> Created Time:  Sun 30 Jun 2024 03:54:09 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <vector>
using namespace std;

/*
 vector<Type>
 clear() 函数的作用：
      将 size 变为0，capacity 不变
      调用 Type 的析构函数 (Type 申请的内存空间没有被释放)
      依旧可以正常访问 Type 这一块内存空间
*/
class Task
{
 public:
  Task()
  {
    cout << "Task created\n";
  }
  ~Task()
  {
    cout << "Task destory\n"; 
  }
  int num = 10;
};
void testClear()
{
  vector<int> nums(10000, 42);
  cout << nums.size() << endl;     // 10000
  cout << nums.capacity() << endl; // 10000

  nums.clear();  // size 变为0  capacity 不变

  cout << nums.size() << endl;     // 0
  cout << nums.capacity() << endl; // 10000

  nums[1] = 2;   // 可以访问
  cout << nums[1] << endl;

  for (int i = 0; i < 10; i++)
  {
    cout << nums[i] << " ";  // 依旧可以访问
  }
  cout << endl;
}
void testClear2()
{
  vector<Task> tasks;
  Task t;
  for (int i = 0; i < 5; i++)
  {
    tasks.push_back(t);
  }
  tasks.clear();  // 在这里会调用析构函数
  cout << tasks.size() << endl;     // 0 
  cout << tasks.capacity() << endl; // 8
  cout << tasks[0].num << endl;  // Task 析构函数被调用，但是Task的内存并没有被释放
}
void correctClearMethod()
{
  vector<int> nums(10000, 32);
  
  vector<int>().swap(nums);
  cout <<  nums.size() << endl;
  cout <<  nums.capacity() << endl;
}

int main()
{
  // testClear();
  // correctClearMethod();
  testClear2();
}