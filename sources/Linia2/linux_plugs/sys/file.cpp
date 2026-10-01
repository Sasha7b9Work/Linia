#include "file.h"


#ifdef _WIN32

int flock(int /*fd*/, int /*operation*/)
{
    return 0;
}

#endif
