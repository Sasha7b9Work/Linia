#include "defines.h"
#include "Device.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <fcntl.h>
    #include <termios.h>
    #include <sys/select.h>
#endif

#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <cstring>


IDevice *g_device = nullptr;
