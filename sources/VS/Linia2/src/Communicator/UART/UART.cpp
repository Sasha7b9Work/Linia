#include "Communicator/UART/UART.h"

#ifdef WIN32
namespace UART
{
    void Init()
    {
    }

    void DeInit()
    {
    }

    bool Open(int /*baudrate*/, const char * /*mode*/)
    {
        return false;
    }

    void Close()
    {
    }

    bool SendByte(uint8_t /*byte*/)
    {
        return false;
    }

    bool SendBuffer(const uint8_t * /*buffer*/, int /*size*/)
    {
        return false;
    }

    void SetCallback(ReceivedCallback /*callback*/)
    {
    }

    void Flush()
    {
    }

    bool IsReady()
    {
        return false;
    }

    int GetBaudrate()
    {
        return 0;
    }

    const char *GetMode()
    {
        return "";
    }
}

#else

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <sys/ioctl.h>
#include <algorithm>
#include <sys/select.h>

namespace UART
{

    /*
    *   Вопросы Николаю.
    *  2. Почему в функции ReaderThreadFunc() локальные переменные объявлены в начале функции ?
    *  5. Почему ReaderThreadFunc() возвращает указатель?
    */

    static int g_uart_fd = -1;
    static int g_baudrate = 115200;
    static char g_mode[4] = "8N1";
    static ReceivedCallback g_callback = nullptr;
    static pthread_t g_reader_thread;
    static bool g_thread_running = false;
    static bool g_stop_reading = false;

    const char *UART_DEVICE = "/dev/ttyS6";       // Путь к UART устройству
    const size_t BUFFER_SIZE = 1024;

    static int GetBaudrateConstant(int baudrate);
    static bool ConfigurePort(int baudrate, const char *mode);
    static void *ReaderThreadFunc(void *arg);

    void Init()
    {
        g_uart_fd = -1;
        g_baudrate = 115200;
        strcpy(g_mode, "8N1");
        g_callback = nullptr;
        g_thread_running = false;
        g_stop_reading = false;

        std::cout << "UART initialized" << std::endl;
    }

    void DeInit()
    {
        if (IsReady())
        {
            Close();
        }
        std::cout << "UART deinitialized" << std::endl;
    }

    // Открытие UART порта с настройкой параметров и запуском потока чтения
    // Принимает: baudrate - скорость передачи (9600, 19200 и т.д.), mode - формат данных ("8N1" и т.д.)
    // Возвращает: true если порт успешно открыт, false при ошибке
    bool Open(int baudrate, const char *mode)
    {
        if (IsReady())
        {
            std::cerr << "Error: UART already opened" << std::endl;
            return false;
        }

        g_uart_fd = ::open(UART_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
        if (g_uart_fd < 0)
        {
            std::cerr << "Error: Cannot open UART device: " << UART_DEVICE << std::endl;
            return false;
        }

        if (flock(g_uart_fd, LOCK_EX | LOCK_NB) != 0)
        {
            std::cerr << "Error: Cannot lock UART device" << std::endl;
            ::close(g_uart_fd);
            g_uart_fd = -1;
            return false;
        }

        if (!ConfigurePort(baudrate, mode))
        {
            std::cerr << "Error: Cannot configure UART port" << std::endl;
            flock(g_uart_fd, LOCK_UN);
            ::close(g_uart_fd);
            g_uart_fd = -1;
            return false;
        }

        g_baudrate = baudrate;
        strncpy(g_mode, mode, sizeof(g_mode) - 1);
        g_mode[sizeof(g_mode) - 1] = '\0';

        g_stop_reading = false;
        if (pthread_create(&g_reader_thread, nullptr, ReaderThreadFunc, nullptr) != 0)
        {
            std::cerr << "Error: Cannot create reader thread" << std::endl;
            flock(g_uart_fd, LOCK_UN);
            ::close(g_uart_fd);
            g_uart_fd = -1;
            return false;
        }

        g_thread_running = true;
        std::cout << "UART opened successfully on " << UART_DEVICE
            << " with baudrate " << baudrate << " and mode " << mode << std::endl;
        return true;
    }

    void Close()
    {
        if (!IsReady()) return;

        g_stop_reading = true;
        if (g_thread_running)
        {
            pthread_join(g_reader_thread, nullptr);
            g_thread_running = false;
        }

        int status;
        if (ioctl(g_uart_fd, TIOCMGET, &status) != -1)
        {
            status &= ~(TIOCM_DTR | TIOCM_RTS);
            ioctl(g_uart_fd, TIOCMSET, &status);
        }

        flock(g_uart_fd, LOCK_UN);
        ::close(g_uart_fd);
        g_uart_fd = -1;

        std::cout << "UART closed" << std::endl;
    }

    // Отправка одного байта через UART
    // Принимает: байт для отправки (0x00-0xFF)
    // Возвращает: true если байт отправлен успешно, false при ошибке
    bool SendByte(uint8_t byte)
    {
        if (!IsReady())
        {
            std::cerr << "Error: UART not ready" << std::endl;
            return false;
        }

        int n = write(g_uart_fd, &byte, 1);
        if (n < 0)
        {
            std::cerr << "Error: Failed to send byte" << std::endl;
            return false;
        }

        return (n == 1);
    }

    // Отправка буфера данных через UART порциями по 32 байта
    // Принимает: указатель на данные для отправки, количество байт для отправки (должно быть > 0)
    // Возвращает: true если все данные отправлены успешно, false при ошибке
    bool SendBuffer(const uint8_t *buffer, int size)
    {
        if (!IsReady())
        {
            std::cerr << "Error: UART not ready" << std::endl;
            return false;
        }

        if (buffer == nullptr || size <= 0)
        {
            std::cerr << "Error: Invalid buffer or size" << std::endl;
            return false;
        }

        int bytes_sent = 0;
        while (bytes_sent < size)
        {
            int chunk_size = std::min(32, size - bytes_sent);
            int n = write(g_uart_fd, buffer + bytes_sent, chunk_size);

            if (n < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    usleep(1000);
                    continue;
                }
                std::cerr << "Error: Failed to send buffer, errno: " << errno << std::endl;
                return false;
            }

            bytes_sent += n;

            if (bytes_sent < size)
            {
                usleep(100);
            }
        }

        if (bytes_sent == size)
        {
            std::cout << "UART sent " << size << " bytes" << std::endl;
        }

        return (bytes_sent == size);
    }

    // Установка callback функции для обработки принятых данных
    // Принимает: функция вида void callback(uint8_t byte)
    void SetCallback(ReceivedCallback callback)
    {
        g_callback = callback;
    }

    void Flush()
    {
        if (IsReady())
        {
            tcflush(g_uart_fd, TCIOFLUSH);
        }
    }

    bool IsReady()
    {
        return g_uart_fd >= 0;
    }

    int GetBaudrate()
    {
        return g_baudrate;
    }

    const char *GetMode()
    {
        return g_mode;
    }

    // Внутренняя функция: преобразование скорости в системную константу
    // Принимает: скорость (9600, 19200 и т.д.)
    // Возвращает: системная константа (B115200, B9600, и т.д.)
    static int GetBaudrateConstant(int baudrate)
    {
        switch (baudrate)
        {
        case 9600:   return B9600;
        case 19200:  return B19200;
        case 38400:  return B38400;
        case 57600:  return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
        case 460800: return B460800;
        case 921600: return B921600;
        default:     return -1;
        }
    }

    // Внутренняя функция: настройка параметров UART порта
    // Принимает: baudrate - скорость передачи (9600, 19200 и т.д.), mode - режим передачи ("8N1" и т.д.)
    // Возвращает: true если настройка успешна, false при ошибке
    static bool ConfigurePort(int baudrate, const char *mode)
    {
        struct termios port_settings;
        memset(&port_settings, 0, sizeof(port_settings));

        int baudr = GetBaudrateConstant(baudrate);
        if (baudr == -1)
        {
            std::cerr << "Error: Unsupported baudrate: " << baudrate << std::endl;
            return false;
        }

        int cbits = CS8, cpar = 0, ipar = IGNPAR, bstop = 0;

        if (strlen(mode) == 3)
        {
            switch (mode[0])
            {
            case '8': cbits = CS8; break;
            case '7': cbits = CS7; break;
            case '6': cbits = CS6; break;
            case '5': cbits = CS5; break;
            default: cbits = CS8; break;
            }

            switch (mode[1])
            {
            case 'N': case 'n': cpar = 0; ipar = IGNPAR; break;
            case 'E': case 'e': cpar = PARENB; ipar = INPCK; break;
            case 'O': case 'o': cpar = (PARENB | PARODD); ipar = INPCK; break;
            default: cpar = 0; ipar = IGNPAR; break;
            }

            switch (mode[2])
            {
            case '1': bstop = 0; break;
            case '2': bstop = CSTOPB; break;
            default: bstop = 0; break;
            }
        }

        port_settings.c_cflag = cbits | cpar | bstop | CLOCAL | CREAD;
        port_settings.c_iflag = ipar;
        port_settings.c_oflag = 0;
        port_settings.c_lflag = 0;
        port_settings.c_cc[VMIN] = 0;
        port_settings.c_cc[VTIME] = 0;

        cfsetispeed(&port_settings, baudr);
        cfsetospeed(&port_settings, baudr);

        if (tcsetattr(g_uart_fd, TCSANOW, &port_settings) == -1)
        {
            std::cerr << "Error: Cannot set port settings" << std::endl;
            return false;
        }

        int status;
        if (ioctl(g_uart_fd, TIOCMGET, &status) == -1)
        {
            std::cerr << "Error: Cannot get modem status" << std::endl;
            return false;
        }

        status |= TIOCM_DTR | TIOCM_RTS;

        if (ioctl(g_uart_fd, TIOCMSET, &status) == -1)
        {
            std::cerr << "Error: Cannot set modem status" << std::endl;
            return false;
        }

        return true;
    }

    // Функция потока чтения UART
    // Читает данные из UART и вызывает callback для каждого принятого байта
    // Работает до установки флага g_stop_reading
    static void *ReaderThreadFunc(void *)
    {
        uint8_t buffer[BUFFER_SIZE];
        int bytes_read;
        fd_set read_fds;
        struct timeval timeout;

        while (!g_stop_reading)
        {
            // Настраиваем select для ожидания данных
            FD_ZERO(&read_fds);
            FD_SET(g_uart_fd, &read_fds);

            timeout.tv_sec = 0;
            timeout.tv_usec = 100000;

            // select блокируется до появления данных или таймаута
            int result = select(g_uart_fd + 1, &read_fds, nullptr, nullptr, &timeout);

            if (result > 0 && FD_ISSET(g_uart_fd, &read_fds))
            {
                bytes_read = read(g_uart_fd, buffer, sizeof(buffer));

                if (bytes_read > 0)
                {
                    if (g_callback)
                    {
                        for (int i = 0; i < bytes_read; i++)
                        {
                            g_callback(buffer[i]);
                        }
                    }
                }
                else if (bytes_read < 0)
                {
                    if (errno != EAGAIN && errno != EWOULDBLOCK)
                    {
                        std::cerr << "Error: Read failed" << std::endl;
                        break;
                    }
                }
            }
            else if (result < 0)
            {
                if (errno != EINTR)
                {
                    std::cerr << "Error: select failed" << std::endl;
                    break;
                }
            }

        }

        return nullptr;
    }
}

#endif
