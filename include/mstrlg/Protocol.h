// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Protocols

namespace MasterLog {

class Protocol
{
    String const _name;
    Protocol* next;

public:
    Protocol(String const& nm);
    virtual ~Protocol();

    String const& name() const { return _name; }
static Protocol* instantiate(JSON_Config::Value const &);
};

} // MasterLog
