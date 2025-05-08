#include "CommandException.h"

CommandException::CommandException(const std::string& what_arg) noexcept : logic_error("Command Exception: " + what_arg)
{
}
