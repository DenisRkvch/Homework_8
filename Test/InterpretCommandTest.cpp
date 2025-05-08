#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "CmdExecutor/ICmdQueue.h"
#include "ServerEndpoint/InterpretCommand.h"
#include "CommandException.h"
#include "IoCContainer/IoCCore/IoC.h"
#include "IoCContainer/IoCCore/IoCException.h"
#include "json.hpp"
#include "UObject.h"

class MockQueue : public ICmdQueue {
public:
    MOCK_METHOD((void), push, (ICommand*), (override));
    MOCK_METHOD((ICommand*), pop, (), (override));
    MOCK_METHOD((bool), isEmpty, (), (override));
};

class MockUObject : public UObject {
public:
    MOCK_METHOD((void), setProperty, (const std::string&, const std::any&), (override)); 
    MOCK_METHOD((std::any), getProperty, (const std::string&), (override));
};

class SomeOperationCommand : public ICommand {
public:
    SomeOperationCommand(UObject* obj) :object(obj) {};
    void execute(void)
    {
        std::any ret = object->getProperty("test_property_get");
        object->setProperty("test_property_set", ret);
    }
private:
    UObject* object;
};

class SomeOperationCommand_args : public ICommand {
public:
    SomeOperationCommand_args(UObject* obj, json j) :object(obj), args(j) {};
    void execute(void)
    {
        std::any ret = object->getProperty(args["arg_1"].get<std::string>());
        object->setProperty(args["arg_2"].get<std::string>(), ret);
    }
private:
    UObject* object;
    json args;
};


TEST(TestInterpretCommand, succeed_interpret_no_args)
{
    // init scope
    IoC::Resolve<ICommand*>("IoC.Scope.New", std::string("scope.test1"))->execute();
    IoC::Resolve<ICommand*>("IoC.Scope.Current", std::string("scope.test1"))->execute();

    // init queue
    MockQueue q_mock;

    IoC::Resolve<ICommand*>(
        "IoC.Register",
        std::string("CommandQueue"),
        New_Resolver((std::function<ICmdQueue * ()>)[&q_mock]()->ICmdQueue* {
            return &q_mock;
    }))->execute();

    // init object
    MockUObject obj_mock;
    IoC::Resolve<ICommand*>(
        std::string("IoC.Register"),
        std::string("Game.Objects"),
        New_Resolver((std::function<UObject * (std::string)>)[&obj_mock](const std::string& str)->UObject* {
            if (str == "mock_object")
            {
                return &obj_mock;
            }
    }))->execute();

    // init operation
    IoC::Resolve<ICommand*>(
        std::string("IoC.Register"),
        std::string("Game.Operations.test_no_args"),
        New_Resolver((std::function<ICommand * (UObject*)>)[](UObject* obj)->ICommand* {
            return  new SomeOperationCommand(obj);
    }))->execute();

    // init args
    json args_empty = json::parse(R"({})");

    // init test-terget interpret command
    InterpretCommand test_command("mock_object", "Game.Operations.test_no_args", args_empty);


    EXPECT_CALL(q_mock, push).Times(1).WillOnce([](ICommand* cmd) {
            cmd->execute();
            delete cmd;
        });

    EXPECT_CALL(obj_mock, getProperty("test_property_get")).Times(1).WillOnce(testing::Return(&obj_mock));  // возвращает ссылку на себя, обернутую в std::any
    EXPECT_CALL(obj_mock, setProperty("test_property_set", ::testing::Truly([&obj_mock](const std::any& arg)// проверяет что пришла ссылка на себя, обернутая в std::any 
        {
            return std::any_cast<MockUObject*>(arg) == &obj_mock;
        }))).Times(1);

    test_command.execute();

    IoC::Resolve<ICommand*>("IoC.Scope.Delete")->execute();
}

TEST(TestInterpretCommand, succeed_interpret_with_args)
{
    // init scope
    IoC::Resolve<ICommand*>("IoC.Scope.New", std::string("scope.test2"))->execute();
    IoC::Resolve<ICommand*>("IoC.Scope.Current", std::string("scope.test2"))->execute();

    // init queue
    MockQueue q_mock;

    IoC::Resolve<ICommand*>(
        "IoC.Register",
        std::string("CommandQueue"),
        New_Resolver((std::function<ICmdQueue * ()>)[&q_mock]()->ICmdQueue* {
            return &q_mock;
    }))->execute();

    // init object
    MockUObject obj_mock;
    IoC::Resolve<ICommand*>(
        std::string("IoC.Register"),
        std::string("Game.Objects"),
        New_Resolver((std::function<UObject * (std::string)>)[&obj_mock](const std::string& str)->UObject* {
        if (str == "mock_object")
        {
            return &obj_mock;
        }
    }))->execute();

    // init operation
    IoC::Resolve<ICommand*>(
        std::string("IoC.Register"),
        std::string("Game.Operations.test_with_args"),
        New_Resolver((std::function<ICommand * (UObject*, json)>)[](UObject* obj, json args)->ICommand* {
            return  new SomeOperationCommand_args(obj, args);
    }))->execute();

    // init args
    json jArgs = json::parse(R"({ "arg_1": "val1", "arg_2": "val2" })");

    // init test-terget interpret command
    InterpretCommand test_command("mock_object", "Game.Operations.test_with_args", jArgs);

    EXPECT_CALL(q_mock, push).Times(1).WillOnce([](ICommand* cmd) {
        cmd->execute();
        delete cmd;
        });

    EXPECT_CALL(obj_mock, getProperty("val1")).Times(1).WillOnce(testing::Return(&obj_mock));  // возвращает ссылку на себя, обернутую в std::any
    EXPECT_CALL(obj_mock, setProperty("val2", ::testing::Truly([&obj_mock](const std::any& arg)// проверяет что пришла ссылка на себя, обернутая в std::any 
        {
            return std::any_cast<MockUObject*>(arg) == &obj_mock;
        }))).Times(1);

    test_command.execute();

    IoC::Resolve<ICommand*>("IoC.Scope.Delete")->execute();
}

TEST(TestInterpretCommand, error_queue_not_initialized)
{
    json args_empty(nullptr);

    // init test-terget interpret command
    InterpretCommand test_command("some_object", "Game.Operations.some", args_empty);

    EXPECT_THROW({
        test_command.execute();
     }, IoCException);
}

TEST(TestInterpretCommand, error_object_not_initialized)
{
    // init scope
    IoC::Resolve<ICommand*>("IoC.Scope.New", std::string("scope.test4"))->execute();
    IoC::Resolve<ICommand*>("IoC.Scope.Current", std::string("scope.test4"))->execute();

    // init queue
    MockQueue q_mock;

    IoC::Resolve<ICommand*>(
        "IoC.Register",
        std::string("CommandQueue"),
        New_Resolver((std::function<ICmdQueue * ()>)[&q_mock]()->ICmdQueue* {
        return &q_mock;
    }))->execute();

    // init operation
    IoC::Resolve<ICommand*>(
        std::string("IoC.Register"),
        std::string("Game.Operations.test_no_args"),
        New_Resolver((std::function<ICommand * (UObject*)>)[](UObject* obj)->ICommand* {
        return  new SomeOperationCommand(obj);
    }))->execute();

    // init args
    json args_empty(nullptr);

    // init test-terget interpret command
    InterpretCommand test_command("nothing_object", "Game.Operations.test_no_args", args_empty);

    EXPECT_THROW({
    test_command.execute();
        }, IoCException);

    IoC::Resolve<ICommand*>("IoC.Scope.Delete")->execute();
}

TEST(TestInterpretCommand, error_operation_not_initialized)
{
    // init scope
    IoC::Resolve<ICommand*>("IoC.Scope.New", std::string("scope.test1"))->execute();
    IoC::Resolve<ICommand*>("IoC.Scope.Current", std::string("scope.test1"))->execute();

    // init queue
    MockQueue q_mock;

    IoC::Resolve<ICommand*>(
        "IoC.Register",
        std::string("CommandQueue"),
        New_Resolver((std::function<ICmdQueue * ()>)[&q_mock]()->ICmdQueue* {
        return &q_mock;
    }))->execute();

    // init object
    MockUObject obj_mock;
    IoC::Resolve<ICommand*>(
        std::string("IoC.Register"),
        std::string("Game.Objects"),
        New_Resolver((std::function<UObject * (std::string)>)[&obj_mock](const std::string& str)->UObject* {
        if (str == "mock_object")
        {
            return &obj_mock;
        }
    }))->execute();

    // init args
    json args_empty(nullptr);

    // init test-terget interpret command
    InterpretCommand test_command("mock_object", "Game.Operations.nothing", args_empty);

    EXPECT_THROW({
    test_command.execute();
        }, IoCException);

    IoC::Resolve<ICommand*>("IoC.Scope.Delete")->execute();
}