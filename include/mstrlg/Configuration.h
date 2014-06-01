// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Basic Configuration values:

namespace MasterLog {

struct Configuration : JSON_Config::Value 
{
    Configuration(const char *defaultFileName);

    int initialize(int argc, char **argv);
    int validate();
};

} // MasterLog
