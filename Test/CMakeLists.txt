# CMakeList.txt: проект CMake для My_test включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

add_executable(My_tests 
	"InitTestsStaticEnvironment.cpp"
	"InterpretCommandTest.cpp"
	
	 "GameAgentMessageHandlerTest.cpp" "KafkaConsumerTest.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
set_property(TARGET My_tests PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
# Включите подпроекты.
enable_testing()

IF (WIN32)
  # set stuff for windows
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
ENDIF()

add_subdirectory(googletest)

target_link_libraries(My_tests PUBLIC
	gtest_main
	gmock_main
	SpaceBattle
)

add_test(
	NAME My_tests
	COMMAND My_tests
)



