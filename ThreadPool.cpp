#include "ThreadPool.h"
#include "Database.h"
#include "ClientHandler.h"

ThreadPool::ThreadPool(int numThreads, Database &database)
    : db(database),stop(false)
{
    for (int i = 0; i < numThreads; i++)
    {
        workers.emplace_back(
            &ThreadPool::workerThread,
            this
        );
    }
}
ThreadPool::~ThreadPool()
{
    {
std::lock_guard<std::mutex> lock(queueMutex);
    stop = true;
    }cv.notify_all();
for(std::thread& worker : workers)
{
    if(worker.joinable())
    {
        worker.join();
    }
}
}
void ThreadPool::workerThread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        cv.wait(lock, [this]()
        {
            return stop||!clientQueue.empty();
        });
         if(stop && clientQueue.empty())
            {
                return;
            }

        SOCKET client = clientQueue.front();

        clientQueue.pop();

        lock.unlock();

        handleClient(client, db);
    }
}

void ThreadPool::addClient(SOCKET client)
{
    std::lock_guard<std::mutex> lock(queueMutex);

    clientQueue.push(client);

    cv.notify_one();
}