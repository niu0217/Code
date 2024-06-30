/* ************************************************************************
> File Name:     version3_main.cpp
> Author:        niu0217
> Created Time:  Sat 29 Jun 2024 10:23:07 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <unistd.h>
#include "TaskQueue.h"
#include "ThreadPool.h"
using namespace std;

void task(void *arg) 
{
	int num = *(int *)arg;
	cout << "corrent id is: " << pthread_self() << " :" << num << endl;
	sleep(1);
}

void test1() 
{
	cout << "start threadPool test" << endl;
	ThreadPool pool(2, 10);
	for (int i = 0; i < 100; ++i) 
  {
		int *num = new int(i + 100);
		pool.addTask(Task(task, num));
	}
	sleep(15);
	for (int i = 0; i < 20; ++i) 
  {
		int* num = new int(i + 200);
		pool.addTask(Task(task, num));
	}
	sleep(20);
	cout << "end threadPool test" << endl;
}

int main() 
{
	test1();
}
