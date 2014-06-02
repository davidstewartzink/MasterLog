// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// DNS Name Resolution

namespace MasterLog {
namespace DNS {

extern int configure(Configuration const& cfg);

struct ResolveName : Ticket
{
    ErrorDescription error;
    String name;
    uint32_t host;

    template<typename C>
    void set(C* obj, void(*f)(C*,ResolveName*), String const& nm)
    {
        name = nm;
        host = 0;
        Ticket::set(obj, f);
    }

    void submit();
};

}} // DNS : MasterLog
