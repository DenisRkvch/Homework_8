#ifndef IIOCCONTAINER_H
#define IIOCCONTAINER_H

#include <string>
#include "IFunctor.h"

class IIoCContainer
{
public:
    virtual IFunctor* operator[](const std::string& str) = 0;
    virtual ~IIoCContainer() = default;

};
#endif // IIOCCONTAINER_H
