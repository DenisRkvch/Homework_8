#include "IoCRegisterCommand.h"

IoCRegisterCommand::IoCRegisterCommand(IScope* scope, const std::string& str, IFunctor* f) : container(scope), key(str), resolver(f)
{

}

void IoCRegisterCommand::execute()
{
    container->add(key,resolver);
}
