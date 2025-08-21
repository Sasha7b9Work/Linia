#include "SPI_defines.h"
#include "Communicator/SPI/SPI.h"

namespace
{
    // Внутренние переменные модуля
    int g_spi_fd = -1;
    uint32_t g_speed = 1000000;
    uint8_t g_mode = 0; // SPI_MODE_0
    uint8_t g_bits_per_word = 8;
    
    // GPIO пины согласно ALGORITM.MD (физические номера пинов)
    const unsigned int EN_DDA1_PIN = 31;  // GPIO6
    const unsigned int EN_DDA2_PIN = 35;  // GPIO19
    
    // libgpiod объекты
#ifdef HAVE_LIBGPIOD
    struct gpiod_chip* g_gpio_chip = nullptr;
    struct gpiod_line* g_dda1_line = nullptr;
    struct gpiod_line* g_dda2_line = nullptr;
#endif
    bool g_gpio_initialized = false;
    
    // Преобразование физического номера пина в номер GPIO
    unsigned int PhysicalToGPIO(unsigned int physical_pin)
    {
        // Маппинг физических пинов на GPIO для Raspberry Pi
        switch (physical_pin)
        {
            case 31: return 6;   // Physical pin 31 -> GPIO6
            case 35: return 19;  // Physical pin 35 -> GPIO19
            default: return physical_pin; // Fallback
        }
    }
    
    bool InitGPIO()
    {
        if (g_gpio_initialized)
            return true;
            
        // Проверяем доступность GPIO библиотеки
#ifndef HAVE_LIBGPIOD
        std::cout << "Warning: GPIO library (libgpiod) not available - GPIO control disabled" << std::endl;
        g_gpio_initialized = true;  // Отмечаем как инициализированный, но без GPIO
        return true;
#else
        // Открываем GPIO chip (обычно gpiochip0 для Raspberry Pi)
        g_gpio_chip = gpiod_chip_open_by_name("gpiochip0");
        if (!g_gpio_chip)
        {
            std::cerr << "Error: Cannot open GPIO chip" << std::endl;
            return false;
        }
        
        // Получаем GPIO линии
        unsigned int dda1_gpio = PhysicalToGPIO(EN_DDA1_PIN);
        unsigned int dda2_gpio = PhysicalToGPIO(EN_DDA2_PIN);
        
        g_dda1_line = gpiod_chip_get_line(g_gpio_chip, dda1_gpio);
        g_dda2_line = gpiod_chip_get_line(g_gpio_chip, dda2_gpio);
        
        if (!g_dda1_line || !g_dda2_line)
        {
            std::cerr << "Error: Cannot get GPIO lines" << std::endl;
            if (g_gpio_chip)
            {
                gpiod_chip_close(g_gpio_chip);
                g_gpio_chip = nullptr;
            }
            return false;
        }
        
        // Настраиваем линии как выходы с начальным значением LOW
        int ret1 = gpiod_line_request_output(g_dda1_line, "SPI_EN_DDA1", 0);
        int ret2 = gpiod_line_request_output(g_dda2_line, "SPI_EN_DDA2", 0);
        
        if (ret1 < 0 || ret2 < 0)
        {
            std::cerr << "Error: Cannot request GPIO lines as outputs" << std::endl;
            if (g_gpio_chip)
            {
                gpiod_chip_close(g_gpio_chip);
                g_gpio_chip = nullptr;
            }
            return false;
        }
        
        g_gpio_initialized = true;
        std::cout << "GPIO initialized successfully using libgpiod" << std::endl;
        return true;
#endif
    }
    
    void DeInitGPIO()
    {
#ifdef HAVE_LIBGPIOD
        if (g_dda1_line)
        {
            gpiod_line_release(g_dda1_line);
            g_dda1_line = nullptr;
        }
        if (g_dda2_line)
        {
            gpiod_line_release(g_dda2_line);
            g_dda2_line = nullptr;
        }
        if (g_gpio_chip)
        {
            gpiod_chip_close(g_gpio_chip);
            g_gpio_chip = nullptr;
        }
#endif
        g_gpio_initialized = false;
    }
    
    void SetCS(int dac_number, bool enable)
    {
        if (!g_gpio_initialized)
            return;
            
#ifdef HAVE_LIBGPIOD
        int value = enable ? 1 : 0;
        
        switch (dac_number)
        {
            case 1:
                if (g_dda1_line)
                {
                    gpiod_line_set_value(g_dda1_line, value);
                }
                break;
            case 2:
                if (g_dda2_line)
                {
                    gpiod_line_set_value(g_dda2_line, value);
                }
                break;
            default:
                std::cerr << "Error: Invalid DAC number: " << dac_number << std::endl;
                break;
        }
#else
        // Заглушка - ничего не делаем без libgpiod
        (void)dac_number;
        (void)enable;
#endif
    }
    
    bool Write(uint8_t* data, size_t length)
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
}

void SPI::Init()
{
    const char* device = "/dev/spidev1.0";
    
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
        // Отключаем все CS
        SetCS(1, false);
        SetCS(2, false);
        
        ::close(g_spi_fd);
        g_spi_fd = -1;
        std::cout << "SPI deinitialized" << std::endl;
    }
    
    // Освобождаем GPIO ресурсы
    DeInitGPIO();
}

bool SPI::WriteDynamicDAC1(uint16_t value)
{
    if (!IsReady())
    {
        std::cerr << "Error: SPI not ready" << std::endl;
        return false;
    }
    
    // Подготовка данных (MSB первым)
    uint8_t data[2];
    data[0] = static_cast<uint8_t>((value >> 8) & 0xFF);
    data[1] = static_cast<uint8_t>(value & 0xFF);
    
    // Активируем CS для первого DAC
    SetCS(1, true);
    usleep(1);
    
    // Передаем данные
    bool result = Write(data, 2);
    
    if (result)
    {
        std::cout << "DAC1 written: 0x" << std::hex << value << std::dec << std::endl;
    }
    
    usleep(1);
    SetCS(1, false);
    
    return result;
}

bool SPI::WriteDynamicDAC2(uint16_t value)
{
    if (!IsReady())
    {
        std::cerr << "Error: SPI not ready" << std::endl;
        return false;
    }
    
    // Подготовка данных (MSB первым)
    uint8_t data[2];
    data[0] = static_cast<uint8_t>((value >> 8) & 0xFF);
    data[1] = static_cast<uint8_t>(value & 0xFF);
    
    // Активируем CS для второго DAC
    SetCS(2, true);
    usleep(1);
    
    // Передаем данные
    bool result = Write(data, 2);
    
    if (result)
    {
        std::cout << "DAC2 written: 0x" << std::hex << value << std::dec << std::endl;
    }
    
    usleep(1);
    SetCS(2, false);
    
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

void SPI::TestGPIO()
{
    std::cout << "\n=== Тестирование GPIO для SPI ===" << std::endl;
    
    if (!g_gpio_initialized)
    {
        std::cout << "❌ GPIO не инициализирован" << std::endl;
        return;
    }
    
#ifdef HAVE_LIBGPIOD
    std::cout << "🔧 Тестирование EN_DDA1 (GPIO6, физический пин 31)..." << std::endl;
    if (g_dda1_line)
    {
        // Включаем
        if (gpiod_line_set_value(g_dda1_line, 1) == 0)
        {
            std::cout << "✅ EN_DDA1 установлен в HIGH" << std::endl;
            usleep(500000); // 0.5 сек
            
            // Выключаем
            if (gpiod_line_set_value(g_dda1_line, 0) == 0)
            {
                std::cout << "✅ EN_DDA1 установлен в LOW" << std::endl;
            }
            else
            {
                std::cout << "❌ Ошибка установки EN_DDA1 в LOW" << std::endl;
            }
        }
        else
        {
            std::cout << "❌ Ошибка установки EN_DDA1 в HIGH" << std::endl;
        }
    }
    else
    {
        std::cout << "❌ GPIO линия EN_DDA1 недоступна" << std::endl;
    }
    
    std::cout << "🔧 Тестирование EN_DDA2 (GPIO19, физический пин 35)..." << std::endl;
    if (g_dda2_line)
    {
        // Включаем
        if (gpiod_line_set_value(g_dda2_line, 1) == 0)
        {
            std::cout << "✅ EN_DDA2 установлен в HIGH" << std::endl;
            usleep(500000); // 0.5 сек
            
            // Выключаем
            if (gpiod_line_set_value(g_dda2_line, 0) == 0)
            {
                std::cout << "✅ EN_DDA2 установлен в LOW" << std::endl;
            }
            else
            {
                std::cout << "❌ Ошибка установки EN_DDA2 в LOW" << std::endl;
            }
        }
        else
        {
            std::cout << "❌ Ошибка установки EN_DDA2 в HIGH" << std::endl;
        }
    }
    else
    {
        std::cout << "❌ GPIO линия EN_DDA2 недоступна" << std::endl;
    }
#else
    std::cout << "⚠️  libgpiod не скомпилирована - GPIO тестирование недоступно" << std::endl;
#endif
    
    std::cout << "=== Тестирование GPIO завершено ===" << std::endl;
}

void SPI::DiagnoseSPI()
{
    std::cout << "\n=== Диагностика SPI ===" << std::endl;
    
    // Проверяем состояние SPI
    std::cout << "SPI готов: " << (IsReady() ? "✅ ДА" : "❌ НЕТ") << std::endl;
    std::cout << "Текущая скорость: " << GetSpeed() << " Hz" << std::endl;
    std::cout << "Текущий режим: " << static_cast<int>(GetMode()) << std::endl;
    
    // Проверяем доступность SPI устройства
    if (CheckSPIDevice("/dev/spidev1.0"))
    {
        std::cout << "SPI устройство: ✅ /dev/spidev1.0 доступно" << std::endl;
    }
    else
    {
        std::cout << "SPI устройство: ❌ /dev/spidev1.0 недоступно" << std::endl;
        
        // Проверяем альтернативные устройства
        if (CheckSPIDevice("/dev/spidev0.0"))
        {
            std::cout << "Альтернатива: ✅ /dev/spidev0.0 доступно" << std::endl;
        }
        else if (CheckSPIDevice("/dev/spidev2.0"))
        {
            std::cout << "Альтернатива: ✅ /dev/spidev2.0 доступно" << std::endl;
        }
        else
        {
            std::cout << "❌ Нет доступных SPI устройств" << std::endl;
        }
    }
    
    // Тестируем GPIO
    TestGPIO();
    
    // Тестируем запись тестовых значений
    std::cout << "\n🔧 Тестирование записи в DAC..." << std::endl;
    if (IsReady())
    {
        std::cout << "Запись тестового значения 0x123 в DAC1..." << std::endl;
        bool result1 = WriteDynamicDAC1(0x123);
        std::cout << "Результат DAC1: " << (result1 ? "✅ Успешно" : "❌ Ошибка") << std::endl;
        
        std::cout << "Запись тестового значения 0x456 в DAC2..." << std::endl;
        bool result2 = WriteDynamicDAC2(0x456);
        std::cout << "Результат DAC2: " << (result2 ? "✅ Успешно" : "❌ Ошибка") << std::endl;
    }
    else
    {
        std::cout << "⚠️  SPI не готов - пропуск тестирования записи" << std::endl;
    }
    
    std::cout << "=== Диагностика SPI завершена ===" << std::endl;
}

bool SPI::CheckSPIDevice(const char* device_path)
{
    int fd = ::open(device_path, O_RDWR);
    if (fd >= 0)
    {
        ::close(fd);
        return true;
    }
    return false;
}

void SPI::PrintSystemInfo()
{
    std::cout << "\n=== Системная информация ===" << std::endl;
    
    // Информация о системе
    std::cout << "Операционная система: ";
    system("uname -a");
    
    // Информация о GPIO
    std::cout << "\nGPIO устройства:" << std::endl;
    system("ls -la /dev/gpiochip* 2>/dev/null || echo 'GPIO устройства не найдены'");
    
    // Информация о SPI
    std::cout << "\nSPI устройства:" << std::endl;
    system("ls -la /dev/spi* 2>/dev/null || echo 'SPI устройства не найдены'");
    
    // Проверка модулей ядра
    std::cout << "\nМодули SPI в ядре:" << std::endl;
    system("lsmod | grep spi || echo 'Модули SPI не загружены'");
    
    // Информация о libgpiod
    std::cout << "\nВерсия libgpiod:" << std::endl;
#ifdef HAVE_LIBGPIOD
    system("pkg-config --modversion libgpiod 2>/dev/null || echo 'libgpiod не найдена'");
#else
    std::cout << "libgpiod не скомпилирована" << std::endl;
#endif
    
    // GPIO информация через libgpiod утилиты
    std::cout << "\nДоступные GPIO чипы:" << std::endl;
    system("gpiodetect 2>/dev/null || echo 'Утилита gpiodetect не найдена'");
    
    std::cout << "=== Конец системной информации ===" << std::endl;
}
