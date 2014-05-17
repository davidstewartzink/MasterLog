// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Help and related services

namespace MasterLog {
namespace Help {

struct Config : ConfigVar::Map
{
    Config();
};

int initialize(int argc, char **argv);

void switchToRunning();

}} // Help : MasterLog
