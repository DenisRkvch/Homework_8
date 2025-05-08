#ifndef SCOPE_H
#define SCOPE_H

#include <map>
#include <vector>
#include "IoCCore/IFunctor.h"
#include "IoCCore/IIoCContainer.h"
#include "IScope.h"

class Scope : public IScope
{

public:
    Scope(const std::string&, IScope* = nullptr);
    Scope(const std::string&, std::map<std::string, IFunctor*>, IScope* = nullptr);
    ~Scope() override;

    const std::string& getId(void) override;
    IScope *parent() override;
    void newChild(const std::string&) override;
    void deleteChild(const std::string&) override;
    IScope* getChild(const std::string&) override;


    void add(const std::string&, IFunctor*) override;
    IFunctor *operator [](const std::string& str) override;

private:
    std::string id;
    IScope* parent_scope;
    std::vector<IScope*> child_list;
    std::map<std::string, IFunctor*> dependency_map;


    // IScope interface
public:

};

#endif // SCOPE_H
