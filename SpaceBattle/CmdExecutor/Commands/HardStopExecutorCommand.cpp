#include "HardStopExecutorCommand.h"

HardStopExecutorCommand::HardStopExecutorCommand(CmdExecutor* executor) : cmdExecutor(executor)
{
}

void HardStopExecutorCommand::execute()
{
    cmdExecutor->setRunCondition([](){return false;});
}
