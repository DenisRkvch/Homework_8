
add_library(CmdExecutor STATIC
    
  "CmdExecutor.cpp" "CmdExecutor.h"
  "Commands/HardStopExecutorCommand.cpp" "Commands/HardStopExecutorCommand.h"
  "Commands/SoftStopExecutorCommand.cpp" "Commands/SoftStopExecutorCommand.h"
  "Commands/StartExecutorCommand.cpp" "Commands/StartExecutorCommand.h"
  "ICmdQueue.h"
  "ThreadSafeCmdQueue.cpp" "ThreadSafeCmdQueue.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CmdExecutor PROPERTY CXX_STANDARD 20)
endif()

