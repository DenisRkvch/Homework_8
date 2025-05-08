#ifndef ISCOPE_H
#define ISCOPE_H

#include "IoCCore/IIoCContainer.h"

class IScope : public IIoCContainer
{
public:
    virtual const std::string& getId() = 0;
    virtual void add(const std::string&, IFunctor*) = 0;
    virtual IScope* parent() = 0;
    virtual void newChild(const std::string&) = 0;
    virtual void deleteChild(const std::string&) = 0;
    virtual IScope* getChild(const std::string&) = 0;

};
#endif // ISCOPE_H
