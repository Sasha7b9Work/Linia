// 2025/10/15 14:32:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Device/OPi5Plus/SCPI.h"
#include "Utils/StringUtils.h"
#include "Hardware/HAL/HAL.h"
#include "Device/Device.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdio>


namespace OPi5Plus
{
    namespace SCPI
    {
        struct StructParser
        {
            pchar         begin_string;
            bool          (*func)(pchar);
            StructParser *structs;
        };

        static bool Func_Upgrade(pchar);

        static StructParser head[] =
        {
            { "UPGRADE",   Func_Upgrade,   nullptr },
            { nullptr,     nullptr,        nullptr }
        };

        static bool ProcessStructures(pchar, StructParser *);
    }
}


void OPi5Plus::SCPI::Parse(pchar command)
{
    if (!ProcessStructures(command, head))
    {
        LOG_ERROR("Bad command : %s", command);
    }
}


bool OPi5Plus::SCPI::ProcessStructures(pchar command, StructParser *handlers)
{
    while (command[0] == ':')
    {
        command++;
    }

    StructParser *handler = handlers;

    while (handler->begin_string)
    {
        if (SU::BeginWith(command, handler->begin_string))
        {
            if (handler->func)
            {
                pchar new_command = command + std::strlen(handler->begin_string);

                if (*new_command == ':')
                {
                    new_command++;
                }

                return handler->func(new_command);
            }
            else if (handler->structs)
            {
                return ProcessStructures(command + std::strlen(handler->begin_string), handler->structs);
            }
        }

        handler++;
    }

    return false;
}


bool OPi5Plus::SCPI::Func_Upgrade(pchar command)
{


    return false;
}


void OPi5Plus::SCPI::Send(pchar format, ...)
{
    char message[1024];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    HAL_USART1::TransmitString(message);
}
