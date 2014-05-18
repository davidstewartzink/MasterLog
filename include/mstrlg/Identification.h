// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Identification for this server/service

namespace MasterLog {
namespace Identification {

struct Config : ConfigVar::Map
{
    ConfigVar::CString system;
    ConfigVar::CString user;

    Config();
};

}} // Identification : MasterLog
