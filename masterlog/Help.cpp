// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Help and related services

#include <mstrlg/Platform.h>
#include <mstrlg/Help.h>

namespace MasterLog {
namespace Help {

static const char *exeName = "masterlog";

int initialize(int argc, char **argv)
{
    if (argv[0] && argv[0][0])
        exeName = argv[0];

    return 0;
}

}} // Help : MasterLog
