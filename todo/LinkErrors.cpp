// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.
//
// Noting to be proud of

#include <mstrlg/Everything.h>

namespace MasterLog {

Listener::Config::Config():Array("listeners", ConfigVar::Type::MAP, NULL){}
Connector::Config::Config():Array("connectors", ConfigVar::Type::MAP, NULL){}
DiskLog::Config::Config():Map("disklog", NULL){}

}
