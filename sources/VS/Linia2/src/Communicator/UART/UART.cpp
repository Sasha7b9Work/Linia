#include "defines.h"
#include "Communicator/UART/UART.h"


//  Lin specific
#include <unistd.h>
#include <sys/file.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/ioctl.h>


#ifdef WIN32
    #pragma warning(push)
    #pragma warning(disable:4365 4389 4996)
    #define O_NOCTTY  0x00000400
    #define O_NDELAY  0
    #define LOCK_EX 2
    #define LOCK_NB 0
    #define LOCK_UN 0
#endif


namespace UART
{
    static int g_uart_fd = -1;
    static ReceivedCallback recv_callback = nullptr;
    static pthread_t g_reader_thread;
    static bool g_thread_running = false;
    static bool g_stop_reading = false;

    const size_t BUFFER_SIZE = 1024;

    static int GetBaudrateConstant(int baudrate);
    static bool ConfigurePort();
    static void *ReaderThreadFunc(void *arg);

    void Init(ReceivedCallback callback)
    {
        LOG_WRITE("Initializing UART...");

        g_uart_fd = -1;
        recv_callback = callback;
        g_thread_running = false;
        g_stop_reading = false;

        LOG_WRITE("UART initialized successfully");
    }

    void DeInit()
    {
        if (IsReady())
        {
            Close();
        }
        LOG_WRITE("UART deinitialized");
    }

    // Открытие UART порта с настройкой параметров и запуском потока чтения
    // Принимает: baudrate - скорость передачи (9600, 19200 и т.д.), mode - формат данных ("8N1" и т.д.)
    // Возвращает: true если порт успешно открыт, false при ошибке
    bool Open()
    {
        if (IsReady())
        {
            LOG_ERROR("UART already opened");
            return false;
        }

        g_uart_fd = ::open(UART_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
        if (g_uart_fd < 0)
        {
            LOG_ERROR("Cannot open UART device: %s", UART_DEVICE);
            return false;
        }

        if (flock(g_uart_fd, LOCK_EX | LOCK_NB) != 0)
        {
            LOG_ERROR("Cannot lock UART device");
            ::close(g_uart_fd);
            g_uart_fd = -1;
            return false;
        }

        if (!ConfigurePort())
        {
            LOG_ERROR("Cannot configure UART port");
            flock(g_uart_fd, LOCK_UN);
            ::close(g_uart_fd);
            g_uart_fd = -1;
            return false;
        }

        g_stop_reading = false;
        if (pthread_create(&g_reader_thread, nullptr, ReaderThreadFunc, nullptr) != 0)
        {
            LOG_ERROR("Cannot create reader thread");
            flock(g_uart_fd, LOCK_UN);
            ::close(g_uart_fd);
            g_uart_fd = -1;
            return false;
        }

        g_thread_running = true;
        LOG_WRITE("UART opened successfully on %s with baudrate %d and mode %s", UART_DEVICE, UART_BAUDRATE, UART_MODE);
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

        LOG_WRITE("UART closed");
    }

    // Отправка одного байта через UART
    // Принимает: байт для отправки (0x00-0xFF)
    // Возвращает: true если байт отправлен успешно, false при ошибке
    bool SendByte(uint8 byte)
    {
        if (!IsReady())
        {
            LOG_ERROR("UART not ready");
            return false;
        }

        int n = write(g_uart_fd, &byte, 1);
        if (n < 0)
        {
            LOG_ERROR("Failed to send byte");
            return false;
        }

        return (n == 1);
    }

    // Отправка буфера данных через UART порциями по 32 байта
    // Принимает: указатель на данные для отправки, количество байт для отправки (должно быть > 0)
    // Возвращает: true если все данные отправлены успешно, false при ошибке
    bool SendBuffer(const void *_buffer, int size)
    {
        if (!IsReady())
        {
            LOG_ERROR("UART not ready");
            return false;
        }

        const uint8 *buffer = (const uint8 *)_buffer;

        if (buffer == nullptr || size <= 0)
        {
            LOG_ERROR("Invalid buffer or size");
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

                LOG_ERROR("Failed to send buffer, errno: %d", errno);
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
            LOG_WRITE("UART sent %d bytes");
        }

        return (bytes_sent == size);
    }

    // Установка callback функции для обработки принятых данных
    // Принимает: функция вида void callback(uint8 byte)
    void SetCallback(ReceivedCallback callback)
    {
        recv_callback = callback;
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

    static bool ConfigurePort()
    {
        struct termios port_settings;
        memset(&port_settings, 0, sizeof(port_settings));

        int baudr = GetBaudrateConstant(UART_BAUDRATE);
        if (baudr == -1)
        {
            LOG_ERROR("Unsupported baudrate: %d", UART_BAUDRATE);
            return false;
        }

        int cbits = CS8, cpar = 0, ipar = IGNPAR, bstop = 0;

        if (strlen(UART_MODE) == 3)
        {
            switch (UART_MODE[0])
            {
            case '8': cbits = CS8; break; //-V1048
            case '7': cbits = CS7; break;
            case '6': cbits = CS6; break;
            case '5': cbits = CS5; break;
            default: cbits = CS8; break; //-V1048
            }

            switch (UART_MODE[1])
            {
            case 'N': case 'n': cpar = 0; ipar = IGNPAR; break; //-V1048 //-V525
            case 'E': case 'e': cpar = PARENB; ipar = INPCK; break;
            case 'O': case 'o': cpar = (PARENB | PARODD); ipar = INPCK; break;
            default: cpar = 0; ipar = IGNPAR; break; //-V1048
            }

            switch (UART_MODE[2])
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
            LOG_ERROR("Cannot set port settings");
            return false;
        }

        int status;
        if (ioctl(g_uart_fd, TIOCMGET, &status) == -1)
        {
            LOG_ERROR("Cannot get modem status");
            return false;
        }

        status |= TIOCM_DTR | TIOCM_RTS;

        if (ioctl(g_uart_fd, TIOCMSET, &status) == -1)
        {
            LOG_ERROR("Cannot set modem status");
            return false;
        }

        return true;
    }

    // Функция потока чтения UART
    // Читает данные из UART и вызывает callback для каждого принятого байта
    // Работает до установки флага g_stop_reading
    static void *ReaderThreadFunc(void *)
    {
        uint8 buffer[BUFFER_SIZE];
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
                    if (recv_callback)
                    {
                        for (int i = 0; i < bytes_read; i++)
                        {
                            recv_callback(buffer[i]);
                        }
                    }
                }
                else if (bytes_read < 0)
                {
                    if (errno != EAGAIN && errno != EWOULDBLOCK)
                    {
                        LOG_ERROR("Read failed");
                        break;
                    }
                }
            }
            else if (result < 0)
            {
                if (errno != EINTR)
                {
                    LOG_ERROR("Select failed");
                    break;
                }
            }

        }

        return nullptr;
    }
}


#ifdef WIN32
    #pragma warning(pop)
#endif
