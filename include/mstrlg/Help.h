// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Help and related services

namespace MasterLog {
namespace Help {

extern const char *exeName;

struct Config : ConfigVar::Map
{
    Config();
};

int initialize(int argc, char **argv);

void switchToRunning();

}} // Help : MasterLog

#define Error(F) fprintf(stderr, "%s ERROR: " F "\n", Help::exeName)
#define ErrorLog(F, P...) fprintf(stderr, "%s ERROR: " F "\n", Help::exeName, P)
#define Debug(F) fprintf(stderr, "%s: " F "\n", Help::exeName)
#define DebugLog(F, P...) fprintf(stderr, "%s: " F "\n", Help::exeName, P)
