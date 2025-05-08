#ifndef STARTEXECUTORCOMMAND_H
#define STARTEXECUTORCOMMAND_H

#include "../../ICommand.h"
#include "../CmdExecutor.h"


class StartExecutorCommand : public ICommand
{
public:
    StartExecutorCommand(CmdExecutor*);
    void execute() override;

private:
    CmdExecutor* cmdExecutor;
};

#endif // STARTEXECUTORCOMMAND_H
