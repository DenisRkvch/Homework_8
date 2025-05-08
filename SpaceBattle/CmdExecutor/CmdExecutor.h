#ifndef CMDEXECUTOR_H
#define CMDEXECUTOR_H
#include <functional>
#include "ICmdQueue.h"

class CmdExecutor
{
public:
    CmdExecutor(ICmdQueue*);
    void run();
    void setRunCondition(std::function<bool(void)>);
    void setActionOnStart(std::function<void(void)>);
    void setActionOnFinish(std::function<void(void)>);


private:
    ICmdQueue* cmdQueue;
    std::function<bool(void)> runCondition = [](){return true;};
    std::function<void(void)> actionOnStart = []() {};
    std::function<void(void)> actionOnFinish = []() {};

};

#endif // CMDEXECUTOR_H
