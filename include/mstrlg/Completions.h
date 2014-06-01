// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Completions

namespace MasterLog {

struct Completion
{
    typedef void (*X)(void *data);
    void *data;
    X compl;

    template<typename C>Completion(C* obj, void(*f)(C*obj))
        :data(static_cast<void*>(obj))
        ,compl(static_cast<X>(f))
    {
    }
};

} // MasterLog
