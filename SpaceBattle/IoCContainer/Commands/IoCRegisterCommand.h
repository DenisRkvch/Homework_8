#ifndef IOCREGISTERCOMMAND_H
#define IOCREGISTERCOMMAND_H
#include "../../ICommand.h"
#include "../Scope.h"
#include "../IoCCore/IoC.h"
#include <map>


class IoCRegisterCommand : public ICommand
{
public:
    IoCRegisterCommand(IScope*, const std::string&, IFunctor*);
    void execute() override;

private:
    IScope* container;
    std::string key;
    IFunctor* resolver;
};

#endif // IOCREGISTERCOMMAND_H
