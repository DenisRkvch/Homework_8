#include "Scope.h"
#include "IoCCore/IoCException.h"

Scope::Scope(const std::string& str, IScope* parent): id(str), parent_scope(parent)
{}

Scope::Scope(const std::string& str, std::map<std::string, IFunctor*> map, IScope* parent): id(str), parent_scope(parent), dependency_map(map)
{}

Scope::~Scope()
{
    for(auto child : child_list)
    {
        delete child;
    }

    auto iter = dependency_map.cbegin();
    while(iter!=dependency_map.end())
    {
        delete iter->second;
        ++iter;
    }
}

const std::string& Scope::getId()
{
    return id;
}

void Scope::add(const std::string& str, IFunctor* f)
{
    if (dependency_map.find(str) != dependency_map.end())
    {
        throw IoCException();
    }
    dependency_map[str] = f;
}

IScope * Scope::parent()
{
    return parent_scope;
}

void Scope::newChild(const std::string& child_id)
{
    // проверить что id свободно
    if(id == child_id)
    {
        throw IoCException("the id:<" + child_id + "> already belongs to current scope");
    }
    if (parent_scope != nullptr)
    {
        if (parent_scope->getId() == child_id)
        {
            throw IoCException("the id:<" + child_id + "> already belongs to parent scope");
        }
    }   
    for(auto child : child_list)
    {
        if(child->getId() == child_id)
        {
            throw IoCException("the id:<" + child_id + "> already belongs to one of the child scopes");
        }
    }

    child_list.push_back(new Scope(child_id, this));
}

void Scope::deleteChild(const std::string& child_id)
{
    auto iter = child_list.cbegin();

    while(iter!=child_list.end())
    {
        if((*iter)->getId() == child_id)
        {
            delete *iter;

            child_list.erase(iter);

            return;
        }
        ++iter;
    }

    // если узла не существует
    throw IoCException("child with id:<" + child_id + "> not exist in current scope");
}

IScope* Scope::getChild(const std::string& child_id)
{
    for(IScope* child : child_list)
    {
        if(child->getId() == child_id)
        {
            return child;
        }
    }
    // если узла не существует
    throw IoCException("child with id:<" + child_id + "> not exist in current scope");
}

IFunctor * Scope::operator [](const std::string& str)
{
    if(dependency_map.find(str) != dependency_map.end())
    {
        return dependency_map[str];
    }
    else
    {
        if(parent_scope != nullptr)
        {
            return (*parent_scope)[str];
        }
        else
        {
            throw IoCException("dependency for <" + str + "> is not found");
        }
    }

}


