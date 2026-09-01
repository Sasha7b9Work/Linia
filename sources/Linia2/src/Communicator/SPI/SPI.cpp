// 2026/09/01 16:45:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Communicator/SPI/SPI.h"
#include "Communicator/GPIO/GPIO.h"
#include <filesystem>


// Lin specific
#include <gpiod.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>


/*

    ПЛИС: SPI0 CS0 M2
    21 - spi_miso   GPIO1_B1 SPI0_MISO_M2
    23 - spi_clk    GPIO1_B3 SPI0_CLK_M2
    24 - spi_cs     GPIO1_B4 SPI0_CS0_M2

    DAC1, DAC2:
    16 - spi_clk    GPIO3_B5
    18 - spi_mosi   GPIO3_B6
    31 - spi_cs     GPIO3_A0    (формирователь развёртки)
    35 - spi_cs     GPIO3_A2    (источник 50В)
*/


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4365 4996)
#endif


namespace SPI
{
    static int fd_cs0 = -1;                             // для CS0 (пин 24)
    static uint speed = SPI_SPEED;
    static uint8 mode = 0;
    static uint8 bits_per_word = 8;

    static const char *device_cs0 = SPI_DEVICE;         // "/dev/spidev0.0"

    static PinOut pinCLK(Pin::DDAC_CLK_18_out);
    static PinOut pinMOSI(Pin::DDAC_MOSI_16_out);
    static PinOut pinCS0_SCAN(Pin::DDAC_CS0_31_out);    // Формирователь развёртки
    static PinOut pinCS1_50V(Pin::DDAC_CS1_35_out);     // Источник 50В

    static bool SetSpeed(uint speedHz);
    static bool SetMode(uint8 mode);

    static void DelayUS(uint timeUS);
}

void SPI::Init()
{
    pinCS0_SCAN.ToHi();
    pinCS1_50V.ToHi();
    pinCLK.ToLow();
    pinMOSI.ToLow();

    LOG_WRITE("Initializing SPI with hardware CS...");

    fd_cs0 = ::open(device_cs0, O_RDWR);
    if (fd_cs0 < 0)
    {
        LOG_ERROR("Cannot open SPI device %s", device_cs0);
    }
    else
    {
        if (ioctl(fd_cs0, SPI_IOC_WR_MODE, &mode) < 0 ||
            ioctl(fd_cs0, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0 ||
            ioctl(fd_cs0, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
        {
            LOG_ERROR("Cannot configure CS0");
            ::close(fd_cs0);
            fd_cs0 = -1;
        }
    }

    if (fd_cs0 >= 0) LOG_WRITE("SPI CS0 (pin 24) initialized");

    SPI::SetSpeed(SPI_SPEED);
    SPI::SetMode(1);

    LOG_WRITE("SPI initialized successfully");
}


void SPI::DeInit()
{
    if (fd_cs0 >= 0)
    {
        ::close(fd_cs0);
        fd_cs0 = -1;
    }

    LOG_WRITE("SPI deinitialized");
}


void SPI::DelayUS(uint timeUS)
{
    (void)timeUS;

#ifdef WIN32
#else
    struct timespec start, now;
    // Получаем текущее время
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Целевое время: старт + timeUS микросекунд (timeUS * 1000 наносекунд)
    long target_ns = start.tv_nsec + (timeUS * 1000);

    // Корректируем перенос секунд, если наносекунды перевалили за 1e9
    if (target_ns >= 1000000000)
    {
        target_ns -= 1000000000;
        start.tv_sec += 1;
    }

    // Активно ждем, пока не наступит нужный момент
    do
    {
        clock_gettime(CLOCK_MONOTONIC, &now);
    } while (now.tv_sec < start.tv_sec || (now.tv_sec == start.tv_sec && now.tv_nsec < target_ns));
#endif
}


bool SPI::WriteDynamicDAC(DAC::E dac, uint16 value)
{
    if (dac != DAC::_0_ChannelC_Form && dac != DAC::_6_Source_50V)
    {
        return false;
    }

    (dac == DAC::_0_ChannelC_Form) ? pinCS0_SCAN.ToLow() : pinCS1_50V.ToLow();

    uint16 mask = 0x8000;

    for (int i = 0; i < 16; i++)
    {
        DelayUS(4);

        pinCLK.ToHi();

        DelayUS(4);

        pinMOSI.Set((mask & value) != 0);
        mask >>= 1;

        DelayUS(4);

        pinCLK.ToLow();

        DelayUS(4);

        pinMOSI.ToLow();
    }

    (dac == DAC::_0_ChannelC_Form) ? pinCS0_SCAN.ToHi() : pinCS1_50V.ToHi();

    return true;
}


bool SPI::ReadFPGA(uint8 *data, size_t length)
{
    static uint8 value = (uint8)std::rand();

    for (size_t i = 0; i < length; i++)
    {
        data[i] = value++;
    }

    return true;

    if (data == nullptr || length == 0)
    {
        LOG_ERROR("Invalid data or length");
        return false;
    }

    if (fd_cs0 < 0)
    {
        LOG_ERROR("SPI device not initialized");
        return false;
    }

    // Создаем буфер для отправки (обычно нули или 0xFF для чтения)
    // При SPI чтении мы одновременно отправляем данные (обычно 0x00)
    uint8 *tx_buffer = new uint8[length]();
    if (tx_buffer == nullptr)
    {
        LOG_ERROR("Failed to allocate TX buffer");
        return false;
    }

    // Заполняем нулями (можно заменить на 0xFF если нужно)
    memset(tx_buffer, 0x00, length);

    struct spi_ioc_transfer transfer = {};
    transfer.tx_buf = (unsigned long long)tx_buffer;
    transfer.rx_buf = (unsigned long long)data;
    transfer.len = (uint)length;
    transfer.speed_hz = speed;
    transfer.delay_usecs = 0;
    transfer.bits_per_word = bits_per_word;
    transfer.cs_change = 0;                 // 0 - CS деактивируется после передачи

    int result = ioctl(fd_cs0, SPI_IOC_MESSAGE(1), &transfer);

    delete[] tx_buffer;

    if (result < 0)
    {
        LOG_ERROR("SPI read failed");
        return false;
    }

    return true;
}


bool SPI::SetSpeed(uint speedHz)
{
    speed = speedHz;
    bool success = true;

    if (fd_cs0 >= 0)
    {
        if (ioctl(fd_cs0, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
        {
            LOG_ERROR("Cannot set speed for CS0");
            success = false;
        }
    }

    if (success) LOG_WRITE("SPI speed set to %u Hz", speedHz);
    return success;
}

bool SPI::SetMode(uint8 _mode)
{
    mode = _mode;
    bool success = true;

    if (fd_cs0 >= 0)
    {
        if (ioctl(fd_cs0, SPI_IOC_WR_MODE, &mode) < 0)
        {
            LOG_ERROR("Cannot set mode for CS0");
            success = false;
        }
    }

    if (success) LOG_WRITE("SPI mode set to %d", (int)mode);
    return success;
}


bool SPI::IsReady()
{
    return (fd_cs0 >= 0);
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
