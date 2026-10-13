// 2025/09/04 16:37:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Settings/Tests/SettingsTests.h"
#pragma warning(push, 0)
#include <wx/arrstr.h>
#pragma warning(pop)


const Chan ChC(Chan::C);
const Chan ChB(Chan::B);
const Chan ChS(Chan::S);


pchar Chan::Name() const
{
    static const pchar names[Count] =
    {
        "C",
        "B",
        "S",
        "E"
    };

    return names[value];
}


pchar TypeScan::NameShort(E v)
{
    static const pchar names[Count] =
    {
        "SYN_P",
        "SYN_N",
        "AC",
        "DC_P",
        "DC_N",
        "IMP_P",
        "IMP_N"
    };

    return names[v];
}


pchar TypeScan::NameFileICO(E v)
{
    static pchar names[Count] =
    {
        "icons/graphs/sin_hi.ico",
        "icons/graphs/sin_lo.ico",
        "icons/graphs/sin.ico",
        "icons/graphs/triang_up.ico",
        "icons/graphs/triang_down.ico",
        "icons/graphs/imp_up.ico",
        "icons/graphs/imp_down.ico"
    };

    if (v < Count)
    {
        return names[v];
    }

    LOG_ERROR("Very big index");

    return "";
}


pchar TypeScan::NameGUI(E v)
{
    static pchar names[Count] =
    {
        "Положительные полуволны",
        "Отрицательные полуволны",
        "Синусоидальное",
        "Положительное постоянное",
        "Отрицательное постоянное",
        "Положительные импульсы",
        "Отрицательные импульсы"
    };

    if (v < Count)
    {
        return names[v];
    }

    LOG_ERROR("Very big index");

    return "";
}
