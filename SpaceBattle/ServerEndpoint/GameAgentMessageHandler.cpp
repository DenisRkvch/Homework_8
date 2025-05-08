#include "GameAgentMessageHandler.h"
#include "../ICommand.h"
#include "../CmdExecutor/ICmdQueue.h"
#include "../IoCContainer/IoCCore/IoC.h"
#include "../IoCContainer/IoCCore/IoCException.h"
#include "../include/json.hpp"
#include "InterpretCommand.h"

using json = nlohmann::json;

GameAgentMessageHandler::GameAgentMessageHandler()
{

}

void GameAgentMessageHandler::handleMessage(const std::vector<uint8_t> &raw_data)
{
    /*
     * Формат сообщения JSON
     *  {
     *      "game_id": "string",
     *      "object_id": "string",
     *      "operation_id": "string",
     *      "args": {}
     *  }
     */

    // распарсить сообщение
    json j;
    try {
        j = json::parse(raw_data);
    }
    catch (const json::parse_error& e) {
        throw std::runtime_error("Invalid JSON format");
    }

    if (!j.contains("game_id") || !j["game_id"].is_string() ||
        !j.contains("object_id") || !j["object_id"].is_string() ||
        !j.contains("operation_id") || !j["operation_id"].is_string() ||
        j.contains("args") && !j["args"].is_object()) {
        throw std::runtime_error("Invalid message format - missing or wrong type fields");
    } 

    // получить из ioc подходящую очередь команд для game_id
    try {
        IoC::Resolve<ICommand*>("IoC.Scope.Current", std::string(j["game_id"]))->execute();
    }
    catch (const IoCException& e) {
        throw std::runtime_error(std::string("Can't find game with id: " + std::string(j["game_id"]) + ". ") + e.what());
    }
    
    ICmdQueue* queue;
    try {
        queue = IoC::Resolve<ICmdQueue*>("CommandQueue");
    }
    catch (const IoCException& e) {
        throw std::runtime_error(std::string("Command queue is not initiated for game: " + std::string(j["game_id"]) + ". ") + e.what());
    }

    // сконструировать interpretcommand
    InterpretCommand* cmd = new InterpretCommand(
        j["object_id"].get<std::string>(), 
        j["operation_id"].get<std::string>(), 
        j["args"]
    );

    // положить interpretcommand в очередь
    queue->push(cmd);
}
