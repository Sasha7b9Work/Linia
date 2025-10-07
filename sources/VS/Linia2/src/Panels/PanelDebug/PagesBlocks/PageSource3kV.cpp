// 2025/10/07 11:39:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/PageSource3kV.h"

/*
    Регистр управления 24 разряда
*/

PageSource3kV *PageSource3kV::self = nullptr;


PageSource3kV::PageSource3kV(wxNotebook *parent) :
    wxPanel(parent)
{
    self = this;

    wxPanel::SetName("Источник 3 кВ");
}
