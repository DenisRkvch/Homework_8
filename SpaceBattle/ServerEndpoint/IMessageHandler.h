#ifndef IMSGHANDLER_H
#define IMSGHANDLER_H

#include <vector>
#include <cstdint>

class IMessageHandler
{
public:
    virtual ~IMessageHandler() = default;
    virtual void handleMessage(const std::vector<uint8_t>& raw_data) = 0;
};
#endif // IMSGHANDLER_H
