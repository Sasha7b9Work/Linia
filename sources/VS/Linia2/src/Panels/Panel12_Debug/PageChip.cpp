// 2025/6/3 14:01:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel12_Debug/PageChip.h"


PageChip::PageChip(wxNotebook *parent, const wxString &title) :
    wxPanel(parent, wxID_ANY)
{
    wxPanel::SetName(title);

    wxPanel::SetBackgroundColour(parent->GetThemeBackgroundColour());
}


void PageChip::AppendRegister(PanelRegister *reg)
{
    int y = 0;

    if (registers.size())
    {
        y = registers[registers.size() - 1]->GetPosition().y + registers[registers.size() - 1]->GetSize().y - 1;
    }

    reg->SetPosition({ 0, y });

    registers.push_back(reg);
}
