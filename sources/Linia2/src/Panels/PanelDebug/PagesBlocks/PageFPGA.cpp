// 2025/10/13 13:54:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/PageFPGA.h"
#include "Utils/String.h"
#include "Utils/SystemDepend.h"


PageFPGA *PageFPGA::self = nullptr;


PageFPGA::PageFPGA(wxNotebook *parent) :
    PageChip(parent, "ПЛИС")
{
    self = this;

    {
        Register *fpga0 = new RegFPGA(this, fpgas[0]);

        std::vector<StructDescription> desc0; //-V827

        std::vector<StructDescription::CommandStruct> commandsScan; //-V827 //-V826
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0000, "Выключена" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0001, "SIN+" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0010, "SIN-" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0011, "AC" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0100, "DC-" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0101, "DC+" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0110, "IMP+" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0111, "IMP-" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b1000, "IMP-CVC" });
        desc0.emplace_back(StructDescription{ 0, 4, "разв", "развёртка", { true, commandsScan } });

        std::vector<StructDescription::CommandStruct> commandsTypeElem; //-V826
        commandsTypeElem.emplace_back(StructDescription::CommandStruct{ 0, "Диод" });
        commandsTypeElem.emplace_back(StructDescription::CommandStruct{ 1, "Транзистор" });
        desc0.emplace_back(StructDescription{ 4, 1, "з", "запуск", { true, commandsTypeElem } });

        std::vector<StructDescription::CommandStruct> commandsCurvers; //-V827 //-V826
        for (int i = 0; i < 16; i++)
        {
            commandsCurvers.emplace_back(StructDescription::CommandStruct{ (uint16)i, String("%d", i).c_str() });
        }
        desc0.emplace_back(StructDescription{ 5, 4, "к", "количество кривых ВАХ для DC+, DC-", { true, commandsCurvers } });

        fpga0->SetDescriptionBits(0, desc0);

        AppendRegister(fpga0);
    }

    {
        Register *fpga1 = new RegFPGA(this, fpgas[1]);

        std::vector<StructDescription> desc1;

        std::vector<StructDescription::CommandStruct> commandsDuration; //-V827 //-V826
        desc1.emplace_back(StructDescription{ 0, 8, "Ти = N * Sk", "", { true, commandsDuration } });

        fpga1->SetDescriptionBits(0, desc1);

        AppendRegister(fpga1);
    }

    {
        Register *fpga2 = new RegFPGA(this, fpgas[2]);

        std::vector<StructDescription> desc2;

        std::vector<StructDescription::CommandStruct> commandsDuration; //-V827 //-V826
        desc2.emplace_back(StructDescription{ 0, 8, "RunDelay = N * 1us", "", { true, commandsDuration } });

        fpga2->SetDescriptionBits(0, desc2);

        AppendRegister(fpga2);
    }

    btnStart = new wxButton{ this, wxID_ANY, "Start", { 10, SD::Y_SB(500) }, { 75, BUTTON_HEIGHT } };
    btnStop = new wxButton{ this, wxID_ANY, "Stop", {10, SD::Y_SB(540)}, { 75, BUTTON_HEIGHT} };

    btnStop->Enable(false);

    Bind(wxEVT_BUTTON, &PageFPGA::OnEventButton, this);
}


void PageFPGA::OnEventButton(wxCommandEvent &event)
{
    event.Skip();

    int id = event.GetId();

    if (id == btnStart->GetId())
    {
        btnStart->Enable(false);
        btnStop->Enable(true);
    }
    else if (id == btnStop->GetId())
    {
        btnStart->Enable(true);
        btnStop->Enable(false);
    }
}
