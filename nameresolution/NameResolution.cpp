// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Network access

#include <mstrlg/Everything.h>

namespace MasterLog { namespace DNS {
//static const String network("dns");

void ResolveName::submit()
{
    host = 0x7f000001;
    evoke();
}

int configure(Configuration const& cfg)
{
    return 0;
}

}} // Network : MasterLog
