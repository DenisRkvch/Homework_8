cmake_minimum_required(VERSION 3.10)

# Ищем librdkafka
if(WIN32)
    find_package(RdKafka REQUIRED)  

    set(RDKAFKAC_LIB RdKafka::rdkafka)
    set(RDKAFKACPP_LIB RdKafka::rdkafka++)

else()
    # Используем pkg-config для поиска библиотеки
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(RDKAFKA REQUIRED IMPORTED_TARGET rdkafka)
    pkg_check_modules(RDKAFKACPP REQUIRED IMPORTED_TARGET rdkafka++)

    set(RDKAFKAC_LIB PkgConfig::RDKAFKA)
    set(RDKAFKACPP_LIB PkgConfig::RDKAFKACPP)
endif()

add_library(ServerEndpoint STATIC
    "KafkaConsumer.cpp" "KafkaConsumer.h"
    "IMessageHandler.h"
    "GameAgentMessageHandler.cpp" "GameAgentMessageHandler.h"
    "InterpretCommand.cpp" "InterpretCommand.h"
)
target_include_directories(ServerEndpoint PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ServerEndpoint PUBLIC
    ${RDKAFKAC_LIB}
    ${RDKAFKACPP_LIB}
)



