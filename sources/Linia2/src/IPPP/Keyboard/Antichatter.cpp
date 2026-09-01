// 2026/09/01 10:10:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "IPPP/Keyboard/Antichatter.h"


bool Antichatter::InOut(bool state, int64 timeMS)
{
    if (timeMS < prev_time_input)
    {
        LOG_ERROR("Invalid time in function %s", __FUNCTION__);

        return prev_state;
    }

    prev_time_input = timeMS;

    bool result = prev_state;

    {
        if (prev_time_change == 0)  // Находимся в состоянии ожидания, дребезг не обрабатываем
        {
            if (state != prev_state)
            {
                result = state;

                prev_time_change = timeMS;
                prev_state = state;
            }
        }
        else                        // Было срабатывание, ждём дребезг
        {
            if (timeMS - prev_time_change >= timeAntichatterMS)
            {
                result = state;

                if (prev_state != state)
                {
                    prev_time_change = timeMS;
                }
                else
                {
                    prev_time_change = 0;
                }

                prev_state = state;
            }
        }
    }

    return result;
}
