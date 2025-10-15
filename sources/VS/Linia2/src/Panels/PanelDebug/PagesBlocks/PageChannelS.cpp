// 2025/10/07 12:22:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/PageChannelS.h"


/*
    ADC     AD7691      16 разрядов
    DAC     AD5531_1    12 разрядов
    DAC     AD5531_2    12 разрядов
    Регистр. упр.       32 разрядов
*/


PageChannelS *PageChannelS::self = nullptr;


PageChannelS::PageChannelS(wxNotebook *parent) :
    PageChip(parent, "Канал S")
{
    self = this;

    AppendRegister(new RegAD5531(this, "DAC4"));

    AppendRegister(new RegAD5531(this, "DAC5"));

    AppendRegister(new RegFPGA(this, "REG4", 32));
}
