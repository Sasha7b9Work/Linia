// 2025/10/23 12:29:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/SCPI/SCPI.h"



namespace SCPI
{
    static bool FuncCommand1(pchar);

    StructSCPI head[] =
    {
        { "COMMAND1", FuncCommand1, nullptr },
        { nullptr, nullptr, nullptr }
    };
}


bool SCPI::FuncCommand1(pchar)
{
    return false;
}
