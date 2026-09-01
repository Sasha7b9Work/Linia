// 2026/09/01 10:10:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "IPPP/Keyboard/Antichatter.h"


bool Antichatter::InOut(bool state, int64 timeMS)
{
    if (timeMS < prev_time_input)
    {
        LOG_ERROR("Invalid time in function %s", __FUNCTION__);

        LOG_WRITE("201");

        return prev_state;
    }

    LOG_WRITE("202");

    prev_time_input = timeMS;

    bool result = prev_state;

    {
        LOG_WRITE("203");

        if (prev_time_change == 0)  // Находимся в состоянии ожидания, дребезг не обрабатываем
        {
            LOG_WRITE("204");
            if (state != prev_state)
            {
                LOG_WRITE("205");
                result = state;

                prev_time_change = timeMS;
                prev_state = state;

                LOG_WRITE("206");
                funcOnChnage(state);
                LOG_WRITE("207");
            }
        }
        else                        // Было срабатывание, ждём дребезг
        {
            if (timeMS - prev_time_change >= timeAntichatterMS)
            {
                LOG_WRITE("208");
                result = state;

                if (prev_state != state)
                {
                    prev_time_change = timeMS;

                    LOG_WRITE("208");

                    funcOnChnage(state);

                    LOG_WRITE("209");
                }
                else
                {
                    prev_time_change = 0;
                }

                prev_state = state;
            }
        }
    }

    LOG_WRITE("210");

    return result;
}
