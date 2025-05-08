#include "KafkaConsumer.h"
#include <vector>
#include <string.h>
#include <iostream>

KafkaConsumer::KafkaConsumer(const std::string &broker, const std::string &topic, IMessageHandler *msgHandler)
    : messsageHandler(msgHandler), running(false)
{
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);


    std::string errstr;

    conf->set("bootstrap.servers", broker, errstr);
    conf->set("group.id", "game-server", errstr);
    conf->set("enable.auto.commit", "false", errstr);

    consumer = RdKafka::KafkaConsumer::create(conf, errstr);

    delete conf;

    consumer->subscribe({ topic });

    start();
}

void KafkaConsumer::start()
{
    if (running) return;
    running = true;
    consumer_thread = std::thread(&KafkaConsumer::ConsumerLoop, this);
}

void KafkaConsumer::stop()
{
    running = false;
    if (consumer_thread.joinable())
    {
        consumer_thread.join();
    }
}

bool KafkaConsumer::isRunning()
{
    return running;
}

KafkaConsumer::~KafkaConsumer()
{
    stop();
    consumer->close();
}

void KafkaConsumer::ConsumerLoop()
{
    while(running)
    {
        // consume msg..
        RdKafka::Message* msg = consumer->consume(1000);

        if (msg->err() == RdKafka::ERR_NO_ERROR)
        {
            try {
                messsageHandler->handleMessage(std::vector<uint8_t>((uint8_t*)msg->payload(), (uint8_t*)msg->payload() + msg->len()));
            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;

            }
        }
        delete msg;
    }
}
