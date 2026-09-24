// 2025/10/23 12:29:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "IPPP/SCPI/SCPI.h"
#include "Utils/StringUtils.h"
#include "IPPP/Real/PinsDevice.h"
#include "GUI/PageDebug/Notebook/PageSTM32.h"


namespace SCPI
{
    static bool FuncPinIn(pchar);
    static bool FuncLog(pchar);
    static bool FuncWarning(pchar);
    static bool FuncError(pchar);
    static bool FuncLogMessage(pchar type, pchar message);
    static bool FuncPing(pchar);
    static bool FuncUpgrade(pchar);

    StructSCPI head[] =
    {
        { "PININ",    FuncPinIn,   nullptr },
        { "LOG",      FuncLog,     nullptr },
        { "WARNING",  FuncWarning, nullptr },
        { "ERROR",    FuncError,   nullptr },
        { "PING",     FuncPing,    nullptr },
        { "UPGRADE",  FuncUpgrade, nullptr },
        { nullptr,    nullptr,     nullptr }
    };
}


bool SCPI::FuncPinIn(pchar command)
{
    Words words{ command };

    SU::SplitToWords(command, words, " :");

    if (words.Size() == 3 && words[1] == "STATE")
    {
        for (int i = 0; i < PinDevice::Count; i++)
        {
            PinInDevice *pin = pinsD[i];

            if (pin->Name() == words[0])
            {
                uint value = SU::UIntFromString(words[2].c_str());

                pin->SetState(value != 0);

                return true;
            }
        }

        return false;
    }

    return false;
}


bool SCPI::FuncLog(pchar command)
{
    return FuncLogMessage("LOG    ", command);
}


bool SCPI::FuncWarning(pchar command)
{
    return FuncLogMessage("WARNING", command);
}


bool SCPI::FuncError(pchar command)
{
    return FuncLogMessage("ERROR  ", command);
}


bool SCPI::FuncLogMessage(pchar type, pchar message)
{
    Log::LogMCU(type, message);

    return true;
}


bool SCPI::FuncPing(pchar)
{
    LOG_WRITE("Ping");

    return true;
}


#define SU_BEGIN_WITH(string)               \
    if(SU::BeginWith(command, string))      \
    {                                       \
        command += std::strlen(string);     \
        char *pos = nullptr;                \
        (void)pos;


bool SCPI::FuncUpgrade(pchar command)
{
    SU_BEGIN_WITH("START")
        PageSTM32::self->OnConfirmUpgradeStart();
    return true;
}
    else SU_BEGIN_WITH("HEAD ")
        int num_block = (int)std::strtoul(command, &pos, 10);
        int size = (int)std::strtoul(pos + 1, &pos, 10);
        uint crc32 = std::strtoul(pos + 1, &pos, 16);
        PageSTM32::self->OnConfirmHeadBlock(num_block, size, crc32);
        return true;
    }
    else SU_BEGIN_WITH("CONTENT ")
        int num_block = (int)std::strtoul(command, &pos, 10);
        int size = (int)std::strtoul(pos + 1, &pos, 10);
        uint crc32 = std::strtoul(pos + 1, &pos, 16);
        PageSTM32::self->OnConfirmContentBlock(num_block, size, crc32);
        return true;
    }
    else SU_BEGIN_WITH("END ")
        int size = (int)std::strtoul(command, &pos, 10);
        uint crc32 = std::strtoul(pos + 1, &pos, 16);
        PageSTM32::self->OnConfirmUpgradeEnd(size, crc32);
        return true;
    }
    else SU_BEGIN_WITH("ERROR")
        PageSTM32::self->OnError();
        return true;
    }

    return false;
}
