// 2025/09/04 16:37:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Tests/SettingsTests.h"


bool Category::IsBCE()
{
    E v = Current();

    return (v == BCE_N || v == BCE_P || v == BCSE_N || v == BCSE_P);
}


bool Category::IsGDS()
{
    E v = Current();

    return (v == GDS_N || v == GDS_P || v == GDBS_N || v == GDBS_P);
}


wxString Chan::Name() const
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

wxString StateJack::Name(E state)
{
    switch (state)
    {
    case _C: return "C";
    case _B: return "B";
    case _S: return "S";
    case _E: return "E";
    case General: return "GENERAL";
    case General_1k: return "GENERAL_1K";
    case Break: return "BREAK";
    case Count:
    default: return "BREAK";
    }
}

wxString ModeSource::Name(E mode)
{
    return mode == U ? "U" : "I";
}

wxString ModeMeas::Name(E mode)
{
    return mode == U ? "U" : "I";
}

wxString TypeScan::Name(E type)
{
    switch (type)
    {
    case ImpulsePos: return "IMP_P";
    case ImpulseNeg: return "IMP_N";
    case DCPos: return "DC_P";
    case DCNeg: return "DC_N";
    case SYNPos: return "SYN_P";
    case SYNNeg: return "SYN_N";
    case AC: return "AC";
    case Count:
    default: return "";
    }
}


wxString FirstQueue::Name(E fq)
{
    return fq == ChannelC ? "C" : "B";
}


wxString GenerationStup::Name(E gs)
{
    return gs == Enable ? "1" : "0";
}
