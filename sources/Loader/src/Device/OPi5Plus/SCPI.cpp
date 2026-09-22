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

/*
    :PING

    // Просто показать, как работает обработка структур
    :CHIP:REG[0...9]:LENGTH [1...32]

    // Управление регистрами ПЛИС
    :FPGA[0...9]:LENGHT [1...32]
    :FPGA[0...9]:WRITE  [0....(uint)-1]

    // Управление ЦАПами блоков
    :DAC[0...9]:LENGTH [8...32]
    :DAC[0...9]:WRITE [0...(uint)-1]

    // Управление регистрами блоков
    :REG[0...9]:LENGTH [8...32]
    :REG[0...9]:WRITE [0...(uint)-1]

    // Управление источником напряжения 50 В
    :SOURCE50V:PLUS  [0...1]        XP10:50E+ Вкл/Откл
    :SOURCE50V:MINUS [0...1]        XP10:50E- Вкл/Откл
*/


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

        static bool Func_Ping(pchar);

        static bool Func_CHIP_REG(pchar);

        static StructParser chip[] =
        {
            { "REG",   Func_CHIP_REG, nullptr },    // :CHIP:REG...
            { nullptr, nullptr,       nullptr }
        };

        static bool Func_FPGA(pchar);
        static bool Func_DAC(pchar);
        static bool Func_REG(pchar);
        static bool Func_SCAN(pchar);
        static bool Func_Source50V(pchar);

        static StructParser head[] =
        {
            { "PING",      Func_Ping,      nullptr },
            { "CHIP",      nullptr,        chip    },
            { "FPGA",      Func_FPGA,      nullptr },
            { "DAC",       Func_DAC,       nullptr },
            { "REG",       Func_REG,       nullptr },
            { "SCAN",      Func_SCAN,      nullptr },
            { "SOURCE50V", Func_Source50V, nullptr },
            { nullptr,     nullptr,        nullptr }
        };

        static bool ProcessStructures(pchar, StructParser *);
    }
}


void OPi5Plus::SCPI::Parse(pchar command)
{
    bool result = ProcessStructures(command, head);

    (void)result;
}


bool OPi5Plus::SCPI::Func_Ping(pchar command)
{
    if (*command != '\0')
    {
        return false;
    }

    SCPI::Send(":PING");

    return true;
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

    return true;
}


bool OPi5Plus::SCPI::Func_FPGA(pchar)
{
    return false;
}


bool OPi5Plus::SCPI::Func_SCAN(pchar)
{
    return false;
}


bool OPi5Plus::SCPI::Func_Source50V(pchar)
{
    return false;
}


bool OPi5Plus::SCPI::Func_DAC(pchar)
{
    return false;
}


bool OPi5Plus::SCPI::Func_REG(pchar)
{
    return false;
}


bool OPi5Plus::SCPI::Func_CHIP_REG(pchar)
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
