// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.
//
// Noting to be proud of

#include <mstrlg/Everything.h>

namespace MasterLog {

Identification::Config::Config():Map("identification", &system, &user, NULL),system("system",NULL),user("user",NULL)
{
}

}
