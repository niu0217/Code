/* ************************************************************************
> File Name:     version1_main.cpp
> Author:        niu0217
> Created Time:  Sat 29 Jun 2024 07:01:29 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <future>
#include <thread>
#include "TaskQueue.cpp"
#include "ThreadPool.cpp"
using namespace std;

void task(void *arg) 
{
	int num = *(int *)arg;
	cout << "corrent id is: " << this_thread::get_id() << " :" << num << endl;
	this_thread::sleep_for(chrono::seconds(1));
}

void test1() 
{
	cout << "start threadPool test" << endl;
	ThreadPool<int> pool(2, 10);
	for (int i = 0; i < 100; ++i) {
		int *num = new int(i + 100);
		pool.addTask(Task<int>(task, num));
	}
	this_thread::sleep_for(chrono::seconds(15));
	for (int i = 0; i < 20; ++i) {
		int* num = new int(i + 200);
		pool.addTask(Task<int>(task, num));
	}
	this_thread::sleep_for(chrono::seconds(20));
	cout << "end threadPool test" << endl;
}

int main() 
{
	test1();
}