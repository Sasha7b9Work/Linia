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


void PageChip::FillRegisterBS(RegFPGA *reg)
{
    std::vector<StructDescription> desc3; //-V827

    std::vector<StructDescription::CommandStruct> commandsLimitCurrent; //-V827 //-V826
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b000, "20 нA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b001, "200 нA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b010, "2 мкA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b011, "20 мкA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b100, "200 мкA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b101, "2 мA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b110, "20 мA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b111, "200 мA" });
    desc3.emplace_back(StructDescription{ 0, 3, "ппт", "Предел по току", { true, commandsLimitCurrent } });

    std::vector<StructDescription::CommandStruct> commandsLimit2A; //-V826
    commandsLimit2A.emplace_back(StructDescription::CommandStruct{ 0, "Выключено" });
    commandsLimit2A.emplace_back(StructDescription::CommandStruct{ 1, "Включено" });
    desc3.emplace_back(StructDescription{ 3, 1, "п", "Предел 2А", { true, commandsLimit2A } });

    std::vector<StructDescription::CommandStruct> commandsLimit10A; //-V826
    commandsLimit10A.emplace_back(StructDescription::CommandStruct{ 0, "Выключено" });
    commandsLimit10A.emplace_back(StructDescription::CommandStruct{ 1, "Включено" });
    desc3.emplace_back(StructDescription{ 4, 1, "п", "Предел 10A", { true, commandsLimit10A } });

    std::vector<StructDescription::CommandStruct> commandsOS_OPA; //-V826
    commandsOS_OPA.emplace_back(StructDescription::CommandStruct{ 0, "В режиме U" });
    commandsOS_OPA.emplace_back(StructDescription::CommandStruct{ 1, "В режиме I" });
    desc3.emplace_back(StructDescription{ 5, 1, "о", "Ucl Подключение ОС вых ОУ", { true, commandsOS_OPA } });

    std::vector<StructDescription::CommandStruct> commandsZeroCorr; //-V826
    commandsZeroCorr.emplace_back(StructDescription::CommandStruct{ 0, "Выключен" });
    commandsZeroCorr.emplace_back(StructDescription::CommandStruct{ 1, "Включен" });
    desc3.emplace_back(StructDescription{ 6, 1, "г", "CLBR Геркон \"Корр. 0 измерителя Uии\"", { true, commandsZeroCorr } });

    std::vector<StructDescription::CommandStruct> commandsAz; //-V826
    commandsAz.emplace_back(StructDescription::CommandStruct{ 0, "Стоп" });
    commandsAz.emplace_back(StructDescription::CommandStruct{ 1, "Измерение" });
    desc3.emplace_back(StructDescription{ 7, 1, "р", "Az Режим", { true, commandsAz } });

    std::vector<StructDescription::CommandStruct> commandsDivider; //-V826
    commandsDivider.emplace_back(StructDescription::CommandStruct{ 0b00, "1:1" });
    commandsDivider.emplace_back(StructDescription::CommandStruct{ 0b01, "1:4" });
    commandsDivider.emplace_back(StructDescription::CommandStruct{ 0b10, "1:10" });
    desc3.emplace_back(StructDescription{ 8, 2, "д", "Делитель в режиме Источник U", { true, commandsDivider } });

    std::vector<StructDescription::CommandStruct> commandsQ10; //-V826
    commandsQ10.emplace_back(StructDescription::CommandStruct{ 0, "Источник U - ограничение по I" });
    commandsQ10.emplace_back(StructDescription::CommandStruct{ 1, "Режим 1:1 источника тока" });
    desc3.emplace_back(StructDescription{ 10, 1, "р", "Режим", { true, commandsQ10 } });

    std::vector<StructDescription::CommandStruct> commandsRange; //-V826
    commandsRange.emplace_back(StructDescription::CommandStruct{ 0b00, "1" });
    commandsRange.emplace_back(StructDescription::CommandStruct{ 0b01, "4" });
    commandsRange.emplace_back(StructDescription::CommandStruct{ 0b10, "10" });
    desc3.emplace_back(StructDescription{ 12, 2, "д", "Диапазон измерения", { true, commandsRange } });


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
