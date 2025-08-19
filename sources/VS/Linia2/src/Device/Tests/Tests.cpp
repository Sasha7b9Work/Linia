// 2025/08/19 14:10:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/Tests/Tests.h"
#include "Panels/Panel03_Config/PanelScheme/PanelScheme.h"


Category::E Category::Current()
{
    return (E)PanelScheme::self->comboCategory->GetCurrentChoice();
}


bool Channel::IsVisible() const
{
    Category::E cat = Category::Current();

    if (value == Channel::_B)
    {
        if (cat == Category::Diod)
        {
            return false;
        }
    }

    if (value == Channel::_S)
    {
        if (cat == Category::Diod ||
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


void StateJack::PrepareArray(wxArrayString &arr, E v1, E v2)
{
    arr.Clear();
    arr.push_back(Name(v1));
    arr.push_back(Name(v2));
}


wxString StateJack::Name(E v)
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


void Test::Load(pchar /*file_name*/)
{
    // CLineDlg::ExtractParamFromTst()
    // CLineDlg::SaveParamToTst(int where) 
}


