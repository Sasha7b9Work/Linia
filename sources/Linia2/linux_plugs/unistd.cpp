#include "unistd.h"


#ifdef _WIN32

int usleep(int /*usec*/)
{
    return 0;
}

#endif
