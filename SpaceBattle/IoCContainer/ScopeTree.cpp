#include "ScopeTree.h"
#include "../ICommand.h"
#include "Commands/IoCRegisterCommand.h"
#include "Commands/IoCScopeNewCommand.h"
#include "Commands/IoCScopeDeleteCommand.h"
#include "Commands/IoCChangeCurrentScopeCommand.h"
#include "IoCCore/IoC.h"
#include <string>

ScopeTree ScopeTree::g_scopeTree;

Scope ScopeTree::root_scope("Root", std::map<std::string, IFunctor*>
{
    {"IoC.Register",        New_Resolver((std::function<ICommand*(std::string, IFunctor*)>)[](const std::string& str, IFunctor* f) -> ICommand*{
                                return new IoCRegisterCommand(curScopePtr, str, f);
                            })},
    {"IoC.Scope.New",       New_Resolver((std::function<ICommand*(std::string)>)[](const std::string& id) -> ICommand*{
                                return new IoCScopeNewCommand(id, curScopePtr);
                            })},
    {"IoC.Scope.Delete",    New_Resolver((std::function<ICommand*()>)[]() -> ICommand*{
                                return new IoCScopeDeleteCommand(&curScopePtr);
                            })},
    {"IoC.Scope.Current",   New_Resolver((std::function<ICommand*(std::string)>)[](const std::string& id) -> ICommand*{
                                return new IoCChangeCurrentScopeCommand(id, &curScopePtr);
                            })}
});

thread_local IScope* ScopeTree::curScopePtr = &root_scope;

const std::string &ScopeTree::getCurScopeId()
{
    return curScopePtr->getId();
}

IFunctor* ScopeTree::operator [](const std::string& str)
{
    return (*curScopePtr)[str];
}




