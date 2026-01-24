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

    RegFPGA *reg3 = new RegFPGA(this, regs[3]);

    FillRegisterBS(reg3);

    AppendRegister(reg3);
}
