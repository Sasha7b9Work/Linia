// 2025/6/3 14:01:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageChip.h"
#include "MainWindow.h"


PageChip::PageChip(wxNotebook *parent, const wxString &title) :
    wxPanel(parent)
{
    wxPanel::SetName(title);

    wxPanel::SetBackgroundColour(parent->GetThemeBackgroundColour());

    wxSize size_button{ 75, BUTTON_HEIGHT };
    btnReturn = new wxButton(this, wxID_ANY, "Закрыть", { MainWindow::WIDTH - size_button.x - 15, 0 }, size_button);

    Bind(wxEVT_RIGHT_DOWN, &PageChip::OnRightClick, this);
    Bind(wxEVT_BUTTON, &PageChip::OnEventButton, this);
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


void PageChip::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnReturn->GetId())
    {
        MainWindow::self->SetMode(ModeMainWindow::Standard);
    }
}


void PageChip::FillRegisterBS(RegFPGA *reg)
{
    std::vector<StructDescription> desc3; //-V827

    std::vector<StructDescription::CommandStruct> commandsLimitCurrent; //-V827 //-V826
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b000, "200 нA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b001, "2 мкA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b010, "20 мкA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b011, "200 мкA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b100, "2 мA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b101, "20 мA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b110, "200 мA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b111, "2 A" });
    desc3.emplace_back(StructDescription{ 0, 3, "ппт", "Предел по току", { true, commandsLimitCurrent } });

    std::vector<StructDescription::CommandStruct> commandsLimit20A; //-V826
    commandsLimit20A.emplace_back(StructDescription::CommandStruct{ 0, "Выключено" });
    commandsLimit20A.emplace_back(StructDescription::CommandStruct{ 1, "Включено" });
    desc3.emplace_back(StructDescription{ 3, 1, "п", "Предел 20А", { true, commandsLimit20A } });

    std::vector<StructDescription::CommandStruct> commandsUc; //-V826
    commandsUc.emplace_back(StructDescription::CommandStruct{ 0, "Подключение " });
    commandsUc.emplace_back(StructDescription::CommandStruct{ 1, "Отключение " });
    desc3.emplace_back(StructDescription{ 4, 1, "U", "Uc", { true, commandsUc } });

    reg->SetDescriptionBits(0, desc3);
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


wxButton *PageChip::GetButtonReturn()
{
    return btnReturn;
}
