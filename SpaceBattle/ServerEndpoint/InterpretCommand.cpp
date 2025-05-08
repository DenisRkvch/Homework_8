#include "InterpretCommand.h"
#include "../IoCContainer/IoCCore/IoC.h"
#include "../CmdExecutor/ICmdQueue.h"
#include "../ICommand.h"
#include "../UObject.h"

InterpretCommand::InterpretCommand(const std::string &obj_id, const std::string &op_id, const json& jargs)
    : object_id(obj_id), operation_id(op_id), args(jargs)
{

}

void InterpretCommand::execute()
{
    ICmdQueue* queue = IoC::Resolve<ICmdQueue*>("CommandQueue");

    UObject* obj = IoC::Resolve<UObject*>("Game.Objects", object_id);

    ICommand* cmd;

    if(args.size()){
        cmd = IoC::Resolve<ICommand*>(operation_id, obj, args);
    } else {
        cmd = IoC::Resolve<ICommand*>(operation_id, obj);
    }
    queue->push(cmd);
}


