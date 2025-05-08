#include "CmdExecutor.h"
#include "../CommandException.h"
#include "../IoCContainer/IoCCore/IoC.h"


CmdExecutor::CmdExecutor(ICmdQueue* q): cmdQueue(q)
{ 
}

void CmdExecutor::run()
{
    actionOnStart();

    while(runCondition())
    {        
        ICommand* cmd = cmdQueue->pop();

        try {
            cmd->execute();
            delete cmd;

        } catch (std::exception* e) {
            ICommand* handleCommand = IoC::Resolve<ICommand*>("HandleException", cmd, e);
            handleCommand->execute();
            delete handleCommand;
        }
    }

    actionOnFinish();
}

void CmdExecutor::setRunCondition(std::function<bool ()> newAction)
{
    runCondition = newAction;
}

void CmdExecutor::setActionOnStart(std::function<void()> newAction)
{
    actionOnStart = newAction;
}

void CmdExecutor::setActionOnFinish(std::function<void()> newAction)
{
    actionOnFinish = newAction;   
}

