// 2025/10/13 13:54:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/PageFPGA.h"


PageFPGA *PageFPGA::self = nullptr;


PageFPGA::PageFPGA(wxNotebook *parent) :
    PageChip(parent, "ПЛИС")
{
    self = this;

    {
        Register *fpga0 = new RegFPGA(this, fpgas[0]);

        std::vector<StructDescription> desc0; //-V827

        std::vector<StructDescription::CommandStruct> commandsStart; //-V826
        commandsStart.emplace_back(StructDescription::CommandStruct{ 0, "Однократный" });
        commandsStart.emplace_back(StructDescription::CommandStruct{ 1, "Автоматический" });
        desc0.emplace_back(StructDescription{ 8, 1, "з", "запуск", { true, commandsStart } });

        std::vector<StructDescription::CommandStruct> commandsScan; //-V827 //-V826
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0000, "Выключена" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0001, "SIN+" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0010, "SIN-" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0011, "AC" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0100, "DC-" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0101, "DC+" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0110, "IMP+" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0111, "IMP-" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b1000, "Осциллограф" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b1000, "ВАХ" });
        desc0.emplace_back(StructDescription{ 3, 4, "разв", "развёртка", { true, commandsScan } });

        std::vector<StructDescription::CommandStruct> commandsCurvers; //-V827 //-V826
        commandsCurvers.emplace_back(StructDescription::CommandStruct{ 0b000, "1" });
        commandsCurvers.emplace_back(StructDescription::CommandStruct{ 0b001, "2" });
        commandsCurvers.emplace_back(StructDescription::CommandStruct{ 0b010, "4" });
        commandsCurvers.emplace_back(StructDescription::CommandStruct{ 0b011, "8" });
        commandsCurvers.emplace_back(StructDescription::CommandStruct{ 0b100, "10" });
        commandsCurvers.emplace_back(StructDescription::CommandStruct{ 0b101, "10" });
        commandsCurvers.emplace_back(StructDescription::CommandStruct{ 0b110, "10" });
        commandsCurvers.emplace_back(StructDescription::CommandStruct{ 0b111, "10" });
        desc0.emplace_back(StructDescription{ 0, 3, "к", "количество кривых ВАХ для DC+, DC-", { true, commandsCurvers } });

        fpga0->SetDescriptionBits(0, desc0);

        AppendRegister(fpga0);
    }

    {
        Register *fpga1 = new RegFPGA(this, fpgas[1]);

        std::vector<StructDescription> desc1;

        std::vector<StructDescription::CommandStruct> commandsDuration; //-V827 //-V826
        commandsDuration.emplace_back(StructDescription::CommandStruct{ 0b000, "0.2 мс" });
        commandsDuration.emplace_back(StructDescription::CommandStruct{ 0b001, "0.2 мс" });
        commandsDuration.emplace_back(StructDescription::CommandStruct{ 0b010, "1.0 мс" });
        commandsDuration.emplace_back(StructDescription::CommandStruct{ 0b011, "0.2 мс" });
        commandsDuration.emplace_back(StructDescription::CommandStruct{ 0b100, "2.0 мс" });
        commandsDuration.emplace_back(StructDescription::CommandStruct{ 0b101, "0.2 мс" });
        commandsDuration.emplace_back(StructDescription::CommandStruct{ 0b110, "0.2 мс" });
        commandsDuration.emplace_back(StructDescription::CommandStruct{ 0b111, "0.2 мс" });
        desc1.emplace_back(StructDescription{ 0, 3, "д", "длит. имп. для IMP+, IMP-(высокое напряжение)", { true, commandsDuration } });

        fpga1->SetDescriptionBits(0, desc1);

        AppendRegister(fpga1);
    }

    {
        Register *fpga2 = new RegFPGA(this, fpgas[2]);

        std::vector<StructDescription> desc2;

        std::vector<StructDescription::CommandStruct> commandsDuration; //-V827 //-V826
        commandsDuration.emplace_back(StructDescription::CommandStruct{ 0b00, "10 мкс" });
        commandsDuration.emplace_back(StructDescription::CommandStruct{ 0b01, "20 мкс" });
        commandsDuration.emplace_back(StructDescription::CommandStruct{ 0b10, "50 мкс" });
        commandsDuration.emplace_back(StructDescription::CommandStruct{ 0b11, "100 мкс" });
        desc2.emplace_back(StructDescription{ 0, 2, "дл", "длит. имп. для IMP+, IMP-(большой ток)", { true, commandsDuration } });

        std::vector<StructDescription::CommandStruct> commandsN; //-V827 //-V826
        commandsN.emplace_back(StructDescription::CommandStruct{ 0b001, "0" });
        commandsN.emplace_back(StructDescription::CommandStruct{ 0b001, "1" });
        commandsN.emplace_back(StructDescription::CommandStruct{ 0b010, "2" });
        commandsN.emplace_back(StructDescription::CommandStruct{ 0b011, "3" });
        commandsN.emplace_back(StructDescription::CommandStruct{ 0b100, "4" });
        commandsN.emplace_back(StructDescription::CommandStruct{ 0b101, "5" });
        commandsN.emplace_back(StructDescription::CommandStruct{ 0b110, "6" });
        commandsN.emplace_back(StructDescription::CommandStruct{ 0b111, "7" });
        desc2.emplace_back(StructDescription{ 2, 3, "N", "N", { true, commandsN } });

        fpga2->SetDescriptionBits(0, desc2);

        AppendRegister(fpga2);
    }
}
