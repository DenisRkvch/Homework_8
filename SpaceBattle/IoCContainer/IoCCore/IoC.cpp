#include "IoC.h"
#include "IoCException.h"

class IoC::DefaultIoCContainer : public IIoCContainer
{

public:
    IFunctor *operator [](const std::string& str) override
    {
        if(str == "IoC.ConnectContainer")
        {
            return default_resolver;
        }
        else
        {
            throw IoCException("dependency for <" + str + "> is not found");
        }
    }

private:
    IFunctor* default_resolver = New_Resolver((std::function<void(IIoCContainer**)>) [](IIoCContainer** newContainer_ptr){
            IIoCContainer** oldContainer_ptr = ioCContainer_ptr;
            ioCContainer_ptr = newContainer_ptr;

            delete *oldContainer_ptr;
            delete oldContainer_ptr;
        });

    ~DefaultIoCContainer(void) override {
        delete default_resolver;
    }
};


IIoCContainer** IoC::ioCContainer_ptr = new IIoCContainer*(new DefaultIoCContainer());
