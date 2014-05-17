// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// API for the basic configuration library.

namespace MasterLog {
namespace ConfigVar {

namespace Type {
    // c++2011 fixes this...
    enum Instance {
        FORBIDDEN = 0,
        INTEGER = 1 << 0,
        STRING = 1 << 1,
        ARRAY = 1 << 2,
        MAP = 1 << 3,
        PATH = 1 << 4,
        DATE = 1 << 5,
    };

    typedef unsigned Combining;
}

struct I
{
    I(const char *nm, Type::Instance ti):name(nm),type(ti){}
    const char * const name;
    const Type::Instance type;
};

struct Forbidden : I
{
    Forbidden(const char *nm);
};

struct Integer : I
{
    Integer(const char *nm, int64_t v);
    int64_t value;
};

struct String : I
{
    String(const char *nm, const char* v);
    const char * value;
};

struct Path : I
{
    Path(const char *nm, const char* v);
    const char * value;
};

struct Date : I
{
    Date(const char *nm, const char* v);
    const char * value;
};

struct Array : I
{
    Array(const char *nm, Type::Combining allowed, I* first, ...);
    I ** values;
    unsigned count;
};

struct Map : I
{
    Map(const char *nm, I* first, ...);
    I ** values;
    unsigned count;
};

}} // ConfigVar : MasterLog
