// 2025/10/07 11:46:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/PageCommutator.h"
#include "Panels/PanelDebug/PagesBlocks/Register.h"


/*
    Регистр управления 16 разрядов
*/

PageCommutator *PageCommutator::self = nullptr;


PageCommutator::PageCommutator(wxNotebook *parent) :
    PageChip(parent, "Коммутатор")
{
    self = this;

    PanelRegister *reg = new RegFPGA(this, "REG2", 16);

    AppendRegister(reg);
}
