// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Help and related services

#include <mstrlg/Everything.h>

namespace MasterLog {
namespace Help {

const char *exeName = "masterlog";

Config::Config():Map("help", NULL)
{
}

int initialize(int argc, char **argv)
{
    if (argv[0] && argv[0][0])
    {
        const char *p = argv[0];
        exeName = p;
        while (*p)
        {
            if (*p++ == '/')
                exeName = p;
        }
    }

    return 0;
}

}} // Help : MasterLog
