#ifndef INTERPRETCOMMAND_H
#define INTERPRETCOMMAND_H
#include "../ICommand.h"
#include <string>
#include "../include/json.hpp"

using json = nlohmann::json;

class InterpretCommand : public ICommand
{
public:
    InterpretCommand(const std::string& obj_id, const std::string& op_id, const json& jargs);

    // ICommand interface
    void execute() override;

private:
    std::string object_id;
    std::string operation_id;
    json args;
};

#endif // INTERPRETCOMMAND_H
