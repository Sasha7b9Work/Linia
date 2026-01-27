// 2025/10/07 12:05:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageChannelB.h"


PageChannelB *PageChannelB::self = nullptr;


PageChannelB::PageChannelB(wxNotebook *parent) :
    PageChip(parent, "Канал B")
{
    self = this;

    AppendRegister(new RegAD5531(this, dacs[2]));

    AppendRegister(new RegAD5531(this, dacs[3]));

    {
        RegFPGA *reg3 = new RegFPGA(this, regs[3]);

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
        desc3.emplace_back(StructDescription{ 0, 3, "ппт", "Предел по току", { false, commandsLimitCurrent } });

        std::vector<StructDescription::CommandStruct> commandsLimit2A; //-V826
        commandsLimit2A.emplace_back(StructDescription::CommandStruct{ 0, "Выключено" });
        commandsLimit2A.emplace_back(StructDescription::CommandStruct{ 1, "Включено" });
        desc3.emplace_back(StructDescription{ 3, 1, "п", "Предел 2А", { false, commandsLimit2A } });

        std::vector<StructDescription::CommandStruct> commandsLimit10A; //-V826
        commandsLimit10A.emplace_back(StructDescription::CommandStruct{ 0, "Выключено" });
        commandsLimit10A.emplace_back(StructDescription::CommandStruct{ 1, "Включено" });
        desc3.emplace_back(StructDescription{ 4, 1, "п", "Предел 10A", { false, commandsLimit10A } });

        std::vector<StructDescription::CommandStruct> commandsOS_OPA; //-V826
        commandsOS_OPA.emplace_back(StructDescription::CommandStruct{ 0, "В режиме U" });
        commandsOS_OPA.emplace_back(StructDescription::CommandStruct{ 1, "В режиме I" });
        desc3.emplace_back(StructDescription{ 5, 1, "о", "Ucl Подключение ОС вых ОУ", { false, commandsOS_OPA } });

        std::vector<StructDescription::CommandStruct> commandsZeroCorr; //-V826
        commandsZeroCorr.emplace_back(StructDescription::CommandStruct{ 0, "Выключен" });
        commandsZeroCorr.emplace_back(StructDescription::CommandStruct{ 1, "Включен" });
        desc3.emplace_back(StructDescription{ 6, 1, "г", "CLBR Геркон \"Корр. 0 измерителя Uии\"", { false, commandsZeroCorr } });

        std::vector<StructDescription::CommandStruct> commandsAz; //-V826
        commandsAz.emplace_back(StructDescription::CommandStruct{ 0, "Стоп" });
        commandsAz.emplace_back(StructDescription::CommandStruct{ 1, "Измерение" });
        desc3.emplace_back(StructDescription{ 7, 1, "р", "Az Режим", { false, commandsAz } });

        std::vector<StructDescription::CommandStruct> commandsDivider; //-V826
        commandsDivider.emplace_back(StructDescription::CommandStruct{ 0b00, "1:1" });
        commandsDivider.emplace_back(StructDescription::CommandStruct{ 0b01, "1:4" });
        commandsDivider.emplace_back(StructDescription::CommandStruct{ 0b10, "1:10" });
        desc3.emplace_back(StructDescription{ 8, 2, "д", "Делитель в режиме Источник U", { false, commandsDivider } });

        std::vector<StructDescription::CommandStruct> commandsQ10; //-V826
        commandsQ10.emplace_back(StructDescription::CommandStruct{ 0, "Источник U - ограничение по I" });
        commandsQ10.emplace_back(StructDescription::CommandStruct{ 1, "Режим 1:1 источника тока" });
        desc3.emplace_back(StructDescription{ 10, 1, "р", "Режим", { false, commandsQ10 } });

        std::vector<StructDescription::CommandStruct> commandsRange; //-V826
        commandsRange.emplace_back(StructDescription::CommandStruct{ 0b00, "1" });
        commandsRange.emplace_back(StructDescription::CommandStruct{ 0b01, "4" });
        commandsRange.emplace_back(StructDescription::CommandStruct{ 0b10, "10" });
        desc3.emplace_back(StructDescription{ 12, 2, "д", "Диапазон измерения", { false, commandsRange } });

        std::vector<StructDescription::CommandStruct> commandsCap; //-V826
        commandsCap.emplace_back(StructDescription::CommandStruct{ 0, "Включён" });
        commandsCap.emplace_back(StructDescription::CommandStruct{ 1, "Отключён" });
        desc3.emplace_back(StructDescription{ 14, 1, "к", "On14 Дополнительный конденсатор DET", { false, commandsCap } });

        std::vector<StructDescription::CommandStruct> commandsBuffer; //-V826
        commandsBuffer.emplace_back(StructDescription::CommandStruct{ 0, "2" });
        commandsBuffer.emplace_back(StructDescription::CommandStruct{ 1, "3" });
        desc3.emplace_back(StructDescription{ 15, 1, "б", "On15 Выбор буфера", { false, commandsBuffer } });

        std::vector<StructDescription::CommandStruct> commandsMeas; //-V826
        commandsMeas.emplace_back(StructDescription::CommandStruct{ 0, "I" });
        commandsMeas.emplace_back(StructDescription::CommandStruct{ 1, "U" });
        desc3.emplace_back(StructDescription{ 17, 1, "и", "Режим измерения", { false, commandsMeas } });

        std::vector<StructDescription::CommandStruct> commandsImp; //-V826
        commandsImp.emplace_back(StructDescription::CommandStruct{ 0, "Запрещены" });
        commandsImp.emplace_back(StructDescription::CommandStruct{ 1, "Разрешены" });
        desc3.emplace_back(StructDescription{ 18, 1, "и", "ImpV Импульсы в канале B режим высокого напряжения", { false, commandsImp } });

        std::vector<StructDescription::CommandStruct> commandsImpI; //-V826
        commandsImpI.emplace_back(StructDescription::CommandStruct{ 0, "Запрещены" });
        commandsImpI.emplace_back(StructDescription::CommandStruct{ 1, "Разрешены" });
        desc3.emplace_back(StructDescription{ 19, 1, "и", "ImpI Импульсы в режиме высокого тока", { false, commandsImpI } });

        std::vector<StructDescription::CommandStruct> commands55; //-V826
        commands55.emplace_back(StructDescription::CommandStruct{ 0, "Отключено" });
        commands55.emplace_back(StructDescription::CommandStruct{ 1, "Включено" });
        desc3.emplace_back(StructDescription{ 22, 1, "5", "U+ 55 В", { false, commands55 } });

        reg3->SetDescriptionBits(0, desc3);

        AppendRegister(reg3);
    }
}
