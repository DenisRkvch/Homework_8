#ifndef GAMEAGENTMESSAGEHANDLER_H
#define GAMEAGENTMESSAGEHANDLER_H

#include "IMessageHandler.h"


class GameAgentMessageHandler : public IMessageHandler
{
public:
    GameAgentMessageHandler();

    // IMessageHandler interface
    void handleMessage(const std::vector<uint8_t> &raw_data) override;
};

#endif // GAMEAGENTMESSAGEHANDLER_H
