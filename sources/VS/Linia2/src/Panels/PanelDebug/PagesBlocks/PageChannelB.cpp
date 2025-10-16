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

    FillRegisterBS(reg3);

    AppendRegister(reg3);
}
