// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

#include <mstrlg/Everything.h>

namespace MasterLog {

const char defaultConfigFile[] = "/etc/masterlog.conf";
static const int MAX_CONFIG_FILE_SIZE = 40960;

Configuration::Configuration():Map(Help::exeName, &help, &identification, &listeners, &connectors, &diskLogs, NULL)
{
};

static const char *applyApply(JSON::JSON_Value const& val, ConfigVar::Map* map);

static ConfigVar::I* convert(String const& name, JSON::JSON_Value const& val)
{
    JSON::Type::Instance typ = val.type();
    switch(typ)
    {
    case JSON::Type::JSNULL:
    case JSON::Type::JSARRAY:
        Error("Crashing!");
        return 0;
    case JSON::Type::JSINTEGER:
        return new ConfigVar::Integer(name, val.integerValue());
    case JSON::Type::JSSTRING:
        return new ConfigVar::CString(name, val.stringValue());
    case JSON::Type::JSNUMBER:
        return new ConfigVar::Number(name, val.numberValue());
    case JSON::Type::JSBOOL:
        return new ConfigVar::Bool(name, val.boolValue());
    case JSON::Type::JSOBJECT: {
            ConfigVar::Map* map = new ConfigVar::Map(name);
            const char *p = applyApply(val, map);
            if (p)
            {
                ErrorLog("Uncaught error: %s", p);
            }
            return map;
        }
    }
    ErrorLog("Crashing on type %d", typ);
    return 0;
}

static const char *applyApply(ConfigVar::I* obj, JSON::JSON_Value const& val);
static const char *applyApply(JSON::JSON_Value const& val, ConfigVar::Array* arr)
{
    int c = arr->count;
    for (int i = 0; i < c; ++i)
        delete arr->values[i];
    delete[] arr->values;

    JSON::Type::Instance typ = val.type();
    switch(typ)
    {
    case JSON::Type::JSARRAY:
        c = val.count();
        arr->values = c ? new ConfigVar::I*[c] : 0;
        arr->count = c;
        for (int i = 0; i < c; ++i)
        {
            JSON::JSON_Value const& ele(val.element(i));
            if (!ele.isNull())
            {
                if (ele.type() == JSON::Type::JSARRAY)
                    return "Illegal nested arrays";
                arr->values[i] = convert(arr->name, ele);
            }
        }
        return 0;
    case JSON::Type::JSINTEGER:
    case JSON::Type::JSSTRING:
    case JSON::Type::JSNUMBER:
    case JSON::Type::JSBOOL:
        break;
    case JSON::Type::JSNULL:
    case JSON::Type::JSOBJECT:
        arr->values = 0;
        arr->count = 0;
        return "Type mismatch";
    }
    arr->values = new ConfigVar::I*[1];
    arr->count = 1;
    arr->values[0] = convert(arr->name, val);
    return 0;
}

static const char *applyApply(JSON::JSON_Value const& val, ConfigVar::Map* map)
{
    const int count = map->count;
    ConfigVar::I** values = map->values;
    for (int i = 0; i < count; ++i)
    {
        ConfigVar::I* v = values[i];
        assert(v->name.length() > 0);
        JSON::JSON_Value jv = val.member(v->name);
        if (!jv.isNull())
        {
            DebugLog("Applying config value: %.*s", v->name.length(), v->name.value());
            const char *ret = applyApply(v, jv);
            if (ret)
                return ret;
        }
        else
        {
            DebugLog("Config value unconfigured: %.*s", v->name.length(), v->name.value());
        }
    }
    return 0;
}

static const char *applyApply(ConfigVar::I* obj, JSON::JSON_Value const& val)
{
    JSON::Type::Instance typ = val.type();
    if (typ == JSON::Type::JSNULL)
        return 0;
    switch(obj->type)
    {
    case ConfigVar::Type::FORBIDDEN:
        return "Forbidden";
    case ConfigVar::Type::INTEGER:
        switch(typ)
        {
        case JSON::Type::JSINTEGER:
            static_cast<ConfigVar::Integer*>(obj)->value = val.integerValue();
            return 0;
        case JSON::Type::JSNULL:
        case JSON::Type::JSOBJECT:
        case JSON::Type::JSARRAY:
        case JSON::Type::JSSTRING:
        case JSON::Type::JSNUMBER:
        case JSON::Type::JSBOOL:
            return "Type mismatch";
        }
        break;
    case ConfigVar::Type::STRING:
        switch(typ)
        {
        case JSON::Type::JSSTRING:
            static_cast<ConfigVar::CString*>(obj)->value = val.stringValue();
            return 0;
        case JSON::Type::JSNULL:
        case JSON::Type::JSOBJECT:
        case JSON::Type::JSARRAY:
        case JSON::Type::JSNUMBER:
        case JSON::Type::JSINTEGER:
        case JSON::Type::JSBOOL:
            return "Type mismatch";
        }
        break;
    case ConfigVar::Type::ARRAY:
        return applyApply(val, static_cast<ConfigVar::Array*>(obj));
    case ConfigVar::Type::MAP:
        switch(typ)
        {
        case JSON::Type::JSOBJECT:
            return applyApply(val, static_cast<ConfigVar::Map*>(obj));
        case JSON::Type::JSNULL:
        case JSON::Type::JSARRAY:
        case JSON::Type::JSSTRING:
        case JSON::Type::JSNUMBER:
        case JSON::Type::JSINTEGER:
        case JSON::Type::JSBOOL:
            break;
        }
        break;
    case ConfigVar::Type::PATH:
        switch(typ)
        {
        case JSON::Type::JSSTRING:
            static_cast<ConfigVar::CString*>(obj)->value = val.stringValue();
            return 0;
        case JSON::Type::JSNULL:
        case JSON::Type::JSOBJECT:
        case JSON::Type::JSARRAY:
        case JSON::Type::JSNUMBER:
        case JSON::Type::JSINTEGER:
        case JSON::Type::JSBOOL:
            return "Type mismatch";
        }
        break;
    case ConfigVar::Type::DATE:
        switch(typ)
        {
        case JSON::Type::JSSTRING:
            static_cast<ConfigVar::CString*>(obj)->value = val.stringValue();
            return 0;
        case JSON::Type::JSNULL:
        case JSON::Type::JSOBJECT:
        case JSON::Type::JSARRAY:
        case JSON::Type::JSNUMBER:
        case JSON::Type::JSINTEGER:
        case JSON::Type::JSBOOL:
            return "Type mismatch";
        }
        break;
    case ConfigVar::Type::BOOL:
        switch(typ)
        {
        case JSON::Type::JSINTEGER:
            if (val.integerValue() & ~1)
            {
                if (val.integerValue() != -1)
                    return "Type mismatch";
                static_cast<ConfigVar::Bool*>(obj)->value = false;
            }
            else
                static_cast<ConfigVar::Bool*>(obj)->value = val.integerValue();
            return 0;
        case JSON::Type::JSNULL:
            static_cast<ConfigVar::Bool*>(obj)->value = false;
            return 0;
        case JSON::Type::JSBOOL:
            static_cast<ConfigVar::Bool*>(obj)->value = val.boolValue();
            return 0;
        case JSON::Type::JSSTRING:
            if (val.stringValue() == "false")
            {
                static_cast<ConfigVar::Bool*>(obj)->value = false;
                return 0;
            }
            if (val.stringValue() == "true")
            {
                static_cast<ConfigVar::Bool*>(obj)->value = true;
                return 0;
            }
        case JSON::Type::JSOBJECT:
        case JSON::Type::JSARRAY:
        case JSON::Type::JSNUMBER:
            return "Type mismatch";
        }
        break;
    case ConfigVar::Type::NUMBER:
        switch(typ)
        {
        case JSON::Type::JSINTEGER:
            static_cast<ConfigVar::Number*>(obj)->value = val.integerValue();
            return 0;
        case JSON::Type::JSNUMBER:
            static_cast<ConfigVar::Number*>(obj)->value = val.numberValue();
            return 0;
        case JSON::Type::JSNULL:
        case JSON::Type::JSOBJECT:
        case JSON::Type::JSARRAY:
        case JSON::Type::JSSTRING:
        case JSON::Type::JSBOOL:
            return "Type mismatch";
        }
        break;
    }
    return "<software error>";
}

static int applyConfigFile(ConfigVar::I* obj, const char *fname)
{
    int r = 0;
    FILE* fp = ::fopen(fname, "r");
    if (!fp)
    {
        ErrorLog("Failed opening \"%s\": %m", fname);
        return 1;
    }

    char *buffer = new char[MAX_CONFIG_FILE_SIZE];
    int len = ::fread(buffer, 1, MAX_CONFIG_FILE_SIZE, fp);
    if (len < 0)
    {
        ErrorLog("Failed reading \"%s\": %m", fname);
        r = 1;
    }
    if (len == MAX_CONFIG_FILE_SIZE)
    {
        if (getc(fp) >= 0)
        {
            ErrorLog("Config file exceeds %u bytes: \"%s\"", MAX_CONFIG_FILE_SIZE, fname);
            r = 1;
        }
    }
    ::fclose(fp);
    if (!r)
    {
        JSON::JSON_Value config;
        JSON::ErrorContext context;
        int ret = config.initialize(buffer, 0, len, &context);
        if (ret < 0)
        {
            if (context.lineNumber)
            {
                ErrorLog("Config file line %d could not be parsed: \"%s\"", context.lineNumber, fname);
                ErrorLog("%s", context.errorMessage);
                ErrorLog("%.*s", context.lineLength, context.linePointer);
                char *x = new char[context.byteOffset+2];
                ::memcpy(x, context.linePointer, context.byteOffset);
                for (int i = 0; i < context.byteOffset; ++i)
                {
                    if (!isspace(x[i]))
                        x[i] = ' ';
                }
                x[context.byteOffset] = '^';
                x[context.byteOffset + 1] = 0;
                ErrorLog("%s", x);
                delete[] x;
            }
            else
                ErrorLog("Config file could not be parsed: \"%s\"", fname);
            r = 1;
        }
        else
        {
            config.print(stderr);
            const char *p = applyApply(obj, config);
            if (p)
            {
                ErrorLog("Config file \"%s\" could not be applied: %s", fname, p);
                r = 1;
            }
        }
    }
    delete[] buffer;
    return r;
}

int Configuration::initialize(int argc, char **argv)
{
    bool cfg = false;
    int r = 0;
    for (int i = 0; argv[++i];)
    {
        const char *p = argv[i];
        if (p[0] == '-')
        {
            switch(p[1])
            {
            case '-':
                if (!::strncmp(p, "--file", 6))
                {
                    const char *configFile = 0;
                    if (p[6] == '=')
                    {
                        configFile = &p[7];
                    }
                    else if (!p[6] && argv[i+1])
                    {
                        configFile = argv[++i];
                    }
                    if (configFile)
                    {
                        cfg = true;
                        if (applyConfigFile(this, configFile))
                        {
                            r = 1;
                            ErrorLog("Failed using config file: \"%s\"", configFile);
                        }
                        continue;
                    }
                }
                break;
            case 'f':
                cfg = true;
                if (argv[i+1] && applyConfigFile(this, argv[++i]))
                {
                    r = 1;
                    ErrorLog("Failed using config file: \"%s\"", argv[i]);
                }
                continue;
            }
        }
        ErrorLog("Unrecognized option: \"%s\"", p);
    }
    if (!r && !cfg)
        r = applyConfigFile(this, defaultConfigFile);
    return r;
}

}
