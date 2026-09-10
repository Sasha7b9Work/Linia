// 2025/09/04 16:37:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Settings/Tests/SettingsTests.h"
#pragma warning(push, 0)
#include <wx/arrstr.h>
#pragma warning(pop)


const Chan ChC(Chan::_C);
const Chan ChB(Chan::_B);
const Chan ChS(Chan::_S);


bool TypeCategory::IsBCE()
{
    E v = Current();

    return (v == BCE_N || v == BCE_P || v == BCSE_N || v == BCSE_P);
}


bool TypeCategory::IsGDS()
{
    E v = Current();

    return (v == GDS_N || v == GDS_P || v == GDBS_N || v == GDBS_P);
}


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


pchar ModeMeas::Name(E mode)
{
    return mode == U ? "U" : "I";
}


pchar TypeScan::NameShort(E v)
{
    static const pchar names[Count] =
    {
        "IMP_P",
        "IMP_N",
        "DC_P",
        "DC_N",
        "SYN_P",
        "SYN_N",
        "AC"
    };

    return names[v];
}


pchar TypeScan::NameFileICO(E v)
{
    static pchar names[Count] =
    {
        "icons/graphs/imp_up.ico",
        "icons/graphs/imp_down.ico",
        "icons/graphs/triang_up.ico",
        "icons/graphs/triang_down.ico",
        "icons/graphs/sin_hi.ico",
        "icons/graphs/sin_lo.ico",
        "icons/graphs/sin.ico"
//        "icons/graphs/imp_hi_osc.ico",
//        "icons/graphs/imp_hi_IVC.ico"
    };

    if (v < Count)
    {
        return names[v];
    }

    LOG_ERROR("Very big index");

    return "";
}


pchar TypeScan::_NameGUI(E v)
{
    static pchar names[Count] =
    {
        "Положительные импульсы",
        "Отрицательные импульсы",
        "Положительное постоянное",
        "Отрицательное постоянное",
        "Положительные полуволны",
        "Отрицательные полуволны",
        "Синусоидальное"
    };

    if (v < Count)
    {
        return names[v];
    }

    LOG_ERROR("Very big index");

    return "";
}


TypeCategory::E TypeCategory::Current()
{
    return TypeCategory::BCE_N;
}


bool Chan::IsVisible() const
{
    TypeCategory::E cat = TypeCategory::Current();

    if (value == Chan::_B)
    {
        if (cat == TypeCategory::Diod || cat == TypeCategory::Resistor || cat == TypeCategory::Capacitor)
        {
            return false;
        }
    }

    if (value == Chan::_S)
    {
        if (cat == TypeCategory::Diod ||
            cat == TypeCategory::Resistor ||
            cat == TypeCategory::Capacitor ||
            cat == TypeCategory::BCE_N ||
            cat == TypeCategory::BCE_P ||
            cat == TypeCategory::GDS_N ||
            cat == TypeCategory::GDS_P)
        {
            return false;
        }
    }

    return true;
}
