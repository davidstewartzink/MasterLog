// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

#include <mstrlg/Everything.h>

namespace MasterLog {

struct String::I
{
    I(const char *p, int len):refCount(1),value(new char[len+1]),length(len)
    {
        ::memcpy(value, p, len);
        value[len] = 0;
    }

    unsigned refCount;
    char *value;
    unsigned length;

static inline void decrementRef(String::I* p)
{
    if (p && !(p->refCount -= 1))
        delete p;
}

static inline void incrementRef(String::I* p)
{
    if (p)
        p->refCount += 1;
}
};

String::String(const char *p):mi(p ? new I(p, ::strlen(p)):NULL)
{
}

String::String():mi(NULL)
{
}

String::String(const String& rhs):mi(rhs.mi)
{
    I::incrementRef(mi);
}

String& String::operator=(const String&rhs)
{
    if (mi != rhs.mi)
    {
        I::incrementRef(rhs.mi);
        I::decrementRef(mi);
        mi = rhs.mi;
    }
    return *this;
}

String::~String()
{
    I::decrementRef(mi);
}

const char *String::value() const
{
    return mi ? mi->value : NULL;
}

unsigned String::length() const
{
    return mi ? mi->length : 0;
}

void String::fromText(const char *p, int len)
{
    I::decrementRef(mi);
    mi = new I(p, len);
}

bool String::operator == (const String&rhs) const
{
    I* rmi = rhs.mi;
    if (!mi)
        return !rmi;
    if (!rmi)
        return false;

    if (mi->length != rmi->length)
        return false;
    return !::memcmp(mi->value, rmi->value, mi->length);
}

bool String::operator == (const char *rhs) const
{
    if (!mi)
        return !rhs;
    if (!rhs)
        return false;

    return !::strncmp(mi->value, rhs, mi->length) && !rhs[mi->length];
}

bool String::operator < (const String&rhs) const
{
    I* rmi = rhs.mi;
    if (!mi)
        return rmi;
    if (!rmi)
        return false;

    if (mi->length < rmi->length)
        return ::memcmp(mi->value, rmi->value, mi->length) <= 0;
    return ::memcmp(mi->value, rmi->value, rmi->length) < 0;
}

} // String : MasterLog
