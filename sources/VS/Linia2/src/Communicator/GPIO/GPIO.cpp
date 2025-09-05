#include "defines.h"
#include "Communicator/GPIO/GPIO.h"
#include <gpiod.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <cstring>


PinIn pinSTART(Pin::START);
PinIn pinSTOP(Pin::STOP);
PinIn pinDAT_F0(Pin::DAT_F0);
PinIn pinDAT_F1(Pin::DAT_F1);
PinIn pinDAT_F2(Pin::DAT_F2);
PinIn pinDAT_F3(Pin::DAT_F3);
PinIn pinFIFO_FULL(Pin::FIFO_FULL);

PinOut pinREQ_RD(Pin::REQ_RD);


namespace GPIO
{
    // Отдельные массивы для разных типов пинов
    static InputPinInfo g_input_pins[] = {
        // START - pin 15 (GPIO1_B0) 
        { { 8, "gpiochip1", nullptr, nullptr }, false, nullptr },
        // STOP - pin 21 (GPIO1_B1)
        { { 9, "gpiochip1", nullptr, nullptr }, false, nullptr },
        // DAT_F0 - pin 16 (GPIO3_B5)
        { { 13, "gpiochip3", nullptr, nullptr }, false, nullptr },
        // DAT_F1 - pin 18 (GPIO3_B6) 
        { { 14, "gpiochip3", nullptr, nullptr }, false, nullptr },
        // DAT_F2 - pin 22 (GPIO1_A2)
        { { 2, "gpiochip1", nullptr, nullptr }, false, nullptr },
        // DAT_F3 - pin 24 (GPIO1_B4)
        { { 12, "gpiochip1", nullptr, nullptr }, false, nullptr },
        // FIFO_FULL - pin 36 (GPIO3_A5)
        { { 5, "gpiochip3", nullptr, nullptr }, false, nullptr },
    };

    static OutputPinInfo g_output_pins[] = {
        // REQ_RD - pin 32 (GPIO1_A3)
        { { 3, "gpiochip1", nullptr, nullptr } },
    };

    // Маппинг enum Pin::Type на индексы в массивах
    static const struct {
        bool is_input;
        int index;
    } g_pin_mapping[] = {
        { true,  0 },  // START -> g_input_pins[0]
        { true,  1 },  // STOP -> g_input_pins[1]
        { true,  2 },  // DAT_F0 -> g_input_pins[2]
        { true,  3 },  // DAT_F1 -> g_input_pins[3]
        { true,  4 },  // DAT_F2 -> g_input_pins[4]
        { true,  5 },  // DAT_F3 -> g_input_pins[5]
        { true,  6 },  // FIFO_FULL -> g_input_pins[6]
        { false, 0 },  // REQ_RD -> g_output_pins[0]
    };

    static pthread_t g_monitor_thread;
    static bool g_thread_running = false;
    static bool g_stop_monitoring = false;

    static const int INPUT_PINS_COUNT = sizeof(g_input_pins) / sizeof(g_input_pins[0]);
    static const int OUTPUT_PINS_COUNT = sizeof(g_output_pins) / sizeof(g_output_pins[0]);

    // Вспомогательные функции для получения информации о пинах
    InputPinInfo *GetInputPinInfo(Pin::Type type)
    {
        if (type >= Pin::Count) return nullptr;

        auto &mapping = g_pin_mapping[type];
        if (mapping.is_input)
        {
            return &g_input_pins[mapping.index];
        }
        return nullptr;
    }

    OutputPinInfo *GetOutputPinInfo(Pin::Type type)
    {
        if (type >= Pin::Count) return nullptr;

        auto &mapping = g_pin_mapping[type];
        if (!mapping.is_input)
        {
            return &g_output_pins[mapping.index];
        }
        return nullptr;
    }

    static void *MonitorThreadFunc(void *arg);
}


namespace GPIO
{
    void Init()
    {
        std::cout << "Initializing GPIO..." << std::endl;

        for (int i = 0; i < INPUT_PINS_COUNT; i++)
        {
            InputPinInfo &info = g_input_pins[i];

            info.hw.chip = gpiod_chip_open_by_name(info.hw.chip_name);
            if (!info.hw.chip)
            {
                std::cerr << "Error: Cannot open GPIO chip " << info.hw.chip_name << std::endl;
                continue;
            }

            info.hw.line = gpiod_chip_get_line(info.hw.chip, (uint)info.hw.pin_number);
            if (!info.hw.line)
            {
                std::cerr << "Error: Cannot get GPIO line " << info.hw.pin_number << std::endl;
                gpiod_chip_close(info.hw.chip);
                info.hw.chip = nullptr;
                continue;
            }

            int ret = gpiod_line_request_input_flags(info.hw.line, nullptr,
                GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
            if (ret < 0)
            {
                std::cerr << "Error: Cannot request GPIO line " << info.hw.pin_number
                    << " as input" << std::endl;
                gpiod_chip_close(info.hw.chip);
                info.hw.chip = nullptr;
                info.hw.line = nullptr;
                continue;
            }

            info.last_state = (gpiod_line_get_value(info.hw.line) == 1);

            std::cout << "GPIO input pin " << info.hw.pin_number << " initialized" << std::endl;
        }

        for (int i = 0; i < OUTPUT_PINS_COUNT; i++)
        {
            OutputPinInfo &info = g_output_pins[i];

            info.hw.chip = gpiod_chip_open_by_name(info.hw.chip_name);
            if (!info.hw.chip)
            {
                std::cerr << "Error: Cannot open GPIO chip " << info.hw.chip_name << std::endl;
                continue;
            }

            info.hw.line = gpiod_chip_get_line(info.hw.chip, (uint)info.hw.pin_number);
            if (!info.hw.line)
            {
                std::cerr << "Error: Cannot get GPIO line " << info.hw.pin_number << std::endl;
                gpiod_chip_close(info.hw.chip);
                info.hw.chip = nullptr;
                continue;
            }

            int ret = gpiod_line_request_output(info.hw.line, nullptr, 0);
            if (ret < 0)
            {
                std::cerr << "Error: Cannot request GPIO line " << info.hw.pin_number
                    << " as output" << std::endl;
                gpiod_chip_close(info.hw.chip);
                info.hw.chip = nullptr;
                info.hw.line = nullptr;
                continue;
            }

            std::cout << "GPIO output pin " << info.hw.pin_number << " initialized" << std::endl;
        }

        g_stop_monitoring = false;
        if (pthread_create(&g_monitor_thread, nullptr, MonitorThreadFunc, nullptr) == 0)
        {
            g_thread_running = true;
            std::cout << "GPIO monitor thread started" << std::endl;
        }
        else
        {
            std::cerr << "Error: Cannot create GPIO monitor thread" << std::endl;
        }
    }

    void DeInit()
    {
        std::cout << "Deinitializing GPIO..." << std::endl;

        if (g_thread_running)
        {
            g_stop_monitoring = true;
            pthread_join(g_monitor_thread, nullptr);
            g_thread_running = false;
        }

        for (int i = 0; i < INPUT_PINS_COUNT; i++)
        {
            InputPinInfo &info = g_input_pins[i];

            if (info.hw.line)
            {
                gpiod_line_release(info.hw.line);
                info.hw.line = nullptr;
            }

            if (info.hw.chip)
            {
                gpiod_chip_close(info.hw.chip);
                info.hw.chip = nullptr;
            }
        }

        for (int i = 0; i < OUTPUT_PINS_COUNT; i++)
        {
            OutputPinInfo &info = g_output_pins[i];

            if (info.hw.line)
            {
                gpiod_line_release(info.hw.line);
                info.hw.line = nullptr;
            }

            if (info.hw.chip)
            {
                gpiod_chip_close(info.hw.chip);
                info.hw.chip = nullptr;
            }
        }

        std::cout << "GPIO deinitialized" << std::endl;
    }

    static void *MonitorThreadFunc(void *arg)
    {
        (void)arg;

        std::cout << "GPIO event-driven monitor thread started" << std::endl;

        fd_set read_fds;
        int max_fd = 0;
        int input_pins[INPUT_PINS_COUNT];
        int input_count = 0;

        for (int i = 0; i < INPUT_PINS_COUNT; i++)
        {
            InputPinInfo &info = g_input_pins[i];

            if (!info.hw.line)
                continue;

            gpiod_line_release(info.hw.line);

            int ret = gpiod_line_request_both_edges_events_flags(
                info.hw.line,
                nullptr,
                GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP
            );

            if (ret < 0)
            {
                std::cerr << "Error: Cannot request events for GPIO pin "
                    << info.hw.pin_number << std::endl;
                continue;
            }

            int fd = gpiod_line_event_get_fd(info.hw.line);
            if (fd < 0)
            {
                std::cerr << "Error: Cannot get event fd for GPIO pin "
                    << info.hw.pin_number << std::endl;
                continue;
            }

            input_pins[input_count] = i;
            input_count++;

            if (fd > max_fd)
                max_fd = fd;

            info.last_state = (gpiod_line_get_value(info.hw.line) == 1);

            std::cout << "GPIO pin " << info.hw.pin_number
                << " configured for event monitoring" << std::endl;
        }

        if (input_count == 0)
        {
            std::cerr << "Warning: No input pins configured for event monitoring" << std::endl;
            return nullptr;
        }

        while (!g_stop_monitoring)
        {
            FD_ZERO(&read_fds);

            for (int i = 0; i < input_count; i++)
            {
                int pin_idx = input_pins[i];
                InputPinInfo &info = g_input_pins[pin_idx];

                if (info.hw.line)
                {
                    int fd = gpiod_line_event_get_fd(info.hw.line);
                    if (fd >= 0)
                    {
#ifdef WIN32
    #pragma warning(push, 0)
#endif
                        FD_SET(fd, &read_fds);
#ifdef WIN32
    #pragma warning(pop)
#endif
                    }
                }
            }

            struct timeval timeout;
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            int result = select(max_fd + 1, &read_fds, nullptr, nullptr, &timeout);

            if (result < 0)
            {
                if (errno == EINTR)
                    continue;

                std::cerr << "Error: select() failed in GPIO monitor: "
                    << strerror(errno) << std::endl;
                break;
            }
            else if (result == 0)
            {
                continue;
            }

            for (int i = 0; i < input_count; i++)
            {
                int pin_idx = input_pins[i];
                InputPinInfo &info = g_input_pins[pin_idx];

                if (!info.hw.line || !info.callback)
                    continue;

                int fd = gpiod_line_event_get_fd(info.hw.line);
                if (fd < 0 || !FD_ISSET(fd, &read_fds))
                    continue;

                struct gpiod_line_event event;
                int ret = gpiod_line_event_read(info.hw.line, &event);

                if (ret < 0)
                {
                    std::cerr << "Error: Cannot read GPIO event for pin "
                        << info.hw.pin_number << std::endl;
                    continue;
                }

                bool new_state;
                if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE)
                {
                    new_state = true;
                }
                else if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE)
                {
                    new_state = false;
                }
                else
                {
                    continue;
                }

                if (new_state != info.last_state)
                {
                    info.last_state = new_state;

                    info.callback(new_state);

                    std::cout << "GPIO pin " << info.hw.pin_number
                        << " event: " << (new_state ? "RISING" : "FALLING")
                        << " -> " << (new_state ? "HIGH" : "LOW") << std::endl;
                }
            }
        }

        std::cout << "GPIO event-driven monitor thread stopped" << std::endl;
        return nullptr;
    }
}

bool Pin::Get() const
{
    if (type_ >= Pin::Count) return false;

    // Сначала пробуем как input pin
    InputPinInfo *input_info = GPIO::GetInputPinInfo(type_);
    if (input_info && input_info->hw.line)
    {
        int val = gpiod_line_get_value(input_info->hw.line);
        if (val < 0)
        {
            std::cerr << "Error: Cannot read GPIO pin " << input_info->hw.pin_number << std::endl;
            return false;
        }
        return (val == 1);
    }

    // Если не input, то пробуем как output pin
    OutputPinInfo *output_info = GPIO::GetOutputPinInfo(type_);
    if (output_info && output_info->hw.line)
    {
        int val = gpiod_line_get_value(output_info->hw.line);
        if (val < 0)
        {
            std::cerr << "Error: Cannot read GPIO pin " << output_info->hw.pin_number << std::endl;
            return false;
        }
        return (val == 1);
    }

    return false;
}

void PinOut::Set(bool state)
{
    if (type_ >= Pin::Count) return;

    OutputPinInfo *info = GPIO::GetOutputPinInfo(type_);
    if (!info || !info->hw.line) return;

    int ret = gpiod_line_set_value(info->hw.line, state ? 1 : 0);
    if (ret < 0)
    {
        std::cerr << "Error: Cannot set GPIO pin " << info->hw.pin_number
            << " to " << (state ? "HIGH" : "LOW") << std::endl;
    }
}

void PinIn::SetChangeCallback(ChangeCallback callback)
{
    callback_ = callback;

    if (type_ < Pin::Count)
    {
        InputPinInfo *info = GPIO::GetInputPinInfo(type_);
        if (info)
        {
            info->callback = callback;
        }
    }
}
