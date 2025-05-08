#include "IoCScopeNewCommand.h"
#include "../IoCCore/IoC.h"
#include "IoCRegisterCommand.h"

IoCScopeNewCommand::IoCScopeNewCommand(const std::string& str, IScope* cur_ptr) : id(str), curScope(cur_ptr)
{
}

void IoCScopeNewCommand::execute()
{   
    curScope->newChild(id);
}
