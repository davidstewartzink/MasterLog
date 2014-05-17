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

// Configured values:
static Configuration configuration;

int main(int argc, char **argv)
{
    int r = Help::initialize(argc, argv);
    if (r) return r;

    r = configuration.initialize(argc, argv);
    if (r) return r;

    return 0;
}

}

int main(int argc, char **argv)
{
    return MasterLog::main(argc, argv);
}
