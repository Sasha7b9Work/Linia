#include "Communicator/SPI/SPI.h"

#ifdef WIN32
namespace SPI
{
    void Init() {}
    
    void DeInit() {}
    
    bool WriteDynamicDAC(int /*number_DAC*/, uint16_t /*value*/) 
    { 
        return false; 
    }
    
    bool SetSpeed(uint32_t /*speedHz*/) 
    { 
        return false; 
    }
    
    bool SetMode(uint8_t /*mode*/) 
    { 
        return false; 
    }
    
    bool IsReady() 
    { 
        return false; 
    }
    
    uint32_t GetSpeed() 
    { 
        return 0; 
    }
    
    uint8_t GetMode() 
    { 
        return 0; 
    }
}

#else

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <iostream>
#include <cstring>

#ifdef ARM64
#include <gpiod.h>
#endif

namespace SPI
{
    static int g_spi_fd = -1;                     
    static uint32_t g_speed = 100000;             
    static uint8_t g_mode = 0;                    
    static uint8_t g_bits_per_word = 8;           
    static bool g_gpio_initialized = false;       

    const char *device = "/dev/spidev0.0"; // Путь к SPI устройству

    const int MAX_DAC_COUNT = 2;                  

#ifdef ARM64
    const char *gpio_chip_name = "gpiochip3";                        // Имя GPIO чипа для ARM64
    struct gpiod_chip *g_gpio_chip = nullptr;                        // Дескриптор GPIO чипа
    struct gpiod_line *g_dac_lines[MAX_DAC_COUNT] = {nullptr, nullptr}; // Линии GPIO для каждого DAC

    const unsigned int DAC_GPIO_NUMS[MAX_DAC_COUNT] = {
        0,  // GPIO пин для DAC (pin. 31)
        2   // GPIO пин для DAC #2 (pin. 35)
    };

    // Имена GPIO линий для отладки
    const char* DAC_NAMES[MAX_DAC_COUNT] = {
        "SPI_EN_DDA1",  // Включение DAC #1
        "SPI_EN_DDA2"   // Включение DAC #2
    };
#endif

    static bool InitGPIO();                           
    static void DeInitGPIO();                         
    static void SetCS(int dac_number, bool enable);   // Управление CS (Chip Select) для конкретного DAC
    static bool Write(uint8_t *data, size_t length);  

    void Init()
    {
        

        g_spi_fd = ::open(device, O_RDWR);
        if (g_spi_fd < 0)
        {
            std::cerr << "Error: Cannot open SPI device: " << device << std::endl;
            return;
        }

        if (ioctl(g_spi_fd, SPI_IOC_WR_MODE, &g_mode) < 0)
        {
            std::cerr << "Error: Cannot set SPI mode" << std::endl;
            ::close(g_spi_fd);
            g_spi_fd = -1;
            return;
        }

        if (ioctl(g_spi_fd, SPI_IOC_WR_BITS_PER_WORD, &g_bits_per_word) < 0)
        {
            std::cerr << "Error: Cannot set bits per word" << std::endl;
            ::close(g_spi_fd);
            g_spi_fd = -1;
            return;
        }

        if (ioctl(g_spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &g_speed) < 0)
        {
            std::cerr << "Error: Cannot set SPI speed" << std::endl;
            ::close(g_spi_fd);
            g_spi_fd = -1;
            return;
        }

        if (!InitGPIO())
        {
            ::close(g_spi_fd);
            g_spi_fd = -1;
            return;
        }

        std::cout << "SPI initialized successfully on " << device << std::endl;
    }

    void DeInit()
    {
        if (g_spi_fd >= 0)
        {
            for (int i = 1; i <= MAX_DAC_COUNT; i++)
            {
                SetCS(i, false);
            }

            ::close(g_spi_fd);
            g_spi_fd = -1;
            std::cout << "SPI deinitialized" << std::endl;
        }

        DeInitGPIO();
    }

    // Запись 16-битного значения в динамический DAC через SPI
    // number_DAC: номер DAC (1 или 2)
    // value: 16-битное значение для записи (0x0000-0xFFFF)
    // Возвращает: true если запись успешна, false при ошибке
    bool WriteDynamicDAC(int number_DAC, uint16_t value)
    {
        if (!IsReady())
        {
            std::cerr << "Error: SPI not ready" << std::endl;
            return false;
        }

        if (number_DAC < 1 || number_DAC > 2)
        {
            std::cerr << "Error: Invalid DAC number: " << number_DAC << ". Valid range: 1-2" << std::endl;
            return false;
        }

        uint8_t data[2];
        data[0] = static_cast<uint8_t>((value >> 8) & 0xFF);
        data[1] = static_cast<uint8_t>(value & 0xFF);

        SetCS(number_DAC, true);
        usleep(1);

        bool result = Write(data, 2);

        if (result)
        {
            std::cout << "DAC" << number_DAC << " written: 0x" << std::hex << value << std::dec << std::endl;
        }

        usleep(1);
        SetCS(number_DAC, false);

        return result;
    }

    // Установка скорости SPI интерфейса
    bool SetSpeed(uint32_t speedHz)
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

    // Установка режима SPI (полярность и фаза тактового сигнала)
    // mode: режим SPI (0-3: 0=CPOL=0,CPHA=0; 1=CPOL=0,CPHA=1; 2=CPOL=1,CPHA=0; 3=CPOL=1,CPHA=1)
    // Возвращает: true если режим установлен успешно, false при ошибке
    bool SetMode(uint8_t mode)
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

    // Проверка готовности SPI к работе
    // Возвращает: true если SPI устройство открыто и готово к работе, false если закрыто
    bool IsReady()
    {
        return g_spi_fd >= 0 && g_gpio_initialized;
    }

    // Получение текущей скорости SPI
    // Возвращает: скорость в Герцах
    uint32_t GetSpeed()
    {
        return g_speed;
    }

    // Получение текущего режима SPI
    // Возвращает: режим SPI (0-3)
    uint8_t GetMode()
    {
        return g_mode;
    }

#ifdef ARM64
    bool InitGPIO()
    {
        if (g_gpio_initialized)
            return true;

        g_gpio_chip = gpiod_chip_open_by_name(gpio_chip_name);
        if (!g_gpio_chip)
        {
            std::cerr << "Error: Cannot open " << gpio_chip_name << std::endl;
            return false;
        }
        
        std::cout << "Opened " << gpio_chip_name << " for GPIO3_A0 and GPIO3_A2" << std::endl;

        for (int i = 0; i < MAX_DAC_COUNT; i++)
        {
            unsigned int gpio_num = DAC_GPIO_NUMS[i];
            
            std::cout << "Initializing " << DAC_NAMES[i] 
                      << " (GPIO" << gpio_num << " in gpiochip3)" << std::endl;
            
            g_dac_lines[i] = gpiod_chip_get_line(g_gpio_chip, gpio_num);
            if (!g_dac_lines[i])
            {
                std::cerr << "Error: Cannot get GPIO line " << gpio_num << " for " << DAC_NAMES[i] << std::endl;

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

            if (gpiod_line_request_output(g_dac_lines[i], DAC_NAMES[i], 0) < 0)
            {
                std::cerr << "Error: Cannot request GPIO line " << DAC_NAMES[i] << " as output" << std::endl;

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
    }

    void DeInitGPIO()
    {
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
        g_gpio_initialized = false;
    }

    // Внутренняя функция: управление CS (Chip Select) для конкретного DAC
    // dac_number: номер DAC (1 или 2)
    // enable: true = активировать CS (LOW), false = деактивировать CS (HIGH)
    void SetCS(int dac_number, bool enable)
    {
        if (!g_gpio_initialized)
            return;

        if (dac_number < 1 || dac_number > MAX_DAC_COUNT)
        {
            std::cerr << "Error: Invalid DAC number: " << dac_number << std::endl;
            return;
        }

        int dac_index = dac_number - 1;
        int value = enable ? 1 : 0;

        if (g_dac_lines[dac_index])
        {
            gpiod_line_set_value(g_dac_lines[dac_index], value);
        }
        else
        {
            std::cerr << "Error: GPIO line for DAC" << dac_number << " not initialized" << std::endl;
        }
    }

    bool Write(uint8_t *data, size_t length)
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
    bool InitGPIO()
    {
        g_gpio_initialized = true;
        return true;
    }

    void DeInitGPIO()
    {
        g_gpio_initialized = false;
    }

    void SetCS(int dac_number, bool enable)
    {
    }

    bool Write(uint8_t *data, size_t length)
    {
        return false;
    }

#endif 

}

#endif