// 2025/08/19 14:10:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Tests/Tests.h"
#include "Panels/Panel03_Config/PanelScheme/PanelScheme.h"
#include "Utils/Configurator.h"
#include "Panels/Panel03_Config/Panel03_Config.h"


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


bool TypeCommutation::IsInternal()
{
    return PanelScheme::self->comboCommutation->GetCurrentSelection() == 0;
}


void Test::Load(pchar file_name)
{
    Config::SetFile(wxGetCwd() + "/" + file_name);

    PanelConfig::self->Unpack();

    Config::SetFile("");
}


void Test::Save(pchar file_name)
{
    Config::SetFile(wxGetCwd() + "/" + file_name);

    PanelConfig::self->Pack();

    Config::SetFile("");
}


