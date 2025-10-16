// 2025/10/07 12:05:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/PageChannelB.h"


PageChannelB *PageChannelB::self = nullptr;


PageChannelB::PageChannelB(wxNotebook *parent) :
    PageChip(parent, "Канал B")
{
    self = this;

    AppendRegister(new RegAD5531(this, "DAC2"));

    AppendRegister(new RegAD5531(this, "DAC3"));

    RegFPGA *reg3 = new RegFPGA(this, "REG3", 32);

    std::vector<StructDescription> desc3;

    std::vector<StructDescription::CommandStruct> commandsLimitCurrent;
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b000, "200 нA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b001, "2 мкA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b010, "20 мкA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b011, "200 мкA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b100, "2 мA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b101, "20 мA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b110, "200 мA" });
    commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b111, "2 A" });
    desc3.emplace_back(StructDescription{ 0, 3, "ппт", "Предел по току", { true, commandsLimitCurrent } });

    std::vector<StructDescription::CommandStruct> commandsLimit20A;
    commandsLimit20A.emplace_back(StructDescription::CommandStruct{ 0, "Выключено" });
    commandsLimit20A.emplace_back(StructDescription::CommandStruct{ 1, "Включено" });
    desc3.emplace_back(StructDescription{ 3, 1, "п", "Предел 20А", { true, commandsLimit20A } });

    std::vector<StructDescription::CommandStruct> commandsUc;
    commandsUc.emplace_back(StructDescription::CommandStruct{ 0, "Подключение " });
    commandsUc.emplace_back(StructDescription::CommandStruct{ 1, "Отключение " });
    desc3.emplace_back(StructDescription{ 4, 1, "U", "Uc", { true, commandsUc }});

    reg3->SetDescriptionBits(0, desc3);

    AppendRegister(reg3);
}
