// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Basic Configuration values:

namespace MasterLog {

struct Configuration
{
    Configuration();

    int initialize(int argc, char **argv);
    int validate();

    void print(FILE*);
};

} // Config : MasterLog
