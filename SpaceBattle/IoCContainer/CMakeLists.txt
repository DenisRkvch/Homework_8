
add_library(IoC STATIC
    
  "IoCConnectContainerCommand.cpp" "IoCConnectContainerCommand.h"
  "IScope.h"
  "Scope.cpp" "Scope.h"
  "ScopeTree.cpp" "ScopeTree.h"
  "Commands/IoCChangeCurrentScopeCommand.cpp" "Commands/IoCChangeCurrentScopeCommand.h"
  "Commands/IoCRegisterCommand.cpp" "Commands/IoCRegisterCommand.h"
  "Commands/IoCScopeDeleteCommand.cpp" "Commands/IoCScopeDeleteCommand.h"
  "Commands/IoCScopeNewCommand.cpp" "Commands/IoCScopeNewCommand.h"
)

add_subdirectory (IoCCore)

target_link_libraries(IoC PRIVATE
	IoCCore
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SpaceBattle PROPERTY CXX_STANDARD 20)
endif()

