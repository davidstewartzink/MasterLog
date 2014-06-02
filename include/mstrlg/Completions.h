// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Completions

namespace MasterLog {

struct Completion
{
    typedef void (*CompletionFunc)(void *);

    void *parameter;
    intptr_t completer;

    template<typename C>
    Completion(C* obj, void(*f)(C*obj))
        :parameter(static_cast<void*>(obj))
        ,completer(reinterpret_cast<intptr_t>(f))
    {
    }

    void evoke()
    {
        reinterpret_cast<CompletionFunc>(completer)(parameter);
    }
};

} // MasterLog
