// 2025/6/3 22:47:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PageAD5697.h"
#include "Panels/Debug/PanelRegister.h"


PageAD5697 *PageAD5697::self = nullptr;


PageAD5697::PageAD5697(wxNotebook *parent) : PageChip(parent, "AD5697")
{
    self = this;

    wxArrayString names;
    for (int i = 0; i < 4; i++)
    {
        names.push_back("X");
    }
    for (int i = 0; i < 12; i++)
    {
        names.push_back(wxString::Format("D%d", i));
    }
    names.push_back("DAC A");
    names.push_back("0");
    names.push_back("0");
    names.push_back("DAC B");
    for (int i = 0; i < 4; i++)
    {
        names.push_back(wxString::Format("C%d", i));
    }

    PanelRegister *panel = new PanelRegister(this, "Input Register", 24, false);

    panel->SetNamesBits(names);
}
