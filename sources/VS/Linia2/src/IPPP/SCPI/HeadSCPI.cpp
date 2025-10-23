// 2025/10/23 12:29:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/SCPI/SCPI.h"
#include "Utils/StringUtils.h"
#include "IPPP/Device/PinsDevice.h"


namespace SCPI
{
    static bool FuncPinIn(pchar);

    StructSCPI head[] =
    {
        { "PININ",    FuncPinIn,    nullptr },
        { nullptr,    nullptr,      nullptr }
    };
}


bool SCPI::FuncPinIn(pchar command)
{
    Words words{ command };

    SU::SplitToWords(command, words, " :");

    if (words.Size() == 3 && words[1] == "STATE")
    {
        for (int i = 0; ; i++)
        {
            PinInDevice *pin = pinsD[i];

            if (pin == nullptr)
            {
                return false;
            }

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
