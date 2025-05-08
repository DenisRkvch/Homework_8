#include "ServerEndpoint/KafkaConsumer.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include "KafkaConsumer.h"
#include "IMessageHandler.h"
#include <librdkafka/rdkafka.h>
#include <vector>

using ::testing::_;
using ::testing::Return;
using ::testing::Throw;

class MockMessageHandler : public IMessageHandler 
{
public:
    MOCK_METHOD(void, handleMessage, (const std::vector<uint8_t>&), (override));
};

// Вспомогательная функция для отправки сообщения
void ProduceMessage(const std::string& broker, const std::string& topic, const std::vector<uint8_t>& msg) {
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    std::string errstr;
    conf->set("bootstrap.servers", broker, errstr);

    RdKafka::Producer* producer = RdKafka::Producer::create(conf, errstr);
    delete conf;

   
    RdKafka::ErrorCode err = producer->produce(
        topic,
        RdKafka::Topic::PARTITION_UA,
        RdKafka::Producer::RK_MSG_COPY,
        const_cast<uint8_t*>(msg.data()), msg.size(),
        nullptr, 0, 0, nullptr
    );

    if (err != RdKafka::ERR_NO_ERROR) {
        std::cerr << "Failed to produce message: " << RdKafka::err2str(err) << std::endl;
    }

    producer->flush(1000);
   

    delete producer;
}


// Тест запуска и остановки потока
TEST(KafkaConsumerTest, start_and_stop_consumer_thread) 
{
    KafkaConsumer consumer("", "", nullptr);

    EXPECT_TRUE(consumer.isRunning()); 

    consumer.stop();

    EXPECT_FALSE(consumer.isRunning());

    consumer.start();

    EXPECT_TRUE(consumer.isRunning());

    consumer.stop();

    EXPECT_FALSE(consumer.isRunning());
}

TEST(KafkaConsumerTest, consume_and_processes_message) {
    const std::vector<uint8_t> test_message = { 't', 'e', 's', 't'};

    MockMessageHandler handler;
    EXPECT_CALL(handler, handleMessage(test_message)).Times(1);

    KafkaConsumer consumer("localhost:9092", "testtopic", &handler);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    ProduceMessage("localhost:9092", "testtopic", test_message);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    consumer.stop();
}

TEST(KafkaConsumerTest, handleexceptions) {
    const std::vector<uint8_t> test_message = { 'e', 'x', 'c', 'e', 'p', 't', 'i', 'o', 'n'};

    MockMessageHandler handler;
    EXPECT_CALL(handler, handleMessage(test_message))
        .WillOnce(Throw(std::runtime_error("test error")));

    KafkaConsumer consumer("localhost:9092", "testtopic", &handler);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    ProduceMessage("localhost:9092", "testtopic", test_message);

    // Проверяем, что исключение не приводит к падению
    std::this_thread::sleep_for(std::chrono::seconds(1));

    consumer.stop();
}