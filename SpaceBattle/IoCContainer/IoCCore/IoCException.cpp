#include "IoCException.h"

IoCException::IoCException(const std::string& str) noexcept : logic_error("IoC Exception: " + str)
{    
}

