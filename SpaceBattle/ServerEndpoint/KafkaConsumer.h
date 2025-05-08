#ifndef KAFKACONSUMER_H
#define KAFKACONSUMER_H

#include <librdkafka/rdkafkacpp.h>
#include <string>
#include <thread>
#include <atomic>
#include "IMessageHandler.h"

class KafkaConsumer
{
public:
    ~KafkaConsumer();
    KafkaConsumer(const std::string& broker, const std::string& topic, IMessageHandler* msgHandler);
    void start();
    void stop();
    bool isRunning();
   

private:
    void ConsumerLoop();

    IMessageHandler* messsageHandler;
    std::atomic<bool> running;
    std::thread consumer_thread;
    RdKafka::KafkaConsumer* consumer;
};

#endif // KAFKACONSUMER_H
