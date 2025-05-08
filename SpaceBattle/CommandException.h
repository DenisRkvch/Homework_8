#ifndef COMMANDEXCEPTION_H
#define COMMANDEXCEPTION_H

#include <stdexcept>

class CommandException : public std::logic_error
{
public:
    CommandException(const std::string& what_arg = "no description") noexcept;
};

#endif
