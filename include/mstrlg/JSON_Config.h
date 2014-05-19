// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// API for the JSON Init parsing library; for config files.

namespace MasterLog {
namespace JSON_Config {

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
    ErrorContext(const char *fn):fileName(fn),errorMessage("<none>"),linePointer(""),lineLength(0),byteOffset(0),lineNumber(0){}
    const char *const fileName;
    const char *errorMessage;
    const char *linePointer;
    int lineLength;
    int byteOffset;
    int lineNumber;
};

class Value
{
    Type::Instance _type;
    intptr_t _value;
    const char *_fileName;
    const char *_linePointer;
    int _lineLength;
    int _byteOffset;
    int _lineNumber;
public:
    Value();
    Value(Type::Instance);
    Value(const Value&);
    Value& operator=(const Value&);
    ~Value();

    // returns number of bytes consumed, -1 if parsing fails.
    int initialize(const char *buffer, int off, int maxl, ErrorContext* ctxt);

    Type::Instance type() const { return _type; }

    bool isNull() const;
    bool boolValue() const;
    int64_t integerValue() const;
    double numberValue() const;
    String stringValue() const;

    unsigned count() const;
    const Value& element(unsigned index) const;
    void appendElement(Value const&) const;
    void appendElements(Value const&) const;

    const Value& member(String const& name) const;
    void setMember(String const& name, Value const&) const;
    unsigned keys(String* keys, unsigned maxKeys) const;

    void print(FILE* fp) const;
    void print(FILE*fp, int depth) const;

    const char *const fileName() const;
    const char *linePointer() const;
    int lineLength() const;
    int byteOffset() const;
    int lineNumber() const;
};

}} // JSON_Config : MasterLog
