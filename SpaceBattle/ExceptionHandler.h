#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H

#include <map>
#include <string>
#include <functional>
#include <typeinfo>
#include <typeindex>
#include <iostream>
#include "ICommand.h"

class ExceptionHandler
{

public:

    ExceptionHandler() = default;
	~ExceptionHandler() = default;

	static void ExceptionHandlerInit(void);

private:

	static std::map<
		std::type_index,
		std::map<
		std::type_index,
		std::function<ICommand* (ICommand*, std::exception*)>
		>
	>_store;

	// Команда-обработчик по умолчанию, если не найденна подходящая в _store
    class defaultHandlerCommand : public ICommand
	{
	public:
        defaultHandlerCommand(ICommand* c, std::exception* e) : _c(c), _e(e) {  }

		// Унаследовано через ICommand
		void execute(void) override
		{
            std::cout << "A corresponding handler for Command: " << typeid(*_c).name() <<
                " and Exception class: " << typeid(*_e).name() << " is not registered! \n";
            delete _c;
		}
	private:
		ICommand* _c;
		std::exception* _e;
	};

	// Команда-регистратов оюработчиков в _store
	class RegisterExceptionHandlerCommand : public ICommand
	{
	public:
		RegisterExceptionHandlerCommand(std::type_index command_type, 
										std::type_index exception_type, 
										std::function < ICommand* (ICommand*, std::exception*)> callback) 
			: _command_type(command_type), _exception_type(exception_type), _callback(callback)
		{
		}

		// Унаследовано через ICommand
		void execute(void) override
		{
			_store[_command_type][_exception_type] = _callback;
		}
	private:
		std::type_index _command_type, _exception_type;
		std::function < ICommand* (ICommand*, std::exception*)> _callback;
	};

};

#endif
