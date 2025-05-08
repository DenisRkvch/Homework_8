#include "IoCScopeDeleteCommand.h"
#include "../IoCCore/IoCException.h"

IoCScopeDeleteCommand::IoCScopeDeleteCommand(IScope** scope_ptr): curScope_ptr(scope_ptr)
{

}

void IoCScopeDeleteCommand::execute()
{
    IScope* parent = (*curScope_ptr)->parent();

    if(parent != nullptr)
    {
        parent->deleteChild((*curScope_ptr)->getId());
        *curScope_ptr = parent;
    }
    else
    {
        throw IoCException("the root scope can't be deleted");
    }
}
