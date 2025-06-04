// 2025/6/3 22:47:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PageAD5697.h"
#include "Panels/Debug/PanelRegister.h"


PageAD5697 *PageAD5697::self = nullptr;


PageAD5697::PageAD5697(wxNotebook *parent) : PageChip(parent, "AD5697")
{
    self = this;

    new PanelRegister(this, "Input Register", 32);
}
