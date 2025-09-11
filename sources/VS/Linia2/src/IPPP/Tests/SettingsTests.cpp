// 2025/09/04 16:37:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Tests/SettingsTests.h"
#include "Panels/Panel03_Config/PanelScheme/PanelScheme.h"


Chan ChC(Chan::_C);
Chan ChB(Chan::_B);
Chan ChS(Chan::_S);


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


pchar ModeSource::Name(E mode)
{
    return mode == U ? "U" : "I";
}


pchar ModeMeas::Name(E mode)
{
    return mode == U ? "U" : "I";
}


pchar TypeScan::Name(E v)
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


const pchar StateJack::names[StateJack::Count][2] =
{
    { "C",        "C" },
    { "B",        "B" },
    { "S",        "S" },
    { "E",        "E" },
    { "общий",    "GENERAL" },
    { "общий 1к", "GENERAL_1K"},
    { "обрыв",    "BREAK" }
};


pchar StateJack::NameHardware(E v)
{
    return names[v][1];
}


pchar StateJack::NameGUI(E v)
{
    return names[v][0];
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


bool TypeCommutation::IsInternal()
{
    return PanelScheme::self->comboCommutation->GetCurrentSelection() == 0;
}


pchar MicroChip::Name(E v)
{
    static const pchar names[Count] =
    {
        "ADC_7691_1",
        "ADC_7691_2"
        "DAC_5300_1",
        "DAC_5300_2",
        "DAC_5443",
        "DAC_5531"
    };

    return names[v];
}
