// 2025/6/11 18:47:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/SPI/SPI.h"

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

namespace SPI
{
    // Внутренние переменные модуля
    static int g_spi_fd = -1;  
    static uint32_t g_speed = 1000000;
    static uint8_t g_mode = 0; // SPI_MODE_0
    static uint8_t g_bits_per_word = 8;

    // GPIO пины (физические номера пинов)
    static const unsigned int EN_DDA1_PIN = 31;  // GPIO6
    static const unsigned int EN_DDA2_PIN = 35;  // GPIO19

    // libgpiod объекты
#ifdef HAVE_LIBGPIOD

    struct gpiod_chip *g_gpio_chip = nullptr;

    // Массив GPIO линий для DAC (индекс 0 = DAC1, индекс 1 = DAC2)
    const int MAX_DAC_COUNT = 2;
    struct gpiod_line *g_dac_lines[MAX_DAC_COUNT] = {nullptr, nullptr};

    // Массив физических пинов для каждого DAC
    const unsigned int DAC_PINS[MAX_DAC_COUNT] = {
        EN_DDA1_PIN,  // DAC1 -> физический пин 31 (GPIO6)
        EN_DDA2_PIN   // DAC2 -> физический пин 35 (GPIO19)
    };

    // Массив имен для GPIO линий
    const char* DAC_NAMES[MAX_DAC_COUNT] = {
        "SPI_EN_DDA1",
        "SPI_EN_DDA2"
    };

    // Преобразование физического номера пина в номер GPIO
    static unsigned int PhysicalToGPIO(unsigned int physical_pin);

#else

    // Константы для Windows (заглушки)
    const int MAX_DAC_COUNT = 2;
    const unsigned int DAC_PINS[MAX_DAC_COUNT] = {31, 35}; // Заглушки
    const char* DAC_NAMES[MAX_DAC_COUNT] = {"SPI_EN_DDA1", "SPI_EN_DDA2"};
    
    // Преобразование физического номера пина в номер GPIO
    static unsigned int PhysicalToGPIO(unsigned int physical_pin);

#endif

    static bool g_gpio_initialized = false;

    static bool InitGPIO();

    static void DeInitGPIO();

    static void SetCS(int dac_number, bool enable);

    static bool Write(uint8_t *data, size_t length);
}

void SPI::Init()
{
    const char *device = "/dev/spidev0.0";

    // Открываем SPI устройство
    g_spi_fd = ::open(device, O_RDWR);
    if (g_spi_fd < 0)
    {
        std::cerr << "Error: Cannot open SPI device: " << device << std::endl;
        return;
    }

    // Настройка режима SPI
    if (ioctl(g_spi_fd, SPI_IOC_WR_MODE, &g_mode) < 0)
    {
        std::cerr << "Error: Cannot set SPI mode" << std::endl;
        ::close(g_spi_fd);
        g_spi_fd = -1;
        return;
    }

    // Настройка битов на слово
    if (ioctl(g_spi_fd, SPI_IOC_WR_BITS_PER_WORD, &g_bits_per_word) < 0)
    {
        std::cerr << "Error: Cannot set bits per word" << std::endl;
        ::close(g_spi_fd);
        g_spi_fd = -1;
        return;
    }

    // Настройка скорости
    if (ioctl(g_spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &g_speed) < 0)
    {
        std::cerr << "Error: Cannot set SPI speed" << std::endl;
        ::close(g_spi_fd);
        g_spi_fd = -1;
        return;
    }

    // Инициализация GPIO
    if (!InitGPIO())
    {
        ::close(g_spi_fd);
        g_spi_fd = -1;
        return;
    }

    std::cout << "SPI initialized successfully on " << device << std::endl;
}

void SPI::DeInit()
{
    if (g_spi_fd >= 0)
    {
        // Отключаем все CS используя цикл
        for (int i = 1; i <= MAX_DAC_COUNT; i++)
        {
            SetCS(i, false);
        }

        ::close(g_spi_fd);
        g_spi_fd = -1;
        std::cout << "SPI deinitialized" << std::endl;
    }

    // Освобождаем GPIO ресурсы
    DeInitGPIO();
}

// Функция для записи в динамический DAC
bool SPI::WriteDynamicDAC(int number_DAC, uint16_t value)
{
    if (!IsReady())
    {
        std::cerr << "Error: SPI not ready" << std::endl;
        return false;
    }

    // Проверка корректности номера DAC
    if (number_DAC < 1 || number_DAC > 2)
    {
        std::cerr << "Error: Invalid DAC number: " << number_DAC << ". Valid range: 1-2" << std::endl;
        return false;
    }

    // Подготовка данных (MSB первым)
    uint8_t data[2];
    data[0] = static_cast<uint8_t>((value >> 8) & 0xFF);
    data[1] = static_cast<uint8_t>(value & 0xFF);

    // Активируем CS для указанного DAC
    SetCS(number_DAC, true);
    usleep(1);

    // Передаем данные
    bool result = Write(data, 2);

    if (result)
    {
        std::cout << "DAC" << number_DAC << " written: 0x" << std::hex << value << std::dec << std::endl;
    }

    usleep(1);
    SetCS(number_DAC, false);

    return result;
}

bool SPI::SetSpeed(uint32_t speedHz)
{
    g_speed = speedHz;

    if (IsReady())
    {
        if (ioctl(g_spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &g_speed) < 0)
        {
            std::cerr << "Error: Cannot set SPI speed to " << speedHz << " Hz" << std::endl;
            return false;
        }
        std::cout << "SPI speed set to " << speedHz << " Hz" << std::endl;
    }

    return true;
}


bool SPI::SetMode(uint8_t mode)
{
    g_mode = mode;

    if (IsReady())
    {
        if (ioctl(g_spi_fd, SPI_IOC_WR_MODE, &g_mode) < 0)
        {
            std::cerr << "Error: Cannot set SPI mode to " << static_cast<int>(mode) << std::endl;
            return false;
        }
        std::cout << "SPI mode set to " << static_cast<int>(mode) << std::endl;
    }

    return true;
}


bool SPI::IsReady()
{
    return g_spi_fd >= 0 && g_gpio_initialized;
}


uint32_t SPI::GetSpeed()
{
    return g_speed;
}


uint8_t SPI::GetMode()
{
    return g_mode;
}

#ifdef HAVE_LIBGPIOD

unsigned int SPI::PhysicalToGPIO(unsigned int physical_pin)
{
    // Маппинг физических пинов на GPIO для Orange Pi
    switch (physical_pin)
    {
    // \error Магические числа
    case 31: return 6;   // Physical pin 31 -> GPIO6
    case 35: return 19;  // Physical pin 35 -> GPIO19
    default: return physical_pin; // Fallback
    }
}

#else

unsigned int SPI::PhysicalToGPIO(unsigned int physical_pin)
{
    return physical_pin; // Fallback для Windows
}

#endif

bool SPI::InitGPIO()
{
    if (g_gpio_initialized)
        return true;

    // Проверяем доступность GPIO библиотеки
#ifndef HAVE_LIBGPIOD
    std::cout << "Warning: GPIO library (libgpiod) not available - GPIO control disabled" << std::endl;
    g_gpio_initialized = true;  // Отмечаем как инициализированный, но без GPIO
    return true;
#else
    // Открываем GPIO chip (обычно gpiochip0 для Orange Pi)
    g_gpio_chip = gpiod_chip_open_by_name("gpiochip0");
    if (!g_gpio_chip)
    {
        std::cerr << "Error: Cannot open GPIO chip" << std::endl;
        return false;
    }

    // Инициализируем все DAC линии
    for (int i = 0; i < MAX_DAC_COUNT; i++)
    {
        unsigned int gpio_num = PhysicalToGPIO(DAC_PINS[i]);
        
        g_dac_lines[i] = gpiod_chip_get_line(g_gpio_chip, gpio_num);
        if (!g_dac_lines[i])
        {
            std::cerr << "Error: Cannot get GPIO line for DAC" << (i + 1) << std::endl;
            // Очищаем уже инициализированные линии
            for (int j = 0; j < i; j++)
            {
                if (g_dac_lines[j])
                {
                    gpiod_line_release(g_dac_lines[j]);
                    g_dac_lines[j] = nullptr;
                }
            }
            gpiod_chip_close(g_gpio_chip);
            g_gpio_chip = nullptr;
            return false;
        }

        // Настраиваем линию как выход с начальным значением LOW
        if (gpiod_line_request_output(g_dac_lines[i], DAC_NAMES[i], 0) < 0)
        {
            std::cerr << "Error: Cannot request GPIO line " << DAC_NAMES[i] << " as output" << std::endl;
            // Очищаем все линии
            for (int j = 0; j <= i; j++)
            {
                if (g_dac_lines[j])
                {
                    gpiod_line_release(g_dac_lines[j]);
                    g_dac_lines[j] = nullptr;
                }
            }
            gpiod_chip_close(g_gpio_chip);
            g_gpio_chip = nullptr;
            return false;
        }
    }

    g_gpio_initialized = true;
    std::cout << "GPIO initialized successfully using libgpiod" << std::endl;
    return true;
#endif
}


void SPI::DeInitGPIO()
{
#ifdef HAVE_LIBGPIOD
    // Освобождаем все GPIO линии
    for (int i = 0; i < MAX_DAC_COUNT; i++)
    {
        if (g_dac_lines[i])
        {
            gpiod_line_release(g_dac_lines[i]);
            g_dac_lines[i] = nullptr;
        }
    }
    
    if (g_gpio_chip)
    {
        gpiod_chip_close(g_gpio_chip);
        g_gpio_chip = nullptr;
    }
#endif
    g_gpio_initialized = false;
}


void SPI::SetCS(int dac_number, bool enable)
{
    if (!g_gpio_initialized)
        return;

#ifdef HAVE_LIBGPIOD
    // Проверяем корректность номера DAC и преобразуем в индекс массива
    if (dac_number < 1 || dac_number > MAX_DAC_COUNT)
    {
        std::cerr << "Error: Invalid DAC number: " << dac_number << std::endl;
        return;
    }

    int dac_index = dac_number - 1;  // Преобразуем номер DAC (1,2) в индекс массива (0,1)
    int value = enable ? 1 : 0;

    if (g_dac_lines[dac_index])
    {
        gpiod_line_set_value(g_dac_lines[dac_index], value);
    }
    else
    {
        std::cerr << "Error: GPIO line for DAC" << dac_number << " not initialized" << std::endl;
    }
#else
    // Заглушка - ничего не делаем без libgpiod
    (void)dac_number;
    (void)enable;
#endif
}

bool SPI::Write(uint8_t *data, size_t length)
{
    if (g_spi_fd < 0)
    {
        std::cerr << "Error: SPI not initialized" << std::endl;
        return false;
    }

    if (data == nullptr || length == 0)
    {
        std::cerr << "Error: Invalid data or length" << std::endl;
        return false;
    }

    struct spi_ioc_transfer transfer = {};
    transfer.tx_buf = (unsigned long)data;
    transfer.rx_buf = 0;
    transfer.len = length;
    transfer.speed_hz = g_speed;
    transfer.delay_usecs = 0;
    transfer.bits_per_word = g_bits_per_word;
    transfer.cs_change = 0;

    int result = ioctl(g_spi_fd, SPI_IOC_MESSAGE(1), &transfer);
    if (result < 0)
    {
        std::cerr << "Error: SPI transfer failed" << std::endl;
        return false;
    }

    return true;
}


#else

#endif
