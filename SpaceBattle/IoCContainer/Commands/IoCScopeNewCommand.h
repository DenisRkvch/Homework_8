#ifndef IOCSCOPENEWCOMMAND_H
#define IOCSCOPENEWCOMMAND_H
#include "../../ICommand.h"
#include "../Scope.h"

class IoCScopeNewCommand : public ICommand
{

public:
    IoCScopeNewCommand(const std::string&, IScope*);

    void execute() override;

private:
    std::string id;
    IScope* curScope;
};

#endif // IOCSCOPENEWCOMMAND_H
