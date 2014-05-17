#include <mstrlg/Platform.h>
#include <mstrlg/ConfigVar.h>
#include <mstrlg/Logging.h>
#include <mstrlg/JSONParse.h>
#include <mstrlg/Identification.h>
#include <mstrlg/Listener.h>
#include <mstrlg/Connector.h>
#include <mstrlg/DiskLog.h>
#include <mstrlg/Help.h>
#include <mstrlg/Configuration.h>

namespace MasterLog {

Identification::Config::Config():Map("identification", NULL){}
Listener::Config::Config():Array("listeners", ConfigVar::Type::MAP, NULL){}
Connector::Config::Config():Array("connectors", ConfigVar::Type::MAP, NULL){}
DiskLog::Config::Config():Map("disklog", NULL){}

}
