#include "ExceptionHandler.h"
#include "IoCContainer/IoCCore/IoC.h"

// define static veriable 
std::map<
    std::type_index, 
    std::map<
        std::type_index, 
        std::function<ICommand* (ICommand*, std::exception*)>
    >
> ExceptionHandler::_store;

void ExceptionHandler::ExceptionHandlerInit(void)
{
    IoC::Resolve<ICommand*>(
        "IoC.Register",
        std::string("HandleException"),
        New_Resolver(static_cast<ICommand*(*)(ICommand*, std::exception *)>([](ICommand* c, std::exception* e)->ICommand* {
            if (_store.find(typeid(*c)) == _store.end()) {
                // если по первому ключу не найдено, то вернуть исключение по умолчанию
                return new defaultHandlerCommand(c, e);
            }
            else if (_store[typeid(*c)].find(typeid(*e)) == _store[typeid(*c)].end()) {
                // если по второму ключу не найдено, то вернуть исключение по умолчанию
                return new defaultHandlerCommand(c, e);
            }

            return _store[typeid(*c)][typeid(*e)](c, e);

        })))->execute();

    IoC::Resolve<ICommand*>(
        "IoC.Register",
        std::string("RegisterException"),
        New_Resolver(static_cast<ICommand* (*)(std::type_index, std::type_index, std::function<ICommand* (ICommand*, std::exception*)>)>
            ([](std::type_index command_type, 
                std::type_index exception_type, 
                std::function < ICommand* (ICommand*, std::exception*)> callback) ->ICommand*
            {                           
                return new RegisterExceptionHandlerCommand(command_type, exception_type, callback);
            })
        ))->execute();
}



