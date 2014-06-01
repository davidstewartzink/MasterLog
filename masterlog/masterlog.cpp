// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

#include <mstrlg/Everything.h>

namespace MasterLog {

// Configured values:
static Configuration configuration("/etc/masterlog.conf");

int main(int argc, char **argv)
{
    int r = Help::initialize(argc, argv);
    if (r) return r;

    r = configuration.initialize(argc, argv);
    if (r) return r;

    Debug("initialized");
    fprintf(stderr, "Configuration = ");
    configuration.print(stderr);

    r = Storage::configure(configuration);
    if (r) return r;

    r = Network::configure(configuration);
    if (r) return r;

    return Network::run();
}

}

int main(int argc, char **argv)
{
    int r = MasterLog::main(argc, argv);
    return r < 0 ? 1 : r;
}
