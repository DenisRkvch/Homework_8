#ifndef FUNCTOR_H
#define FUNCTOR_H
#include <functional>

template<class T, typename... Args> class Functor;

class IFunctor
{
public:

    virtual ~IFunctor() = default;

    template<class T, typename... Args>
    T call(Args... args){
        return (*dynamic_cast<Functor<T, Args...>*>(this))(args...);
    }

};

template<class T, typename... Args>
class Functor : public IFunctor
{
public:


    Functor(std::function<T(Args...)> f) : function(f){}

    T operator()(Args... args)  {
        return function(args...);
    }

private:

    std::function<T(Args...)> function;
};


#endif // FUNCTOR_H
