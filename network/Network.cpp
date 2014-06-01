// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Network access

#include <mstrlg/Everything.h>

namespace MasterLog { namespace Network {
static const String network("network");
static const String listeners("listeners");
static const String connectors("connectors");
static const String interface("interface");
static const String port("port");
static const String protocol("protocol");
static const String JSON("JSON");
static const String TCP("TCP");

static struct
{
    int maxListeners;
    int maxConnectors;
} config;

static int handleTimeout()
{
    return -1;
}

int addListener(JSON_Config::Value const &l)
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

int setupPollers(pollfd pfds[], int maxPfds)
{
    return 0;
}

int run()
{
    int r = 0;
    const int maxPfds = config.maxListeners + config.maxConnectors;
    pollfd *pfds = new pollfd[maxPfds];
    for (;;)
    {
        int npfds = setupPollers(pfds, maxPfds);
        int timeo = handleTimeout();

        int nr = ::poll(pfds, npfds, timeo);

        if (nr == 0)
            continue;
        if (nr < 0)
        {
            switch(errno)
            {
            case EAGAIN: // not a good sign.
                continue;
            case EFAULT: // should be impossible.
            case EINVAL:
            default:
                ErrorLog("poll returns %d", int(errno));
                r = 1;
            case EINTR:
                break;
            }
            break;
        }
    }
    delete[] pfds;
    return r;
}

int configure(Configuration const& cfg)
{
    JSON_Config::Value netConfig(cfg.member(network));
    if (netConfig.isObject())
    {
        config.maxListeners = 1000;
        config.maxConnectors = 1000;
    }
    else if (!netConfig.isNull())
    {
        Error("Invalid \"network\" configuration: Exists but not an Object");
        return 1;
    }

    JSON_Config::Value lConfig(cfg.member(listeners));
    if (lConfig.isArray())
    {
        int n = lConfig.count();
        for (int i = 0; i < n; ++i)
        {
            int r = addListener(lConfig.element(i));
            if (r)
                return r;
        }
    }
    else if (lConfig.isObject())
    {
        int r = addListener(lConfig);
        if (r)
            return r;
    }
    else if (!lConfig.isNull())
    {
        Error("Invalid \"listeners\" configuration: Exists but not an Object or Array");
        return 1;
    }

    JSON_Config::Value cConfig(cfg.member(connectors));
    if (cConfig.isArray())
    {
    }
    else if (cConfig.isObject())
    {
    }
    else if (!cConfig.isNull())
    {
        Error("Invalid \"connectors\" configuration: Exists but not an Object or Array");
        return 1;
    }

    return 0;
}

}} // Network : MasterLog
