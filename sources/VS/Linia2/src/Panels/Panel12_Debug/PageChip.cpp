// 2025/6/3 14:01:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel12_Debug/PageChip.h"
#include "MainWindow.h"


PageChip::PageChip(wxNotebook *parent, const wxString &title) :
    wxPanel(parent, wxID_ANY)
{
    wxPanel::SetName(title);

    wxPanel::SetBackgroundColour(parent->GetThemeBackgroundColour());

    wxSize size_button{ 75, BUTTON_HEIGHT };
    new wxButton(this, ID_MENU_DEBUG_CLOSE, _L("Закрыть"), { MainWindow::WIDTH - size_button.x - 5, 0 }, size_button);

    Bind(wxEVT_RIGHT_DOWN, &PageChip::OnRightClick, this);
    Bind(wxEVT_BUTTON, &PageChip::OnEventButton, this);
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


void PageChip::OnRightClick(wxMouseEvent &event)
{
    wxMenu menu;

    menu.Append(ID_MENU_DEBUG_CLOSE, _L("Закрыть"));

    Bind(wxEVT_MENU, &PageChip::OnMenuEvent, this);

    PopupMenu(&menu);

    event.Skip();
}


void PageChip::OnMenuEvent(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_MENU_DEBUG_CLOSE)
    {
        MainWindow::self->SetMode(ModeMainWindow::Standard);
    }
}


void PageChip::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_MENU_DEBUG_CLOSE)
    {
        MainWindow::self->SetMode(ModeMainWindow::Standard);
    }
}
