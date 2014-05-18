// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// API for the JSON parsing library.

namespace MasterLog {
namespace JSON {

namespace Type {
    enum Instance {
        JSNULL,
        JSOBJECT,
        JSARRAY,
        JSSTRING,
        JSNUMBER,
        JSINTEGER,
        JSBOOL
    };
}

class Value
{
    Type::Instance _type;
    intptr_t _value;
public:
    Value();
    Value(const Value&);
    Value& operator=(const Value&);
    ~Value();

    // How we use this to be determined...
    //int initialize(const char *buffer, int off, int maxl, ErrorContext* ctxt);

    Type::Instance type() const { return _type; }

    bool isNull() const;
    bool boolValue() const;
    int64_t integerValue() const;
    double numberValue() const;
    String stringValue() const;

    unsigned count() const;
    const Value& element(unsigned index) const;

    const Value& member(String const& name) const;

    void print(FILE* fp) const;
    void print(FILE*fp, int depth) const;
};

}} // JSON : MasterLog
