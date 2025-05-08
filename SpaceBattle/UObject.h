#ifndef UOBJECT_H
#define UOBJECT_H
#include <string>
#include <any>

class UObject
{
public:
    virtual ~UObject() = default;
    virtual void setProperty(const std::string&, const std::any&) = 0;
    virtual std::any getProperty(const std::string&) = 0;
};

#endif // UOBJECT_H
