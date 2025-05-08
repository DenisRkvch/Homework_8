#include "SoftStopExecutorCommand.h"


SoftStopExecutorCommand::SoftStopExecutorCommand(CmdExecutor *executor, ICmdQueue *queue) : cmdExecutor(executor), cmdQueue(queue)
{
}

void SoftStopExecutorCommand::execute()
{
    ICmdQueue* queue_ptr = cmdQueue;
    cmdExecutor->setRunCondition([queue_ptr](){return !queue_ptr->isEmpty();});
}
