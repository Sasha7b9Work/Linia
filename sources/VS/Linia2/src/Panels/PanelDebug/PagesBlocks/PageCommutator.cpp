// 2025/10/07 11:46:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/PageCommutator.h"


/*
    Регистр управления 16 разрядов
*/

PageCommutator *PageCommutator::self = nullptr;


PageCommutator::PageCommutator(wxNotebook *parent) :
    wxPanel(parent)
{
    self = this;

    wxPanel::SetName("Коммутатор");
}
