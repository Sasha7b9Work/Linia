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

wxString PointCount::Name(E points)
{
    return points == Points_20 ? "20" : "50";
}


wxString StartTrigger::Name(E trigger)
{
    return trigger == External ? "EXT" : "INT";
}


wxString StartMode::Name(E mode)
{
    return mode == Multiple ? "MULT" : "SINGLE";
}


wxString MeasMode::Name(E mode)
{
    switch (mode)
    {
    case Manual: return "MANUAL";
    case Auto_1: return "AUTO_1";
    case Auto_2: return "AUTO_2";
    case Auto_3: return "AUTO_3";
    case Auto_4: return "AUTO_4";
    case Auto_5: return "AUTO_5";
    case Count:
    default: return "MANUAL";
    }
}


wxString DacCode::Name(E code)
{
    switch (code)
    {
    case Code_100: return "100";
    case Code_110: return "110";
    case Code_120: return "120";
    case Code_130: return "130";
    case Code_140: return "140";
    case Code_150: return "150";
    case Count:
    default: return "130";
    }
}

uint8_t DacCode::Value(E code)
{
    switch (code)
    {
    case Code_100: return 100;
    case Code_110: return 110;
    case Code_120: return 120;
    case Code_130: return 130;
    case Code_140: return 140;
    case Code_150: return 150;
    case Count:
    default: return 130;
    }
}


wxString PulseState::Name(E state)
{
    return state == Enabled ? "ON" : "OFF";
}


wxString ElementType::Name(E type)
{
    return type == Multipole ? "MULTI" : "TWO";
}
