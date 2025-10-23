// 2025/10/13 13:54:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/PageFPGA.h"


PageFPGA *PageFPGA::self = nullptr;


PageFPGA::PageFPGA(wxNotebook *parent) :
    PageChip(parent, "ПЛИС")
{
    self = this;

    Register *fpga0 = new RegFPGA(this, fpgas[0]);

    std::vector<StructDescription> desc0;

    std::vector<StructDescription::CommandStruct> commandsStart;
    commandsStart.emplace_back(StructDescription::CommandStruct{ 0, "Однократный" });
    commandsStart.emplace_back(StructDescription::CommandStruct{ 1, "Автоматический" });
    desc0.emplace_back(StructDescription{ 8, 1, "з", "запуск", { true, commandsStart } });

    fpga0->SetDescriptionBits(0, desc0);

    AppendRegister(fpga0);

    AppendRegister(new RegFPGA(this, fpgas[1]));

    AppendRegister(new RegFPGA(this, fpgas[2]));
}


void PageFPGA::Pack()
{

}


void PageFPGA::Unpack()
{

}
