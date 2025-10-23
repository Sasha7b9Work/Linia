// 2025/10/23 12:29:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/SCPI/HeadSCPI.h"



namespace SCPI
{
    static bool FuncCommand1(pchar);

    StructSCPI head[] =
    {
        { "COMMAND1", FuncCommand1, nullptr },
        { nullptr, nullptr, nullptr }
    };
}



bool SCPI::StructSCPI::Update(pchar message, StructSCPI *handler)
{
    while (handler->begin != nullptr)
    {
        size_t len_msg = std::strlen(message);

        if (std::strlen(handler->begin) >= len_msg)
        {
            if (std::memcmp(handler->begin, message, len_msg) == 0)
            {
                if (handler->func)
                {
                    return handler->func(message + len_msg);
                }
                else if (handler->handler)
                {
                    return StructSCPI::Update(message + len_msg, handler->handler);
                }
            }
        }

        handler++;
    }

    return false;
}


bool SCPI::FuncCommand1(pchar)
{
    return false;
}
