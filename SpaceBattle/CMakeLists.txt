
add_library(SpaceBattle STATIC
  "ICommand.h"
  "UObject.h"
  "CommandException.cpp" "CommandException.h"
  "ExceptionHandler.cpp" "ExceptionHandler.h"
   )

target_include_directories(SpaceBattle PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory (IoCContainer) 
add_subdirectory (CmdExecutor)
add_subdirectory (ServerEndpoint)


target_link_libraries(SpaceBattle PUBLIC
	IoC
	CmdExecutor
	ServerEndpoint
)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SpaceBattle PROPERTY CXX_STANDARD 20)
endif()

