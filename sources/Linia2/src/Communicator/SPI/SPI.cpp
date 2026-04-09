// 2026/04/09 10:07:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/SPI/SPI.h"

// Системные заголовочные файлы
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <cstring>
#include <errno.h>

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4996)
#endif

namespace SPI
{
    // Файловые дескрипторы для каждого аппаратного чипселекта
    static int fd_cs0 = -1; // Для DAC1 (CS0, контакт 24)
    static int fd_cs1 = -1; // Для DAC2 (CS1, контакт 26)

    static uint speed = SPI_SPEED;
    static uint8 mode = 0;
    static uint8 bits_per_word = 8;

    // Пути к устройствам для разных чипселектов
    const char *device_cs0 = "/dev/spidev0.0"; // CS0
    const char *device_cs1 = "/dev/spidev0.1"; // CS1

    // Вспомогательные функции
    static bool SetSpeed(int fd, uint speedHz);
    static bool SetMode(int fd, uint8 mode);
    static bool Write(int fd, uint8 *data, size_t length);
    static int OpenSPIDevice(const char *device);

    int OpenSPIDevice(const char *device)
    {
        int fd = ::open(device, O_RDWR);
        if (fd < 0)
        {
            LOG_ERROR("Cannot open SPI device: %s (errno: %d)", device, errno);
            return -1;
        }

        // Настройка режима SPI
        if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
        {
            LOG_ERROR("Cannot set SPI mode for %s", device);
            ::close(fd);
            return -1;
        }

        // Настройка количества бит на слово
        if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0)
        {
            LOG_ERROR("Cannot set bits per word for %s", device);
            ::close(fd);
            return -1;
        }

        // Настройка максимальной скорости
        if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
        {
            LOG_ERROR("Cannot set SPI speed for %s", device);
            ::close(fd);
            return -1;
        }

        LOG_WRITE("SPI device %s opened and configured successfully", device);
        return fd;
    }

    void Init()
    {
        LOG_WRITE("Initializing SPI with hardware CS...");

        // Открываем устройство для CS0 (DAC1)
        fd_cs0 = OpenSPIDevice(device_cs0);

        // Открываем устройство для CS1 (DAC2)
        fd_cs1 = OpenSPIDevice(device_cs1);

        if (fd_cs0 < 0 && fd_cs1 < 0)
        {
            LOG_ERROR("Failed to open any SPI device");
            return;
        }

        // Устанавливаем режим работы (CPOL=0, CPHA=1)
        SetMode(fd_cs0, 1);
        SetMode(fd_cs1, 1);

        // Устанавливаем скорость
        SetSpeed(fd_cs0, SPI_SPEED);
        SetSpeed(fd_cs1, SPI_SPEED);

        LOG_WRITE("SPI initialized successfully. CS0 fd=%d, CS1 fd=%d", fd_cs0, fd_cs1);
    }

    void DeInit()
    {
        if (fd_cs0 >= 0)
        {
            ::close(fd_cs0);
            fd_cs0 = -1;
            LOG_WRITE("SPI CS0 deinitialized");
        }

        if (fd_cs1 >= 0)
        {
            ::close(fd_cs1);
            fd_cs1 = -1;
            LOG_WRITE("SPI CS1 deinitialized");
        }
    }

    // Запись 16-битного значения в динамический DAC через SPI
    bool WriteDynamicDAC(int number_DAC, uint16 value)
    {
        if (!IsReady())
        {
            LOG_ERROR("SPI not ready");
            return false;
        }

        if (number_DAC < 1 || number_DAC > 2)
        {
            LOG_ERROR("Invalid DAC number: %d. Valid range: 1-2", number_DAC);
            return false;
        }

        // Выбираем правильный файловый дескриптор на основе номера DAC
        int fd = (number_DAC == 1) ? fd_cs0 : fd_cs1;

        if (fd < 0)
        {
            LOG_ERROR("SPI device for DAC%d is not open", number_DAC);
            return false;
        }

        uint8 data[2];
        data[0] = static_cast<uint8>((value >> 8) & 0xFF);
        data[1] = static_cast<uint8>(value & 0xFF);

        // Аппаратный CS управляется ядром автоматически при использовании
        // правильного файла устройства
        bool result = Write(fd, data, 2);

        return result;
    }

    bool SetSpeed(int fd, uint speedHz)
    {
        if (fd < 0) return false;

        if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
        {
            LOG_ERROR("Cannot set SPI speed to %u Hz", speedHz);
            return false;
        }

        LOG_WRITE("SPI speed set to %u Hz", speedHz);
        return true;
    }

    bool SetMode(int fd, uint8 _mode)
    {
        if (fd < 0) return false;

        if (ioctl(fd, SPI_IOC_WR_MODE, &_mode) < 0)
        {
            LOG_ERROR("Cannot set SPI mode to %d", (int)_mode);
            return false;
        }

        LOG_WRITE("SPI mode set to %d", (int)_mode);
        return true;
    }

    bool IsReady()
    {
        return (fd_cs0 >= 0) || (fd_cs1 >= 0);
    }

    bool Write(int fd, uint8 *data, size_t length)
    {
        if (fd < 0)
        {
            LOG_ERROR("SPI file descriptor invalid");
            return false;
        }

        if (data == nullptr || length == 0)
        {
            LOG_ERROR("Invalid data or length");
            return false;
        }

        struct spi_ioc_transfer transfer = {};
        transfer.tx_buf = (unsigned long long)data;
        transfer.rx_buf = 0;
        transfer.len = (uint32_t)length;
        transfer.speed_hz = speed;
        transfer.delay_usecs = 0;
        transfer.bits_per_word = bits_per_word;
        transfer.cs_change = 0; // Не меняем CS после этого transfer'а

        int result = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
        if (result < 0)
        {
            LOG_ERROR("SPI transfer failed: %s", strerror(errno));
            return false;
        }

        return true;
    }
}

bool SPI::IsAvailability()
{
#ifdef ARM64
    return std::filesystem::exists("/dev/spidev0.0") ||
        std::filesystem::exists("/dev/spidev0.1");
#else
    return true;
#endif
}

#ifdef WIN32
#pragma warning(pop)
#endif
