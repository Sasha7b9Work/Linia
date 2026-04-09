// 2026/04/09 10:03:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/SPI/SPI.h"

// Lin specific
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4996)
#endif

namespace SPI
{
    static int fd = -1;
    static uint speed = SPI_SPEED;
    static uint8 mode = 0;
    static uint8 bits_per_word = 8;

    const char *device = SPI_DEVICE;

    static const int MAX_DAC_COUNT = 2;

    // Маппинг номера DAC на аппаратный CS
    static const uint8 dac_to_cs[MAX_DAC_COUNT] = {
        SPI_CS0,  // DAC #1 использует CS0 (контакт 24)
        SPI_CS1   // DAC #2 использует CS1 (контакт 26)
    };

    static bool SetSpeed(uint speedHz);
    static bool SetMode(uint8 mode);
    static bool Write(uint8 *data, size_t length, uint8 cs_line);

    void Init()
    {
        LOG_WRITE("Initializing SPI with hardware CS...");

        fd = ::open(device, O_RDWR);
        if (fd < 0)
        {
            LOG_ERROR("Cannot open SPI device: %s", device);
            return;
        }

        if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
        {
            LOG_ERROR("Cannot set SPI mode");
            ::close(fd);
            fd = -1;
            return;
        }

        if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0)
        {
            LOG_ERROR("Cannot set bits per word");
            ::close(fd);
            fd = -1;
            return;
        }

        if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
        {
            LOG_ERROR("Cannot set SPI speed");
            ::close(fd);
            fd = -1;
            return;
        }

        SPI::SetSpeed(SPI_SPEED);
        SPI::SetMode(1);  // CPOL=0, CPHA=1 - данные выставляются на срез клока

        LOG_WRITE("SPI initialized successfully on %s with hardware CS0 and CS1", device);
    }

    void DeInit()
    {
        if (fd >= 0)
        {
            ::close(fd);
            fd = -1;
            LOG_WRITE("SPI deinitialized");
        }
    }

    bool WriteDynamicDAC(int number_DAC, uint16 value)
    {
        if (!IsReady())
        {
            LOG_ERROR("SPI not ready");
            return false;
        }

        if (number_DAC < 1 || number_DAC > MAX_DAC_COUNT)
        {
            LOG_ERROR("Invalid DAC number: %d. Valid range: 1-2", number_DAC);
            return false;
        }

        uint8 data[2];
        data[0] = static_cast<uint8>((value >> 8) & 0xFF);
        data[1] = static_cast<uint8>(value & 0xFF);

        // Используем аппаратный CS для выбранного DAC
        uint8 cs_line = dac_to_cs[number_DAC - 1];

        bool result = Write(data, 2, cs_line);

        return result;
    }

    bool SetSpeed(uint speedHz)
    {
        speed = speedHz;

        if (IsReady())
        {
            if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
            {
                LOG_ERROR("Cannot set SPI speed to %u Hz", speedHz);
                return false;
            }

            LOG_WRITE("SPI speed set to %u Hz", speedHz);
        }

        return true;
    }

    bool SetMode(uint8 _mode)
    {
        mode = _mode;

        if (IsReady())
        {
            if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
            {
                LOG_ERROR("Cannot set SPI mode to %d", (int)mode);
                return false;
            }

            LOG_WRITE("SPI mode set to %d", (int)mode);
        }

        return true;
    }

    bool IsReady()
    {
        return fd >= 0;
    }

    bool Write(uint8 *data, size_t length, uint8 cs_line)
    {
        if (fd < 0)
        {
            LOG_ERROR("SPI not initialized");
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
        transfer.len = (uint)length;
        transfer.speed_hz = speed;
        transfer.delay_usecs = 0;
        transfer.bits_per_word = bits_per_word;
        transfer.cs_change = 0;

        // Указываем номер аппаратного чипселекта
        transfer.cs = cs_line;

        int result = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
        if (result < 0)
        {
            LOG_ERROR("SPI transfer failed on CS%d", cs_line);
            return false;
        }

        LOG_WRITE("SPI write success: %d bytes to CS%d", length, cs_line);
        return true;
    }
}

bool SPI::IsAvailability()
{
#ifdef ARM64
    return std::filesystem::exists(SPI_DEVICE);
#else
    return true;
#endif
}

#ifdef WIN32
#pragma warning(pop)
#endif
