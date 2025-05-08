#ifndef IOCCONNECTCONTAINERCOMMAND_H
#define IOCCONNECTCONTAINERCOMMAND_H

#include "../ICommand.h"
#include "IoCCore/IIoCContainer.h"
#include "IoCCore/IoC.h"


class IoCConnectContainerCommand : public ICommand
{
public:

    IoCConnectContainerCommand(IIoCContainer*);

    void execute() override;

private:

    IIoCContainer* iocContainer;
    static IIoCContainer* iocContainer_connector;
    static bool isConnected;
};

#endif // IOCCONNECTCONTAINERCOMMAND_H
