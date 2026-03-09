// 2025/10/07 11:51:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageChannelC.h"


/*
    ADC     AD7691      16 разрядов
    DAC     AD5543      12 разрядов
    Регистр. упр.       32 разрядов
*/


PageChannelC *PageChannelC::self = nullptr;


PageChannelC::PageChannelC(wxNotebook *parent) :
    PageChip(parent, "Канал C")
{
    self = this;

    {
        RegAD5543 *dac1 = new RegAD5543(this, dacs[1]);

        std::vector<StructDescription> desc1;

        desc1.emplace_back(StructDescription{ 0, 12, "", "", { true } });

        dac1->SetDescriptionBits(0, desc1);

        AppendRegister(dac1);
    }

    AppendRegister(new RegFPGA(this, regs[2]));

    {
        RegAD5543 *dac0 = new RegAD5543(this, dacs[0]);

        std::vector<StructDescription> desc1;

        desc1.emplace_back(StructDescription{ 0, 12, "", "", { true } });

        dac0->SetDescriptionBits(0, desc1);

        AppendRegister(dac0);
    }
}
