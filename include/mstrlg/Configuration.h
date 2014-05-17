// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Basic Configuration values:

namespace MasterLog {

struct Configuration : ConfigVar::Map
{
// Help for this Service:
    Help::Config help;

// Identification for this Service:
    Identification::Config identification;

// Initial Ports to listen to:
    Listener::Config listeners;

// Initial Remote Devices to connect to:
    Connector::Config connectors;

// Parameters regarding Log Space Allocation:
    DiskLog::Config diskLogs;


    Configuration();

    int initialize(int argc, char **argv);
    int validate();
};

} // Config : MasterLog
