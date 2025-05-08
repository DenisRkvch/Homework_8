#ifndef IOCSCOPEDELETECOMMAND_H
#define IOCSCOPEDELETECOMMAND_H
#include "../../ICommand.h"
#include "../IScope.h"

class IoCScopeDeleteCommand : public ICommand
{
public:

    IoCScopeDeleteCommand(IScope**curScope_ptr);
    void execute() override;

private:
    IScope** curScope_ptr;
};

#endif // IOCSCOPEDELETECOMMAND_H
