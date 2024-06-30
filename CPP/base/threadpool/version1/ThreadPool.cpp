/* ************************************************************************
> File Name:     ThreadPool.cpp
> Author:        niu0217
> Created Time:  Sat 29 Jun 2024 06:59:57 PM CST
> Description:   
 ************************************************************************/

#pragma once

#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <string.h>
#include <atomic>
#include <vector>
#include <condition_variable>
#include <signal.h>
#include <algorithm> 
#include "TaskQueue.cpp"

using namespace std;

//线程池结构体
template<class T>
class ThreadPool 
{
public:
	ThreadPool();
	explicit ThreadPool(unsigned int min, unsigned int max);
	~ThreadPool();

	void addTask(const Task<T> &task);
	unsigned int getBusyNum(); //忙的线程个数
	unsigned int getAliveNum(); //活的线程个数
private:
	void threadExit();//线程退出
	static void* worker(void *arg);  //工作线程调用的任务函数
	static void* manager(void *arg); //管理者线程调用的任务函数

	const static unsigned int NUMBER = 2; //一次加入的线程数

	//任务队列
	TaskQueue<T> *taskQueue;

	thread *managerThread; //管理者线程

	vector<thread> workGroup; //工作线程组

	unsigned int minNum; //最小线程数
	unsigned int maxNum; //最大线程数

	atomic<unsigned int> busyNum; //忙的线程的个数
	atomic<unsigned int> liveNum; //存活的线程的个数
	atomic<unsigned int> exitNum; //要销毁的线程个数

	mutex mutexPool; //锁整个线程池
	condition_variable notEmpty; //任务队列是不是空了 利用条件变量通知

	bool isShutdown; //线程池销毁 true 
};

template<class T>
ThreadPool<T>::ThreadPool() {}

template<class T>
ThreadPool<T>::ThreadPool(unsigned int min, unsigned int max) 
{
	taskQueue = new TaskQueue<T>;
	if (nullptr == taskQueue) 
  {
		cout << "new taskQueue fail..." << endl;
		return;
	}
	workGroup.reserve(max);//分配最大容量

	minNum = min;
	maxNum = max;
	busyNum = 0;
	liveNum = min;
	exitNum = 0;

	isShutdown = false;
	//管理者线程
	managerThread = new thread(manager, this);
	//if (managerThread->joinable()) {
	//	// 管理线程是守护线程，这里可以将其分离 
	//	managerThread->detach();
	//}
	if (nullptr == managerThread) 
  {
		cout << "new managerThread fail..." << endl;
		return;
	}
	//工作线程
	for (unsigned int i = 0; i < min; ++i) 
  {
		workGroup.push_back(thread(worker, this));
	}
}

template<class T>
ThreadPool<T>::~ThreadPool() 
{
	isShutdown = true;
	//唤醒工作线程 主动关闭
	if (liveNum > 0) 
  {
		notEmpty.notify_all();
	}
	if (taskQueue) 
  {
		delete taskQueue;
	}
	if (managerThread != nullptr) 
  {
		//设置managerThread为守护线程时
		//c++运行库可以保证managerThread相关资源回收
		if (managerThread->joinable()) 
    {
			managerThread->join();
		}
		delete managerThread;
	}
	if (!workGroup.empty()) 
  {
		threadExit();
	}
	cout << "ThreadPool end!" << endl;
}

template<class T>
void * ThreadPool<T>::worker(void *arg) 
{
	cout << this_thread::get_id() << " worker thread starting..." << endl;

	ThreadPool *pool = static_cast<ThreadPool *>(arg);
	while (true) 
  {
		unique_lock<mutex> poolLock(pool->mutexPool);
    // 任务队列为空并且线程池没有被销毁
		while (0 == pool->taskQueue->taskNumber() && !pool->isShutdown) 
    {
			cout << this_thread::get_id() << " waiting task..." << endl;
			pool->notEmpty.wait(poolLock); //阻塞等待任务，唤醒后自动上锁
			//判断是否销毁线程
			if (pool->exitNum > 0 && pool->liveNum > pool->minNum) 
      {
				pool->exitNum--;
				pool->liveNum--;
				poolLock.unlock();
				cout << this_thread::get_id() << " thread end work" << endl;
				return NULL;
			}
		}
    // 线程池已经被销毁
		if (pool->isShutdown) 
    {
			poolLock.unlock();
			cout << this_thread::get_id() << " thread end work" << endl;
			return NULL;
		}
		//取出一个任务
		Task<T> task = pool->taskQueue->takeTask();
		poolLock.unlock();

		pool->busyNum++;
		task.function(task.arg);//执行函数
		delete task.arg;
		pool->busyNum--;
	}
}

template<class T>
void * ThreadPool<T>::manager(void *arg) 
{
	cout << this_thread::get_id() << " manager thread starting..." << endl;
	ThreadPool *pool = static_cast<ThreadPool *>(arg);

	int checkInterval = 3;
	while (!pool->isShutdown) 
  {
		this_thread::sleep_for(chrono::seconds(checkInterval));//每3秒检查
		unique_lock<mutex> poolLock(pool->mutexPool);
		unsigned int queueSize = pool->taskQueue->taskNumber();
		//唤醒等待任务的线程
		unsigned int sleepThreadNum = pool->liveNum - pool->busyNum;
		if (pool->busyNum < queueSize) 
    {
			while (sleepThreadNum > 0) 
      {
				pool->notEmpty.notify_one();
				sleepThreadNum--;
			}
		}
		//添加线程
		if (queueSize > pool->liveNum && pool->liveNum < pool->maxNum) 
    {
			for (unsigned int counter = 0; counter < NUMBER
				&& pool->liveNum < pool->maxNum; ++counter) 
      {
				pool->workGroup.push_back(thread(worker, pool));
				pool->liveNum++;
			}
		}
		//检查时间自适应
		//还需完善
		if ((queueSize * 2 < pool->liveNum || queueSize > 2 * pool->liveNum)
			&& checkInterval > 1) 
    {
			checkInterval--;
		}
		poolLock.unlock();
		//销毁线程
		if (pool->busyNum * 2 < pool->liveNum && pool->liveNum > pool->minNum) 
    {
			pool->exitNum = NUMBER;
			for (int i = 0; i < NUMBER; ++i) {
				pool->notEmpty.notify_one();
			}
		}
		//代表有些线程以经结束了工作，需要从容器中删除，回收资源
		if (pool->workGroup.size() > pool->liveNum) {
			pool->threadExit();
		}
	}
	return NULL;
}

template<class T>
void ThreadPool<T>::threadExit() 
{
	//清空容器里结束的线程
  // workGroup.erase(remove_if(workGroup.begin(), workGroup.end(),
  //                          [](thread &t) { return !t.joinable(); }),
  //                 workGroup.end());

	//thread::id tid = this_thread::get_id();
	for (auto it = workGroup.begin(); it != workGroup.end();)
  {
    if (it->joinable())
    {
      it->join();
      it = workGroup.erase(it);
    }
    else
    {
      it++;
    }
  }
}

template<class T>
void ThreadPool<T>::addTask(const Task<T> &task) 
{
	taskQueue->addTask(task);
}

template<class T>
unsigned int ThreadPool<T>::getBusyNum() 
{
	return this->busyNum;
}

template<class T>
unsigned int ThreadPool<T>::getAliveNum() 
{
	return this->liveNum;
}