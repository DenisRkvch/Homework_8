#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ServerEndpoint/GameAgentMessageHandler.h"
#include "IoCContainer/IoCCore/IoC.h"
#include "CmdExecutor/ICmdQueue.h"
#include "ServerEndpoint/InterpretCommand.h"
#include "UObject.h"
#include <vector>
#include <string>


class MockQueue : public ICmdQueue {
public:
    MOCK_METHOD((void), push, (ICommand*), (override));
    MOCK_METHOD((ICommand*), pop, (), (override));
    MOCK_METHOD((bool), isEmpty, (), (override));
};


TEST(TestGameAgentMessageHandler, succeed_parse_message)
{
    bool succeed_get_object = false;
    bool succeed_get_operation = false;

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
    IoC::Resolve<ICommand*>(
        std::string("IoC.Register"),
        std::string("Game.Objects"),
        New_Resolver((std::function<UObject * (std::string)>)[&succeed_get_object](const std::string& str)->UObject* {
            succeed_get_object = str == "test_obj_id";
            return nullptr;      
    }))->execute();

    // init operation
    IoC::Resolve<ICommand*>(
        std::string("IoC.Register"),
        std::string("test_op_id"),
        New_Resolver((std::function<ICommand * (UObject*, json)>)[&succeed_get_operation](UObject* obj, json args)->ICommand* {
            succeed_get_operation = 
                args.contains("arg1") &&
                args.contains("arg2") &&
                (args["arg1"] == "val1") &&
                (args["arg2"] == 100);
            return  nullptr; 
        }
    ))->execute();

    GameAgentMessageHandler test_message_handler;

    std::string message = R"(
    { 
        "game_id": "scope.test1", 
        "object_id": "test_obj_id",
        "operation_id": "test_op_id",
        "args": {
            "arg1": "val1",
            "arg2": 100
        }
    })";

    EXPECT_CALL(q_mock, push).Times(2)
        .WillOnce([](ICommand* cmd) {
            cmd->execute();
            delete cmd;
        })
        .WillOnce([]() {});

    test_message_handler.handleMessage(std::vector<uint8_t>(message.begin(), message.end()));

    EXPECT_TRUE(succeed_get_object);
    EXPECT_TRUE(succeed_get_operation);

    IoC::Resolve<ICommand*>("IoC.Scope.Delete")->execute();
}

TEST(TestGameAgentMessageHandler, succeed_parse_message_no_args)
{
    bool succeed_get_object = false;
    bool succeed_get_operation = false;

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


    GameAgentMessageHandler test_message_handler;

    std::string message = R"(
    { 
        "game_id": "scope.test2", 
        "object_id": "test_obj_id",
        "operation_id": "test_op_id"
    })";

    EXPECT_CALL(q_mock, push(testing::NotNull())).Times(1);
    EXPECT_NO_THROW({
        test_message_handler.handleMessage(std::vector<uint8_t>(message.begin(), message.end()));
    });

    IoC::Resolve<ICommand*>("IoC.Scope.Delete")->execute();
}

TEST(TestGameAgentMessageHandler, error_json_format)
{
    GameAgentMessageHandler test_message_handler;
    std::string message = R"({)";

    EXPECT_THROW({
        test_message_handler.handleMessage(std::vector<uint8_t>(message.begin(), message.end()));
    }, std::runtime_error);
}

TEST(TestGameAgentMessageHandler, error_message_format)
{
    // init scope
    IoC::Resolve<ICommand*>("IoC.Scope.New", std::string("str1"))->execute();
    IoC::Resolve<ICommand*>("IoC.Scope.Current", std::string("str1"))->execute();

    // init queue
    MockQueue q_mock;

    IoC::Resolve<ICommand*>(
        "IoC.Register",
        std::string("CommandQueue"),
        New_Resolver((std::function<ICmdQueue * ()>)[&q_mock]()->ICmdQueue* {
        return &q_mock;
    }))->execute();

    GameAgentMessageHandler test_message_handler;

    std::string message = R"({"err": "str1", "object_id": "str2", "operation_id": "str3", "args": {"arg1": "val1","arg2": 100}})";
    EXPECT_THROW({
        test_message_handler.handleMessage(std::vector<uint8_t>(message.begin(), message.end()));
        }, std::runtime_error);

    message = R"({"str1": 666, "object_id": "str2", "operation_id": "str3", "args": {"arg1": "val1","arg2": 100}})";
    EXPECT_THROW({
        test_message_handler.handleMessage(std::vector<uint8_t>(message.begin(), message.end()));
        }, std::runtime_error);

    message = R"({"game_id": "str1", "err": "str2", "operation_id": "str3", "args": {"arg1": "val1","arg2": 100}})";
    EXPECT_THROW({
        test_message_handler.handleMessage(std::vector<uint8_t>(message.begin(), message.end()));
        }, std::runtime_error);
    message = R"({"game_id": "str1", "object_id": 666, "operation_id": "str3", "args": {"arg1": "val1","arg2": 100}})";
    EXPECT_THROW({
        test_message_handler.handleMessage(std::vector<uint8_t>(message.begin(), message.end()));
        }, std::runtime_error);

    message = R"({"game_id": "str1", "object_id": "str2", "err": "str3", "args": {"arg1": "val1","arg2": 100}})";
    EXPECT_THROW({
        test_message_handler.handleMessage(std::vector<uint8_t>(message.begin(), message.end()));
        }, std::runtime_error);
    message = R"({"game_id": "str1", "object_id": "str2", "operation_id": 666, "args": {"arg1": "val1","arg2": 100}})";
    EXPECT_THROW({
        test_message_handler.handleMessage(std::vector<uint8_t>(message.begin(), message.end()));
        }, std::runtime_error);

    message = R"({"game_id": "str1", "object_id": "str2", "operation_id": str3, "args": {"arg1": "val1","arg2": 100}})";
    EXPECT_THROW({
        test_message_handler.handleMessage(std::vector<uint8_t>(message.begin(), message.end()));
        }, std::runtime_error);

    message = R"({"game_id": "str1", "object_id": "str2", "operation_id": str3, "args": 666})";
    EXPECT_THROW({
        test_message_handler.handleMessage(std::vector<uint8_t>(message.begin(), message.end()));
        }, std::runtime_error);

    IoC::Resolve<ICommand*>("IoC.Scope.Delete")->execute();

}

TEST(TestGameAgentMessageHandler, error_no_game_queue)
{
    // init scope
    IoC::Resolve<ICommand*>("IoC.Scope.New", std::string("game_scope"))->execute();

    GameAgentMessageHandler test_message_handler;

    // ошмбка - нет очереди, есть скоуп
    std::string message = R"({"game_id": "game_scope", "object_id": "str2", "operation_id": "str3", "args": {"arg1": "val1","arg2": 100}})";
    EXPECT_THROW({
        test_message_handler.handleMessage(std::vector<uint8_t>(message.begin(), message.end()));
        }, std::runtime_error);

    IoC::Resolve<ICommand*>("IoC.Scope.Delete")->execute();
}

TEST(TestGameAgentMessageHandler, error_no_game_scope)
{
    // init scope
    IoC::Resolve<ICommand*>("IoC.Scope.New", std::string("another_scope"))->execute();
    IoC::Resolve<ICommand*>("IoC.Scope.Current", std::string("another_scope"))->execute();

    // init queue
    MockQueue q_mock;
    IoC::Resolve<ICommand*>(
        "IoC.Register",
        std::string("CommandQueue"),
        New_Resolver((std::function<ICmdQueue * ()>)[&q_mock]()->ICmdQueue* {
        return &q_mock;
    }))->execute();

    GameAgentMessageHandler test_message_handler;

    // ошмбка - нет нужного скоупа 
    std::string message = R"({"game_id": "game_scope", "object_id": "str2", "operation_id": "str3", "args": {"arg1": "val1","arg2": 100}})";
    EXPECT_THROW({
        test_message_handler.handleMessage(std::vector<uint8_t>(message.begin(), message.end()));
        }, std::runtime_error);

    IoC::Resolve<ICommand*>("IoC.Scope.Current", std::string("another_scope"))->execute();
    IoC::Resolve<ICommand*>("IoC.Scope.Delete")->execute();
}