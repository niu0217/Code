/* ************************************************************************
> File Name:     ThreadPool.h
> Author:        niu0217
> Created Time:  Sat 29 Jun 2024 10:19:00 PM CST
> Description:   
 ************************************************************************/

#pragma once

#include "TaskQueue.h"

class ThreadPool
{
public:
    ThreadPool(int min, int max);
    ~ThreadPool();

    void addTask(Task task);
    int getBusyNumber();
    int getAliveNumber();

private:
    static void* worker(void* arg);  // 工作的线程的任务函数
    static void* manager(void* arg); // 管理者线程的任务函数
    void threadExit();

private:
    pthread_mutex_t m_lock;
    pthread_cond_t m_notEmpty;
    pthread_t* m_threadIDs;
    pthread_t m_managerID;
    TaskQueue* m_taskQ;
    int m_minNum;
    int m_maxNum;
    int m_busyNum;
    int m_aliveNum;
    int m_exitNum;
    bool m_shutdown = false;
};