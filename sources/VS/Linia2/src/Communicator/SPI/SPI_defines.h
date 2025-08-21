
#pragma once

#ifndef WIN32
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <iostream>
#include <cstring>

#ifdef HAVE_LIBGPIOD
#include <gpiod.h>
#endif

#endif 
