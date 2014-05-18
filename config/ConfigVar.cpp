// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

#include <mstrlg/Everything.h>

namespace MasterLog {
namespace ConfigVar {

Forbidden::Forbidden(const char *nm):I(nm,Type::FORBIDDEN){}
Forbidden::Forbidden(String const& nm):I(nm,Type::FORBIDDEN){}
void Forbidden::printValue(FILE* fp, unsigned depth)
{
}

Bool::Bool(const char *nm, bool v):I(nm,Type::BOOL),value(v){}
Bool::Bool(String const& nm, bool v):I(nm,Type::BOOL),value(v){}
void Bool::printValue(FILE* fp, unsigned depth)
{
    fputs((value ? "true" : "false"), fp);
}

Number::Number(const char *nm, double v):I(nm,Type::NUMBER),value(v){}
Number::Number(String const& nm, double v):I(nm,Type::NUMBER),value(v){}
void Number::printValue(FILE* fp, unsigned depth)
{
    fprintf(fp, "%g", value);
}

Integer::Integer(const char *nm, int64_t v):I(nm,Type::INTEGER),value(v){}
Integer::Integer(String const& nm, int64_t v):I(nm,Type::INTEGER),value(v){}
void Integer::printValue(FILE* fp, unsigned depth)
{
    fprintf(fp, "%lld", static_cast<long long>(value));
}

CString::CString(const char *nm, const char* v):I(nm,Type::STRING),value(v){}
CString::CString(String const& nm, String const& v):I(nm,Type::STRING),value(v){}
void CString::printValue(FILE* fp, unsigned depth)
{
    fprintf(fp, "%.*s", value.length(), value.value());
}

Path::Path(const char *nm, const char* v):I(nm,Type::PATH),value(v){}
Path::Path(String const& nm, String const& v):I(nm,Type::PATH),value(v){}
void Path::printValue(FILE* fp, unsigned depth)
{
    fprintf(fp, "%.*s", value.length(), value.value());
}

Date::Date(const char *nm, const char* v):I(nm,Type::DATE),value(v){}
Date::Date(String const& nm, String const& v):I(nm,Type::DATE),value(v){}
void Date::printValue(FILE* fp, unsigned depth)
{
    fprintf(fp, "%.*s", value.length(), value.value());
}

Array::Array(const char *nm, Type::Combining allowed, I* first, ...):I(nm,Type::ARRAY),values(NULL),count(0)
{
}

void Array::printValue(FILE* fp, unsigned depth)
{
    fprintf(fp, "[");

    for (unsigned i = 0; i < count; ++i)
    {
        if (i)
            putc(',', fp);
        putc('\n', fp);
        unsigned l = depth + 1;
        while(l--)
            putc(' ', fp);
        values[i]->printValue(fp, depth);
    }

    if (count)
    {
        putc('\n', fp);
        unsigned l = depth;
        while(l--)
            putc(' ', fp);
    }
    putc(']', fp);
    fflush(fp);
}

Map::Map(String const& nm):I(nm,Type::MAP),values(NULL),count(0){}
Map::Map(const char *nm, I* first, ...):I(nm,Type::MAP),values(NULL),count(0)
{
    if (!first)
        return;
    int c = 1;
    va_list ap;
    va_start(ap, first);
    while (va_arg(ap, I*))
        c += 1;
    va_end(ap);
    values = new I*[c];
    I*t = first;
    va_start(ap, first);
    do
    {
        values[count++] = t;
        t = va_arg(ap, I*);
    } while(t);
    va_end(ap);
}

void Map::printValue(FILE* fp, unsigned depth)
{
    putc('{', fp);

    for (unsigned i = 0; i < count; ++i)
    {
        if (i)
            putc(',', fp);
        putc('\n', fp);
        unsigned l = depth + 1;
        while(l--)
            putc(' ', fp);
        fprintf(fp, "%.*s : ", values[i]->name.length(), values[i]->name.value());
        values[i]->printValue(fp, depth + 1);
    }

    if (count)
    {
        putc('\n', fp);
        unsigned l = depth;
        while(l--)
            putc(' ', fp);
    }
    putc('}', fp);
}

void Map::print(FILE* fp)
{
    printValue(fp, 0);
    putc('\n', fp);
    fflush(fp);
}

}} // ConfigVar : MasterLog
