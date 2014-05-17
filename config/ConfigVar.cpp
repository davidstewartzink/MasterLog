// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

#include <mstrlg/Platform.h>
#include <mstrlg/ConfigVar.h>

namespace MasterLog {
namespace ConfigVar {

Forbidden::Forbidden(const char *nm):I(nm,Type::FORBIDDEN){}

Integer::Integer(const char *nm, int64_t v):I(nm,Type::INTEGER),value(v){}

String::String(const char *nm, const char* v):I(nm,Type::STRING),value(v){}

Path::Path(const char *nm, const char* v):I(nm,Type::PATH),value(v){}

Date::Date(const char *nm, const char* v):I(nm,Type::DATE),value(v){}

}} // ConfigVar : MasterLog
