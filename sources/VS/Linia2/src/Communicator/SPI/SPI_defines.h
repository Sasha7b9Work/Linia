// 2025/08/15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
// Отдельные определения для SPI модуля без зависимости от wxWidgets
#pragma once

#ifndef WIN32
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <iostream>
#include <cstring>

// Попытка подключить libgpiod, если доступна
#ifdef HAVE_LIBGPIOD
#include <gpiod.h>
#endif

#endif // WIN32
