// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

#include <mstrlg/Everything.h>

namespace MasterLog { namespace Listener {
static const String listeners("listeners");
static const String bindTo("bindTo");
static const String port("port");
static const String protocol("protocol");

class Listener
{
    Protocol* proto;
    uint32_t _bindTo;
    uint16_t _port;

public:
    Listener(uint32_t b, uint16_t po, Protocol* p):proto(p),_bindTo(b),_port(po)
    {
        (void)proto;
    }

    uint32_t bindTo() const { return _bindTo; }
    uint16_t port() const { return _port; }
};

struct MakeListener
{
    Network::BindToPort binder;
    DNS::ResolveName resolver;
    Protocol* protocol;

    MakeListener(uint16_t po, String ho, Protocol* pr):
        protocol(pr)
    {
        binder.port = po;
        resolver.set(this, sResolved, ho);
        resolver.submit();
    }

    void resolved()
    {
        Debug("resolved");
        binder.set(this, sBound, resolver.host, binder.port);
        binder.submit();
    }

    void bound()
    {
        Debug("bound");
    }

static void sResolved(MakeListener* ml, DNS::ResolveName* resolver)
    {
        ml->resolved();
    }

static void sBound(MakeListener* ml, Network::BindToPort* resolver)
    {
        ml->bound();
    }
};

int addListener(JSON_Config::Value const &lConfig)
{
    if (!lConfig.isObject()) return 1;
    JSON_Config::Value const &tmp(lConfig.member(port));
    JSON_Config::Value const &hos(lConfig.member(bindTo));
    JSON_Config::Value const &pro(lConfig.member(protocol));

    if (!tmp.isInteger()) return 1;
    if (!hos.isString() && !hos.isNull()) return 1;
    if (!pro.isString() && !pro.isArray()) return 1;

    int po = tmp.integerValue();

    if (po <= 0 || po >= 65536)
    {
        return 1;
    }

    Protocol* proto = Protocol::instantiate(pro);
    if (!proto)
        return 1;

    if (hos.isNull())
        new Listener(0, po, proto);
    else
        new MakeListener(po, hos.stringValue(), proto);
    return 0;
}

int configure(Configuration const& cfg)
{
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
    return 0;
}

}}
