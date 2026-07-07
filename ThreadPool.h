#pragma once

#include <queue>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <winsock2.h>

class Database;

class ThreadPool
{
private:
    std::queue<SOCKET> clientQueue;

    std::mutex queueMutex;

    std::condition_variable cv;

    std::vector<std::thread> workers;
    bool stop;
    Database &db;

    void workerThread();

public:
    ThreadPool(int numThreads, Database &database);
   ~ThreadPool();
    void addClient(SOCKET client);
};