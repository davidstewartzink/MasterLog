// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

#include <mstrlg/Everything.h>

namespace MasterLog { namespace Connector {
static const String connectors("connectors");
static const String interface("host");
static const String port("port");
static const String protocol("protocol");
static const String JSON("JSON");
static const String TCP("TCP");

int addConnector(JSON_Config::Value const &l)
{
    if (!l.isObject()) return 1;

/*
  {
     "interface":"localhost",
        "port":4099,
           "protocol":[
               "JSON",
                   "TCP"
                      ]
                        }
*/

    return 0;
}

int configure(Configuration const& cfg)
{
    JSON_Config::Value cConfig(cfg.member(connectors));
    if (cConfig.isArray())
    {
        int n = cConfig.count();
        for (int i = 0; i < n; ++i)
        {
            int r = addConnector(cConfig.element(i));
            if (r)
                return r;
        }
    }
    else if (cConfig.isObject())
    {
        int r = addConnector(cConfig);
        if (r)
            return r;
    }
    else if (!cConfig.isNull())
    {
        Error("Invalid \"listeners\" configuration: Exists but not an Object or Array");
        return 1;
    }

    return 0;
}

}}
