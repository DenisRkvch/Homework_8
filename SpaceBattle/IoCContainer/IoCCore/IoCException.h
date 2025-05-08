#ifndef IOCEXCEPTION_H
#define IOCEXCEPTION_H
#include <stdexcept>
#include <string>

class IoCException : public std::logic_error
{
public:
    IoCException(const std::string& message = "no description") noexcept;
};

#endif // IOCEXCEPTION_H
