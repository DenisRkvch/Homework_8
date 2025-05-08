#include "IoCConnectContainerCommand.h"

bool IoCConnectContainerCommand::isConnected = false;
IIoCContainer* IoCConnectContainerCommand::iocContainer_connector;

IoCConnectContainerCommand::IoCConnectContainerCommand(IIoCContainer* ioc_container) : iocContainer(ioc_container)
{
    if(!isConnected)
    {
        IoC::Resolve<void>("IoC.ConnectContainer", &iocContainer_connector);
        isConnected = true;
    }
}

void IoCConnectContainerCommand::execute()
{
    iocContainer_connector = iocContainer;
}
