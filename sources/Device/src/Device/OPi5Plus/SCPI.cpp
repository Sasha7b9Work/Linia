// 2025/10/15 14:32:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Device/OPi5Plus/SCPI.h"
#include "Utils/StringUtils.h"
#include "Device/FPGA.h"
#include "Device/Chips.h"
#include "Hardware/HAL/HAL.h"
#include "Device/Device.h"
#include "Device/Sources.h"
#include "Hardware/Bootloader.h"
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

        static bool Func_Ping(pchar);
        static bool Func_CHIP_REG(pchar);
        static bool Func_FPGA(pchar);
        static bool Func_DAC(pchar);
        static bool Func_REG(pchar);
        static bool Func_SCAN(pchar);
        static bool Func_Source50V(pchar);
        static bool Func_Upgrade(pchar);
        static bool Func_Info(pchar);

        static StructParser chip[] =
        {
            { "REG",   Func_CHIP_REG, nullptr },    // :CHIP:REG...
            { nullptr, nullptr,       nullptr }
        };

        static StructParser head[] =
        {
            { "PING",      Func_Ping,      nullptr },
            { "CHIP",      nullptr,        chip    },
            { "FPGA",      Func_FPGA,      nullptr },
            { "DAC",       Func_DAC,       nullptr },
            { "REG",       Func_REG,       nullptr },
            { "SCAN",      Func_SCAN,      nullptr },
            { "SOURCE50V", Func_Source50V, nullptr },
            { "UPGRADE",   Func_Upgrade,   nullptr },
            { "INFO",      Func_Info,      nullptr },
            { nullptr,     nullptr,        nullptr }
        };

        static bool ProcessStructures(pchar, StructParser *);
    }
}


void OPi5Plus::SCPI::Parse(pchar command)
{
    if (!ProcessStructures(command, head))
    {
        LOG_ERROR("Error comand : %s", command);
    }
}


bool OPi5Plus::SCPI::Func_Ping(pchar command)
{
    if (*command != '\0')
    {
        return false;
    }

    SCPI::SendString(":PING");

    return true;
}


#define SU_BEGIN_WITH(string)               \
    if(SU::BeginWith(command, string))      \
    {                                       \
        command += std::strlen(string);     \
        char *pos = nullptr;                \
        (void)pos;


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


bool OPi5Plus::SCPI::Func_FPGA(pchar command)
{
    if (*command < '0' || *command > '9')
    {
        return false;
    }

    int num_reg = (int)((*command) & 0x0F);

    command++;

    if (*command != ':')
    {
        return false;
    }

    command++;

    SU_BEGIN_WITH("LENGTH ")                                    // :FPGA:REG:LENGTH
        uint length = std::strtoul(command, &pos, 16);

        if (SU::CharIs(*pos, " :"))
        {
            FPGA::Reg::SetLength(num_reg, length);

            return true;
        }

        return false;
    }
    else SU_BEGIN_WITH("WRITE ")                                // :FPGA:REG:WRITE
        uint value = std::strtoul(command, &pos, 16);

        if (SU::CharIs(*pos, " :"))
        {
            FPGA::Reg::Write(num_reg, value);

            return true;
        }

        return false;
    }

    return false;
}


bool OPi5Plus::SCPI::Func_SCAN(pchar command)
{
    SU_BEGIN_WITH("START ")
        uint period = std::strtoul(command, &pos, 10);

        if (SU::CharIs(*pos, " :"))
        {
            Device::EmergencyStart();

            FPGA::StartScan(period);

            return true;
        }

        return false;
    }
    else SU_BEGIN_WITH("STOP")
        Device::EmergencyStop();

        FPGA::StopScan();

        return true;
    }

    return false;
}


bool OPi5Plus::SCPI::Func_Source50V(pchar command)
{
    SU_BEGIN_WITH("PLUS ")
        uint value = std::strtoul(command, &pos, 10);

        if (value == 0)
        {
            Source50V::Enable50Plus(false);
            return true;
        }
        else if (value == 1)
        {
            Source50V::Enable50Plus(true);
            return true;
        }
    }
    else SU_BEGIN_WITH("MINUS ")
        uint value = std::strtoul(command, &pos, 10);

        if (value == 0)
        {
            Source50V::Enable50Minus(false);
            return true;
        }
        else if (value == 1)
        {
            Source50V::Enable50Minus(true);
            return true;
        }
    }

    return false;
}


bool OPi5Plus::SCPI::Func_Upgrade(pchar command)
{
    SU_BEGIN_WITH("START ")
        int size = (int)std::strtoul(command, &pos, 10);

        if (size > 1)
        {
            Bootloader::Run();

            // Здесь не надо ничего возвращать - переходим на загрузчик, из которого выход только на перезагрузку
        }
    }

    return false;
}


bool OPi5Plus::SCPI::Func_Info(pchar)
{
    LOG_WRITE("STM32 : Version : %d, Date build : %s", VERSION_BUILD, DATE_BUILD);

    OPi5Plus::SCPI::SendFormat(":INFO:STM32 : SION : %d, DATE : %s", VERSION_BUILD, DATE_BUILD);

    return true;
}


bool OPi5Plus::SCPI::Func_DAC(pchar command)
{
    if (*command < '0' || *command > '9')
    {
        return false;
    }

    int num_dac = (int)((*command) & 0x0F);

    command++;

    if (*command != ':')
    {
        return false;
    }

    command++;

    SU_BEGIN_WITH("LENGTH ")                              // :DAC:LENGTH
        uint length = std::strtoul(command, &pos, 10);

        if(SU::CharIs(*pos, " :"))
        {
            ChipDAC::Get((ChipDAC::E)num_dac).SetLength(length);

            return true;
        }

        return false;
    }
    else SU_BEGIN_WITH("WRITE ")                          // :DAC:WRITE
        uint value = std::strtoul(command, &pos, 16);

        if (SU::CharIs(*pos, " :"))
        {
            ChipDAC::Get((ChipDAC::E)num_dac).WriteValue(value);

            return true;
        }

        return false;
    }

    return false;
}


bool OPi5Plus::SCPI::Func_REG(pchar command)
{
    if (*command < '0' || *command > '9')
    {
        return false;
    }

    int num_reg = (int)((*command) & 0x0F);

    command++;

    if (*command != ':')
    {
        return false;
    }

    command++;

    SU_BEGIN_WITH("LENGTH ")                              // :REG:LENGTH
        uint length = std::strtoul(command, &pos, 16);

        if (SU::CharIs(*pos, " :"))
        {
            ChipREG::Get((ChipREG::E)num_reg).SetLength(length);

            return true;
        }

        return false;
    }
    else SU_BEGIN_WITH("WRITE ")                          // :REG:WRITE
        uint value = std::strtoul(command, &pos, 16);

        if (SU::CharIs(*pos, " :"))
        {
            ChipREG::Get((ChipREG::E)num_reg).WriteValue(value);

            return true;
        }

        return false;
    }

    return false;
}


bool OPi5Plus::SCPI::Func_CHIP_REG(pchar)
{
    return false;
}


void OPi5Plus::SCPI::SendFormat(pchar format, ...)
{
    char message[1024];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    HAL_USART1::TransmitString(message);
}


void OPi5Plus::SCPI::SendString(pchar line)
{
    HAL_USART1::TransmitString(line);
}
