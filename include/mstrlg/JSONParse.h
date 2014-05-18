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

struct ErrorContext
{
    ErrorContext():errorMessage("<none>"),linePointer(""),lineLength(0),byteOffset(0),lineNumber(0){}
    const char *errorMessage;
    const char *linePointer;
    int lineLength;
    int byteOffset;
    int lineNumber;
};

class JSON_Value
{
    Type::Instance _type;
    intptr_t _value;
public:
    JSON_Value();
    JSON_Value(const JSON_Value&);
    JSON_Value& operator=(const JSON_Value&);
    ~JSON_Value();

    // returns number of bytes consumed, -1 if parsing fails.
    int initialize(const char *buffer, int off, int maxl, ErrorContext* ctxt);

    Type::Instance type() const { return _type; }

    bool isNull() const;
    bool boolValue() const;
    int64_t integerValue() const;
    double numberValue() const;
    String stringValue() const;

    unsigned count() const;
    const JSON_Value& element(unsigned index) const;

    const JSON_Value& member(String const& name) const;

    void print(FILE* fp) const;
    void print(FILE*fp, int depth) const;
};

}} // JSON : MasterLog
