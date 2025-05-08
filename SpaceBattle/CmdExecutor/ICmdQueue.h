#ifndef ICMDQUEUE_H
#define ICMDQUEUE_H

#include "../ICommand.h"

class ICmdQueue
{
public:
    virtual ~ICmdQueue() = default;

    virtual void push(ICommand*) = 0;
    virtual ICommand* pop(void) = 0;
    virtual bool isEmpty(void) = 0;

};
#endif // ICMDQUEUE_H
