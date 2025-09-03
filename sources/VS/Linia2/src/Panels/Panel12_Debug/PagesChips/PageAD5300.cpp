// 2025/08/30 11:32:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel12_Debug/PagesChips/PageAD5300.h"


PageAD5300 *PageAD5300::self = nullptr;


PageAD5300::PageAD5300(wxNotebook *parent) : PageChip(parent, "AD5300")
{
    self = this;

    wxArrayString names;
    for (int i = 0; i < 4; i++)
    {
        names.push_back("X");
    }
    for (int i = 0; i < 8; i++)
    {
        names.push_back(wxString::Format("D%d", i));
    }
    names.push_back("PD0");
    names.push_back("PD1");
    for (int i = 0; i < 2; i++)
    {
        names.push_back("X");
    }

    PanelRegister *regInput = new PanelRegister(this, "Input Register", 16);

    regInput->SetNamesBits(names);

    std::vector<StructDescription::CommandStruct> commands;
    commands.push_back({ 0, "Normal operation" });
    commands.push_back({ 1, "1 kOhm to GND" });
    commands.push_back({ 2, "100 kOhm to GND" });
    commands.push_back({ 3, "Three-State" });

    std::vector<StructDescription> desc0;
    desc0.push_back({  0, 4, "not used" });
    desc0.push_back({  4, 8, "DATA BITS", { true } });
    desc0.push_back({ 12, 2, "POWER_DOWN MODES", { false, true, commands} });
    desc0.push_back({ 14, 2, "not used" });

    regInput->SetDescriptionBits(0, desc0);

    AppendRegister(regInput);
}
