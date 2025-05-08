#ifndef IOCCHENGECURRENTSCOPECOMMAND_H
#define IOCCHENGECURRENTSCOPECOMMAND_H

#include "../../ICommand.h"
#include "../Scope.h"

class IoCChangeCurrentScopeCommand : public ICommand
{

public:
    IoCChangeCurrentScopeCommand(const std::string&, IScope**);
    void execute() override;

private:
    std::string id;
    IScope** curScope_ptr;
};

#endif // IOCCHENGECURRENTSCOPECOMMAND_H
