// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Tickets

namespace MasterLog {

struct Ticket
{
    typedef void (*TicketFunc)(void *, Ticket*);

    Ticket(){}

    template<typename C, typename T>
    void set(C* obj, void(*f)(C*,T*))
    {
        static_cast<Ticket*>(static_cast<T*>(this))-> // type verifier
        parameter = static_cast<void*>(obj);
        completer = reinterpret_cast<intptr_t>(f);
    }

    void evoke()
    {
        reinterpret_cast<TicketFunc>(completer)(parameter, this);
    }

private:
    void *parameter;
    intptr_t completer;
};

} // MasterLog
