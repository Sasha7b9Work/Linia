// 2025/6/3 22:44:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PageAD9952.h"


PageAD9952 *PageAD9952::self = nullptr;


PageAD9952::PageAD9952(wxNotebook *parent) : PageChip(parent, "AD9952")
{
    self = this;
}
