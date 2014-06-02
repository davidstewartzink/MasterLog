// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Protocols

#include <mstrlg/Everything.h>

namespace MasterLog {
static const String json("JSON");
static const String tcp("TCP");

Protocol::Protocol(String const& nm):_name(nm),next(NULL)
{
}

Protocol::~Protocol()
{
    delete next;
}

Protocol* Protocol::instantiate(JSON_Config::Value const &pro)
{
    if (pro.isString())
    {
        return new Protocol(pro.stringValue());
    }
    Protocol* p = NULL;
    if (pro.isArray())
    {
        const int n = pro.count();
        for (int i = 0; i < n; ++i)
        {
            Protocol* t = instantiate(pro.element(i));
            if (!t)
            {
                delete p;
                return 0;
            }
            t->next = p;
            p = t;
        }
    }
    return p;
}

}
