// 2025/6/3 14:01:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageChip.h"
#include "MainWindow.h"


PageChip::PageChip(wxNotebook *parent, const wxString &title) :
    wxPanel(parent)
{
    wxPanel::SetName(title);

    wxPanel::SetBackgroundColour(parent->GetThemeBackgroundColour());

    Bind(wxEVT_RIGHT_DOWN, &PageChip::OnRightClick, this);
}


void PageChip::AppendRegister(Register *reg)
{
    int y = 0;

    if (registers.size())
    {
        y = registers[registers.size() - 1]->GetPosition().y + registers[registers.size() - 1]->GetSize().y - 1;
    }

    reg->SetPosition({ 0, y });

    registers.push_back(reg);
}


void PageChip::OnRightClick(wxMouseEvent &event)
{
    wxMenu menu;

    itemReturn = menu.Append(wxID_ANY, "Закрыть");

    Bind(wxEVT_MENU, &PageChip::OnMenuEvent, this);

    PopupMenu(&menu);

    event.Skip();
}


void PageChip::OnMenuEvent(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == itemReturn->GetId())
    {
        MainWindow::self->SetMode(ModeMainWindow::Standard);
    }
}


void PageChip::FillRegisterBS(RegFPGA */*reg*/)
{

}


void PageChip::Pack()
{
    for (auto *reg : registers)
    {
        reg->Pack();
    }
}


void PageChip::Unpack()
{
    for (auto *reg : registers)
    {
        reg->Unpack();
    }
}
