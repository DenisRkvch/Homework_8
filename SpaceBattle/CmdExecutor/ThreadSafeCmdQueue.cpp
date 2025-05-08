#include "ThreadSafeCmdQueue.h"

ThreadSafeCmdQueue::ThreadSafeCmdQueue()
{
}

ThreadSafeCmdQueue::~ThreadSafeCmdQueue()
{
}

void ThreadSafeCmdQueue::push(ICommand* cmd)
{
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(cmd);
    condVar_.notify_one();
}

ICommand* ThreadSafeCmdQueue::pop()
{
    std::unique_lock<std::mutex> lock(mutex_);
    condVar_.wait(lock, [this] { return !queue_.empty(); });

    ICommand* cmd = queue_.front();
    queue_.pop();
    return cmd;
}

bool ThreadSafeCmdQueue::isEmpty()
{
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}