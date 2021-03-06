// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

#include <mstrlg/Everything.h>

namespace MasterLog {

static const char *defaultConfigFile;
static const int MAX_CONFIG_FILE_SIZE = 40960;

Configuration::Configuration(const char *def)
{ // we are singleton!?
    defaultConfigFile = def;
}

static int valOverVal(JSON_Config::Value* obj, JSON_Config::Value const &val);

static int arrayExtendsArray(JSON_Config::Value* obj, JSON_Config::Value const &val)
{
    int n = val.count();
    for (int i = 0; i < n; ++i)
    {
        JSON_Config::Value tmp;
        int r = valOverVal(&tmp, val.element(i));
        if (r)
            return r;
        obj->appendElement(tmp);
    }
    return 0;
}

static int objectExtendsObject(JSON_Config::Value* obj, JSON_Config::Value const &val)
{
    int r = 0;
    const unsigned nKeys = val.keys(NULL, 0);
    String* keys = new String[nKeys];
    val.keys(keys, nKeys);
    for (unsigned i = 0; i < nKeys; ++i)
    {
        String& k(keys[i]);
        int l = k.length();
        JSON_Config::Value const &subVal(val.member(k));
        if (l <= 8 || ::memcmp(k.value(), "extends:", 8))
        {
            JSON_Config::Value tmp;
            r = valOverVal(&tmp, subVal);
            if (r)
                return r;
            obj->setMember(k, tmp);
            continue;
        }
        if (subVal.type() == JSON_Config::Type::JSNULL)
            continue;
        k.fromText(k.value() + 8, l - 8);
        JSON_Config::Value subObj(obj->member(k));
        if (subObj.type() == JSON_Config::Type::JSNULL)
            r = valOverVal(&subObj, subVal);
        else if (subObj.type() == JSON_Config::Type::JSARRAY)
        {
            if (subVal.type() == JSON_Config::Type::JSARRAY)
                r = arrayExtendsArray(&subObj, subVal);
            else
            {
                JSON_Config::Value tmp;
                r = valOverVal(&tmp, subVal);
                if (r)
                    return r;
                obj->member(k).appendElement(subVal);
                continue;
            }
        }
        else if (subVal.type() == JSON_Config::Type::JSARRAY)
        {
            JSON_Config::Value tmp(subObj);
            subObj = JSON_Config::Value(JSON_Config::Type::JSARRAY);
            subObj.appendElement(tmp);
            r = arrayExtendsArray(&subObj, subVal);
        }
        else if (subObj.type() == JSON_Config::Type::JSOBJECT)
        {
            if (subVal.type() != JSON_Config::Type::JSOBJECT)
            {
                ErrorLog("Cannot extend an object with a non-object: %.*s", k.length(), k.value());
                return 1;
            }
            r = objectExtendsObject(&subObj, subVal);
        }
        else
        {
            ErrorLog("Cannot extend a non-object: %.*s", k.length(), k.value());
            return 1;
        }
        if (r)
            return r;
        obj->setMember(k, subObj);
    }
    delete[] keys;
    return r;
}

// assume they have the same "name"; replace values
static int valOverVal(JSON_Config::Value* obj, JSON_Config::Value const &val)
{
    if (val.type() == JSON_Config::Type::JSOBJECT)
    {
        *obj = JSON_Config::Value(JSON_Config::Type::JSOBJECT);
        return objectExtendsObject(obj, val);
    }
    if (val.type() == JSON_Config::Type::JSARRAY)
    {
        *obj = JSON_Config::Value(JSON_Config::Type::JSARRAY);
        return arrayExtendsArray(obj, val);
    }
    *obj = val;
    return 0;
}

// assume they have the same "name"; replace values
static int valExtendsVal(JSON_Config::Value* obj, JSON_Config::Value const &val)
{
    if (val.type() != JSON_Config::Type::JSOBJECT)
    {
        Error("Configuration file must hold an object");
        return 1;
    }
    if (obj->type() != JSON_Config::Type::JSOBJECT)
        *obj = JSON_Config::Value(JSON_Config::Type::JSOBJECT);
    return objectExtendsObject(obj, val);
}

static int applyConfigFile(Configuration* obj, const char *fname)
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
        JSON_Config::Value config;
        JSON_Config::ErrorContext context(fname);
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
            r = valExtendsVal(obj, config);
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
