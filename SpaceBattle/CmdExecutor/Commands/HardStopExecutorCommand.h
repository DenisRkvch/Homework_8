#ifndef HARDSTOPEXECUTORCOMMAND_H
#define HARDSTOPEXECUTORCOMMAND_H

#include "../../ICommand.h"
#include "../CmdExecutor.h"


class HardStopExecutorCommand : public ICommand
{
public:
    HardStopExecutorCommand(CmdExecutor* executor);

    void execute() override;


private:
    CmdExecutor* cmdExecutor;
};

#endif // HARDSTOPEXECUTORCOMMAND_H
