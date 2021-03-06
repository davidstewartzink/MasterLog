// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.
//
// API for the JSON parsing library.

#include <mstrlg/Everything.h>

// give up...
#include <map>
#include <vector>

struct ErrorContext;

namespace MasterLog {
namespace JSON {
typedef std::map<String, Value> ObjectMap;

static void printString(const String& string, FILE* fp)
{
    const char *p = string.value();
    int len = string.length();
    putc('"', fp);
    for (int i = 0; i < len; ++i)
    {
        int c = p[i];
        switch(c)
        {
        case '"': fputs("\\\"", fp); continue;
        case '\\': fputs("\\\\", fp); continue;
        case '\n': fputs("\\n", fp); continue;
        case '\r': fputs("\\r", fp); continue;
        case '\t': fputs("\\t", fp); continue;
        case '\f': fputs("\\f", fp); continue;
        case '\b': fputs("\\b", fp); continue;
        default:
            if (c < 0x20)
                fprintf(fp, "\\%04X", c);
            else
                putc(c, fp);
        }
    }
    putc('"', fp);
}

static const Value constNull;

struct IObject
{
    unsigned refCount;
    ObjectMap map;

    IObject():refCount(1)
    {
    }

    ~IObject()
    {
    }

    const Value& member(String const& name) const
    {
        ObjectMap::const_iterator it = map.find(name);
        if (it == map.end())
            return constNull;
        return it->second;
    }
    int initialize(const char *buffer, int off, int maxl);
    void print(FILE*fp, int depth)
    {
        ObjectMap::const_iterator it = map.begin();
        putc('{', fp);
        if (it == map.end())
        {
            putc('}', fp);
            return;
        }
        if (depth >= 0)
        {
            putc('\n', fp);
            depth += 1;
        }
        for (int i = 0; i < depth; ++i)
            putc(' ', fp);
        printString(it->first, fp);
        putc(':', fp);
        it->second.print(fp, depth);
        while (++it != map.end())
        {
            putc(',', fp);
            if (depth >= 0)
                putc('\n', fp);
            for (int i = 0; i < depth; ++i)
                putc(' ', fp);
            printString(it->first, fp);
            putc(':', fp);
            it->second.print(fp, depth);
        }
        if (depth >= 0)
            putc('\n', fp);
        for (int i = 1; i < depth; ++i)
            putc(' ', fp);
        putc('}', fp);
    }
};

struct IArray
{
    unsigned refCount;
    std::vector<Value> values;

    IArray():refCount(1)
    {
    }

    ~IArray()
    {
    }

    const Value& element(unsigned index) const
    {
        if (index < count())
            return values[index];
        return constNull;
    }

    unsigned count() const
    {
        return values.size();
    }

    int initialize(const char *buffer, int off, int maxl);
};

struct IString
{
    unsigned refCount;
    String string;

    IString():refCount(1){}

    int initialize(const char *buffer, int off, int maxl);
};

static inline void incrementRef(Type::Instance type, intptr_t value)
{
    switch(type)
    {
    case Type::JSNULL: return;
    case Type::JSOBJECT: {
        IObject* io = static_cast<IObject*>(reinterpret_cast<void*>(value));
        if (io)
            io->refCount += 1;
        return;
    }
    case Type::JSARRAY: {
        IArray* ia = static_cast<IArray*>(reinterpret_cast<void*>(value));
        if (ia)
            ia->refCount += 1;
        return;
    }
    case Type::JSSTRING: {
        IString* is = static_cast<IString*>(reinterpret_cast<void*>(value));
        if (is)
            is->refCount += 1;
        return;
    }
    case Type::JSNUMBER: return;
    case Type::JSINTEGER: return;
    case Type::JSBOOL: return;
    }
}

static inline void decrementRef(Type::Instance type, intptr_t value)
{
    switch(type)
    {
    case Type::JSNULL: return;
    case Type::JSOBJECT: {
        IObject* io = static_cast<IObject*>(reinterpret_cast<void*>(value));
        if (io && !(io->refCount -= 1))
            delete io;
        return;
    }
    case Type::JSARRAY: {
        IArray* ia = static_cast<IArray*>(reinterpret_cast<void*>(value));
        if (ia && !(ia->refCount -= 1))
            delete ia;
        return;
    }
    case Type::JSSTRING: {
        IString* is = static_cast<IString*>(reinterpret_cast<void*>(value));
        if (is && !(is->refCount -= 1))
            delete is;
        return;
    }
    case Type::JSNUMBER: return;
    case Type::JSINTEGER: return;
    case Type::JSBOOL: return;
    }
}

Value::Value():_type(Type::JSNULL){}

Value::Value(const Value&rhs):_type(rhs._type),_value(rhs._value)
{
    incrementRef(_type, _value);
}

Value& Value::operator=(const Value& rhs)
{
    if (_value != rhs._value || _type != rhs._type)
    {
        incrementRef(rhs._type, rhs._value);
        decrementRef(_type, _value);
        _type = rhs._type;
        _value = rhs._value;
    }
    return *this;
}

Value::~Value()
{
    decrementRef(_type, _value);
}

bool Value::isNull() const
{
    return _type == Type::JSNULL;
}

bool Value::boolValue() const
{
    assert(_type == Type::JSBOOL);
    return _value;
}

int64_t Value::integerValue() const
{
    assert(_type == Type::JSINTEGER);
    return _value;
}

double Value::numberValue() const
{
    assert(_type == Type::JSNUMBER || _type == Type::JSINTEGER);
    return *reinterpret_cast<const double*>(&_value);
}

String Value::stringValue() const
{
    assert(_type == Type::JSSTRING);
    IString* is = static_cast<IString*>(reinterpret_cast<void*>(_value));
    assert(is);
    return is->string;
}

const Value& Value::member(String const& name) const
{
    assert(_type == Type::JSOBJECT);
    IObject* io = static_cast<IObject*>(reinterpret_cast<void*>(_value));
    assert(io);
    return io->member(name);
}

const Value& Value::element(unsigned index) const
{
    assert(_type == Type::JSARRAY);
    IArray* ia = static_cast<IArray*>(reinterpret_cast<void*>(_value));
    assert(ia);
    return ia->element(index);
}

unsigned Value::count(void) const
{
    assert(_type == Type::JSARRAY);
    IArray* ia = static_cast<IArray*>(reinterpret_cast<void*>(_value));
    assert(ia);
    return ia->count();
}

int parseCopyNumber(Type::Instance* ptype, intptr_t* pval, const char *buffer, int off, int maxl)
{
    int ln = maxl - off;
    char *x = new char[ln + 1];
    ::memcpy(x, &buffer[off], ln);
    x[ln] = 0;
    char *e;
    errno = 0;
    *reinterpret_cast<double*>(pval) = ::strtod(x, &e);
    if (errno)
    {
        delete[] x;
        return -1;
    }
    delete[] x;
    *ptype = Type::JSNUMBER;
    return off + (e - x);
}

int parseNumber(Type::Instance* ptype, intptr_t* pval, const char *buffer, int off, int maxl)
{
    // this is the weird one; the last character in the buf can be a digit. Not a real case, though.
    int toff = off;

    if (buffer[toff] == '-')
    {
        if (++toff == maxl || !isdigit(buffer[toff]))
            return -1;
    }
    if (buffer[toff] == '0')
    {
        if (++toff == maxl)
        {
            *ptype = Type::JSINTEGER;
            *pval = 0;
            return toff;
        }
        switch(buffer[toff])
        {
        case '.':
            goto parseNumber;
        case 'e':
        case 'E':
            break;
        default:
            *ptype = Type::JSINTEGER;
            *pval = 0;
            return toff;
        }
        if (++toff == maxl)
            return -1;
        int c = buffer[toff];
        if (c == '-' || c == '+')
        {
            if (++toff == maxl)
                return -1;
            c = buffer[toff];
        }
        if (!isdigit(c))
            return -1;
        *ptype = Type::JSINTEGER;
        *pval = 0;
        while (++toff < maxl && isdigit(buffer[toff]))
        {
        }
        return toff;
    }


    for (;;)
    {
        if (toff == maxl)
        {
            int ln = maxl - off;
            char *x = new char[ln + 1];
            ::memcpy(x, &buffer[off], ln);
            x[ln] = 0;
            char *e;
            *ptype = Type::JSINTEGER;
            *pval = ::strtoll(x, &e, 10);
            delete[] x;
            return off + (e - x);
        }
        int c = buffer[toff];
        if (isdigit(c))
        {
            ++toff;
            continue;
        }
        else if (c == '.')
            break;
        else if (c == 'e' || c == 'E')
            goto parseExponent;
        char *e;
        *ptype = Type::JSINTEGER;
        *pval = ::strtoll(&buffer[off], &e, 10);
        return e - buffer;
    }
parseNumber:
    if (++toff == maxl)
    {
        char *e;
        *pval = ::strtoll(&buffer[off], &e, 10);
        if (*e != '.')
            return -1;
        *ptype = Type::JSINTEGER;
        return e - buffer;
    }
    while (isdigit(buffer[toff]))
    {
        if (++toff == maxl)
            return parseCopyNumber(ptype, pval, buffer, off, maxl);
    }
    if (buffer[toff] == 'e' || buffer[toff] == 'E')
    {
parseExponent:
        if (++toff == maxl || !isdigit(buffer[toff]))
            return -1;
        for (;;)
        {
            if (++toff == maxl)
                return parseCopyNumber(ptype, pval, buffer, off, maxl);
            if (!isdigit(buffer[toff]))
                break;
        }
    }
    char *e;
    errno = 0;
    *reinterpret_cast<double*>(pval) = ::strtod(&buffer[off], &e);
    if (errno)
        return -1;
    *ptype = Type::JSNUMBER;
    return e - buffer;
}

static int parseComment(const char *buffer, int off, int maxl)
{
    if (off == maxl)
    {
        return -1;
    }
    int c = buffer[off++];
    if (c == '/')
    {
        while (off < maxl)
        {
            c = buffer[off++];
            if (c == '\n')
                break;
            if (!c)
            {
                return -1;
            }
        }
        return off;
    }
    if (c != '*')
    {
        return -1;
    }
    for (;;)
    {
        if (off == maxl || !buffer[off])
        {
            return -1;
        }
        if (buffer[off++] == '*')
        {
            if (off == maxl)
            {
                return -1;
            }
            if (buffer[off] == '/')
                break;
        }
    }
    return off + 1;
}

static int parseString(char *target, int* targetLen, int maxTarget, const char *buffer, int off, int maxl)
{
    int toff = 0;
    while (off < maxl)
    {
        int c = buffer[off++];
        switch(c)
        {
        case '"':
            if (toff < maxTarget)
                target[toff] = 0;
            *targetLen = toff;
            return off;
        case '\\':
            if (off == maxl)
                goto unterminated;
            c = buffer[off++];
            switch(c)
            {
            case '"':
            case '\\':
            case '/':
                break;
            case 'r':
                c = 0x000D;
                break;
            case 't':
                c = 0x0009;
                break;
            case 'n':
                c = 0x000A;
                break;
            case 'f':
                c = 0x000C;
                break;
            case 'b':
                c = 0x0008;
                break;
            default:
                if (isxdigit(c))
                {
                    if (off + 3 >= maxl)
                        goto unterminated;
                    char x[5];
                    ::memcpy(x, &buffer[off-1], 4);
                    x[4] = 0;
                    char *f;
                    c = strtol(x, &f, 16);
                    if (x + 4 == f)
                        break;
                }
                return -1;
            }
            break;
        default:
            if (c < 0x20)
            {
                if (c >= 0)
                {
                    return -1;
                }
            }
        }
        if (toff < maxTarget)
            target[toff] = c;
        toff += 1;
    }
unterminated:
    return -1;
}

int IString::initialize(const char *buffer, int off, int maxl)
{ // JSON standard: MUST be '"'
    // single parse if short; length and fill if long.
    char buf[100];
    int blen = 0;
    int noff = parseString(buf, &blen, int(sizeof(buf)), buffer, off, maxl);
    if (noff < 0)
        return noff;
    if (blen < int(sizeof(buf)))
    {
        string.fromText(buf, blen);
        return noff;
    }
    char *t = new char[blen];
    off = parseString(t, &blen, blen, buffer, off, maxl);
    assert(off == noff);
    delete[] t; // extra copy!!
    return off;
}

int IObject::initialize(const char *buffer, int off, int maxl)
{
    while (off < maxl)
    {
        int c = buffer[off++];
        if (isspace(c))
            continue;
        if (c != '"')
        {
            if (c == '}')
                return off;
            if (c == '/')
            {
                off = parseComment(buffer, off, maxl);
                if (off < 0)
                    return off;
                continue;
            }
            return -1;
        }
        IString current;
        off = current.initialize(buffer, off, maxl);
        if (off < 0)
            return off;
        while (off < maxl)
        {
            c = buffer[off++];
            if (isspace(c))
                continue;
            if (c == ':')
                break;
            if (c != '/')
            {
                return -1;
            }
            off = parseComment(buffer, off, maxl);
            if (off < 0)
                return off;
        }
        ObjectMap::iterator it = map.find(current.string);
        if (it == map.end())
        {
            ObjectMap::value_type v(current.string, Value());
            off = v.second.initialize(buffer, off, maxl);
            if (off < 0)
                return off;
            map.insert(v);
        }
        else
        {
            off = it->second.initialize(buffer, off, maxl);
            if (off < 0)
                return off;
        }
        while (off < maxl)
        {
            c = buffer[off++];
            if (c == ',')
                break;
            if (c == '}')
                return off;
            if (isspace(c))
                continue;
            if (c == '/')
            {
                off = parseComment(buffer, off, maxl);
                if (off < 0)
                    return off;
                continue;
            }
            return -1;
        }
    }
    return -1;
}

int IArray::initialize(const char *buffer, int off, int maxl)
{
    bool needComma = false;

    while (off < maxl)
    {
        int c = buffer[off++];
        if (isspace(c))
            continue;
        if (c == ']')
            return off;
        if (needComma)
        {
            if (c == ',')
            {
                needComma = false;
                continue;
            }
            if (c != '/')
            {
                return -1;
            }
            off = parseComment(buffer, off, maxl);
            if (off < 0)
                return off;
            continue;
        }
        Value v;
        off = v.initialize(buffer, off - 1, maxl);
        if (off < 0)
            return off;
        values.push_back(v);
        needComma = true;
    }
    return -1;
}

void Value::print(FILE* fp, int depth) const
{
    switch(_type)
    {
    case Type::JSNULL:
        fputs("null", fp);
        return;
    case Type::JSOBJECT: {
        IObject* io = static_cast<IObject*>(reinterpret_cast<void*>(_value));
        io->print(fp, depth);
        return;
    }
    case Type::JSARRAY: {
        int n = count();
        putc('[', fp);
        if (!n)
        {
            putc(']', fp);
            return;
        }
        if (depth >= 0)
        {
            putc('\n', fp);
            depth += 1;
        }
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < depth; ++j)
                putc(' ', fp);
            element(i).print(fp, depth);
            if (i + 1 < n)
                putc(',', fp);
            if (depth >= 0)
                putc('\n', fp);
        }
        for (int j = 1; j < depth; ++j)
            putc(' ', fp);
        putc(']', fp);
        return;
    }
    case Type::JSSTRING: {
        printString(stringValue(), fp);
        return;
    }
    case Type::JSNUMBER:
        fprintf(fp, "%g", numberValue());
        return;
    case Type::JSINTEGER:
        fprintf(fp, "%lld", static_cast<long long>(integerValue()));
        return;
    case Type::JSBOOL:
        fputs((boolValue() ? "true" : "false"), fp);
        return;
    }
    fputs("<error>", fp);
}

void Value::print(FILE* fp) const
{
    print(fp, 0);
    putc('\n', fp);
    fflush(fp);
}

}} // JSON : MasterLog
