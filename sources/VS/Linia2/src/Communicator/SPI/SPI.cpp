#include "defines.h"
#include "Communicator/SPI/SPI.h"

#ifdef WIN32
namespace SPI
{
    void Init() {}
    
    void DeInit() {}
    
    bool WriteDynamicDAC(int number_DAC, uint16_t value) 
    { 
        return false; 
    }
    
    bool SetSpeed(uint32_t speedHz) 
    { 
        return false; 
    }
    
    bool SetMode(uint8_t mode) 
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
    static uint32_t g_speed = 115000;
    static uint8_t g_mode = 0;
    static uint8_t g_bits_per_word = 8;
    static bool g_gpio_initialized = false;

    const int MAX_DAC_COUNT = 2;

#ifdef ARM64
    struct gpiod_chip *g_gpio_chip = nullptr;
    struct gpiod_line *g_dac_lines[MAX_DAC_COUNT] = {nullptr, nullptr};

    const unsigned int DAC_GPIO_NUMS[MAX_DAC_COUNT] = {
        0,
        2
    };

    const char* DAC_NAMES[MAX_DAC_COUNT] = {
        "SPI_EN_DDA1",
        "SPI_EN_DDA2"
    };
#endif

    static bool InitGPIO();
    static void DeInitGPIO();
    static void SetCS(int dac_number, bool enable);
    static bool Write(uint8_t *data, size_t length);

    void Init()
    {
        const char *device = "/dev/spidev0.0";

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

    bool IsReady()
    {
        return g_spi_fd >= 0 && g_gpio_initialized;
    }

    uint32_t GetSpeed()
    {
        return g_speed;
    }

    uint8_t GetMode()
    {
        return g_mode;
    }

#ifdef ARM64
    bool InitGPIO()
    {
        if (g_gpio_initialized)
            return true;

        g_gpio_chip = gpiod_chip_open_by_name("gpiochip3");
        if (!g_gpio_chip)
        {
            std::cerr << "Error: Cannot open gpiochip3" << std::endl;
            return false;
        }
        
        std::cout << "Opened gpiochip3 for GPIO3_A0 and GPIO3_A2" << std::endl;

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