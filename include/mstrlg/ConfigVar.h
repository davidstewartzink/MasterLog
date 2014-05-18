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
        BOOL = 1 << 6,
        NUMBER = 1 << 7,
    };

    typedef unsigned Combining;
}

struct I
{
    I(const char *nm, Type::Instance ti):name(nm),type(ti){}
    I(String const &nm, Type::Instance ti):name(nm),type(ti){}
    String name;
    const Type::Instance type;

    virtual void printValue(FILE*, unsigned depth) = 0;
};

struct Forbidden : I
{
    Forbidden(const char *nm);
    Forbidden(String const&nm);

    void printValue(FILE*, unsigned depth);
};

struct Integer : I
{
    Integer(const char *nm, int64_t v);
    Integer(String const& nm, int64_t v);
    int64_t value;

    void printValue(FILE*, unsigned depth);
};

struct CString : I
{
    CString(const char *nm, const char* v);
    CString(String const& nm, String const&  v);
    String value;

    void printValue(FILE*, unsigned depth);
};

struct Path : I
{
    Path(const char *nm, const char* v);
    Path(String const& nm, String const&  v);
    String value;

    void printValue(FILE*, unsigned depth);
};

struct Date : I
{
    Date(const char *nm, const char* v);
    Date(String const& nm, String const&  v);
    String value;

    void printValue(FILE*, unsigned depth);
};

struct Array : I
{
    Array(const char *nm, Type::Combining allowed, I* first, ...);
    I ** values;
    unsigned count;

    void printValue(FILE*, unsigned depth);
};

struct Map : I
{
    Map(const char *nm, I* first, ...);
    Map(String const& nm);
    I ** values;
    unsigned count;

    void print(FILE*);
    void printValue(FILE*, unsigned depth);
};

struct Bool : I
{
    Bool(const char *nm, bool v);
    Bool(String const& nm, bool v);
    bool value;

    void print(FILE*);
    void printValue(FILE*, unsigned depth);
};

struct Number : I
{
    Number(const char *nm, double v);
    Number(String const& nm, double v);
    double value;

    void print(FILE*);
    void printValue(FILE*, unsigned depth);
};

}} // ConfigVar : MasterLog
