#include "IoCChangeCurrentScopeCommand.h"
#include "../IoCCore/IoCException.h"

IoCChangeCurrentScopeCommand::IoCChangeCurrentScopeCommand(const std::string& str, IScope** scope_ptr) : id(str), curScope_ptr(scope_ptr)
{
}

void IoCChangeCurrentScopeCommand::execute()
{
    // поиск по потомкам
    try {

        *curScope_ptr = (*curScope_ptr)->getChild(id);

    } catch (IoCException& e) {

        // поиск в предке
        if ((*curScope_ptr)->parent() != nullptr)
        {
            // id предка
            if ((*curScope_ptr)->parent()->getId() == id)
            {
                *curScope_ptr = (*curScope_ptr)->parent();
                return;
            }
            else
            {
                // поиск по потомкам предка
                try {

                    *curScope_ptr = (*curScope_ptr)->parent()->getChild(id);
                    return;

                } catch (IoCException& e) {

                    throw IoCException("scope with id:<" + id + "> not visible in current area");
                }
            }
        }
        else if((*curScope_ptr)->getId() == id) // id текущего узла
        {
            return;
        }

        throw IoCException("scope with id:<" + id + "> not visible in current area");

    }
}
