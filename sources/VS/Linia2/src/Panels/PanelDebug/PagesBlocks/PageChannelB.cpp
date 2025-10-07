// 2025/10/07 12:05:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/PageChannelB.h"


/*
    ADC     AD7691      16 разрядов
    DAC     AD5531_1    12 разрядов
    DAC     AD5531_2    12 разрядов
    Регистр. упр.       32 разрядов
*/


PageChannelB *PageChannelB::self = nullptr;


PageChannelB::PageChannelB(wxNotebook *parent) :
    PageChip(parent, "Канал B")
{
    self = this;

    AppendRegister(new RegAD5531(this, "DAC3"));

    AppendRegister(new RegAD5531(this, "DAC4"));

    AppendRegister(new RegFPGA(this, "REG4", 32));

    AppendRegister(new RegAD7691(this, "ADC2"));
}
