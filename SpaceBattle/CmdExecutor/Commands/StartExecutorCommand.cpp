#include "StartExecutorCommand.h"
#include <thread>

StartExecutorCommand::StartExecutorCommand(CmdExecutor * executor): cmdExecutor(executor)
{
}

void StartExecutorCommand::execute()
{
    new std::thread (&CmdExecutor::run, cmdExecutor);
}
