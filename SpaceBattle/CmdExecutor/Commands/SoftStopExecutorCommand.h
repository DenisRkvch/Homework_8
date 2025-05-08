#ifndef SOFTSTOPEXECUTORCOMMAND_H
#define SOFTSTOPEXECUTORCOMMAND_H

#include "../../ICommand.h"
#include "../ICmdQueue.h"
#include "../CmdExecutor.h"

class SoftStopExecutorCommand : public ICommand
{
public:
    SoftStopExecutorCommand(CmdExecutor* executor, ICmdQueue* queue);
    void execute() override;

private:
    CmdExecutor* cmdExecutor;
    ICmdQueue* cmdQueue;
};

#endif // SOFTSTOPEXECUTORCOMMAND_H


