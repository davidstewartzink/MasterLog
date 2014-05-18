// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

#include <mstrlg/Everything.h>

namespace MasterLog {

const char defaultConfigFile[] = "/etc/masterlog.conf";
static const int MAX_CONFIG_FILE_SIZE = 40960;

Configuration::Configuration()
{
}

static int applyConfigFile(Configuration* obj, const char *fname)
{
    return -1;
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
