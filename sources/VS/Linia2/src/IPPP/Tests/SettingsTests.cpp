// 2025/09/04 16:37:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Tests/SettingsTests.h"
#include "Panels/Panel03_Config/PanelScheme/PanelScheme.h"


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


wxString StateJack::NameHardware(E state)
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


wxString StateJack::NameGUI(E v)
{
    static const pchar names[Count] =
    {
        "C",
        "B",
        "S",
        "E",
        "общий",
        "общий 1к",
        "обрыв"
    };

    return names[v];
}


wxString FirstQueue::Name(E fq)
{
    return fq == ChannelC ? "C" : "B";
}


Category::E Category::Current()
{
    return (E)PanelScheme::self->comboCategory->GetCurrentChoice();
}


bool Chan::IsVisible() const
{
    Category::E cat = Category::Current();

    if (value == Chan::_B)
    {
        if (cat == Category::Diod || cat == Category::Resistor || cat == Category::Capacitor)
        {
            return false;
        }
    }

    if (value == Chan::_S)
    {
        if (cat == Category::Diod ||
            cat == Category::Resistor ||
            cat == Category::Capacitor ||
            cat == Category::BCE_N ||
            cat == Category::BCE_P ||
            cat == Category::GDS_N ||
            cat == Category::GDS_P)
        {
            return false;
        }
    }

    return true;
}


void StateJack::PrepareArray(wxArrayString &arr, E v1, E v2, E v3, E v4)
{
    arr.Clear();
    arr.push_back(NameGUI(v1));
    arr.push_back(NameGUI(v2));
    if (v3 != Count)
    {
        arr.push_back(NameGUI(v3));
    }
    if (v4 != Count)
    {
        arr.push_back(NameGUI(v4));
    }
}


bool TypeCommutation::IsInternal()
{
    return PanelScheme::self->comboCommutation->GetCurrentSelection() == 0;
}
