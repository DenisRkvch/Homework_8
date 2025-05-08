
add_library(IoCCore STATIC
    
  "IFunctor.h"
  "IIoCContainer.h"
  "IoC.cpp" "IoC.h"
  "IoCException.cpp" "IoCException.h"
)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SpaceBattle PROPERTY CXX_STANDARD 20)
endif()

