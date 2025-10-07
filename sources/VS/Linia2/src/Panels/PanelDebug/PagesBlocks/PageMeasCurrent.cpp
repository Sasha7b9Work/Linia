// 2025/10/07 12:25:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/PageMeasCurrent.h"

/*
    ADC     AD7691      16 разрядов
    Управление          16 разрядов
*/

PageMeasCurrent *PageMeasCurrent::self = nullptr;


PageMeasCurrent::PageMeasCurrent(wxNotebook *parent) :
    PageChip(parent, "Измеритель тока")
{
    self = this;

    AppendRegister(new RegFPGA(this, "REG6", 16));

    AppendRegister(new RegAD7691(this, "ADC4"));
}

