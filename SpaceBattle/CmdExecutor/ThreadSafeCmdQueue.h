#ifndef THREADSAFECMDQUEUE_H
#define THREADSAFECMDQUEUE_H

#include "ICmdQueue.h"
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadSafeCmdQueue : public ICmdQueue
{
public:
    ThreadSafeCmdQueue();
    ~ThreadSafeCmdQueue() override;

    void push(ICommand* cmd) override;
    ICommand* pop() override;
    bool isEmpty() override;

private:
    std::queue<ICommand*> queue_;
    mutable std::mutex mutex_;
    std::condition_variable condVar_;
    std::atomic<bool> deleted_{ false };
};

#endif // THREADSAFECMDQUEUE_H