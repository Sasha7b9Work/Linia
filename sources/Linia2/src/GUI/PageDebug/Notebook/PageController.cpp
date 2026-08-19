// 2026/08/19 11:02:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageDebug/Notebook/PageController.h"


PageController *PageController::self = nullptr;


PageController::PageController(wxNotebook *notebook) :
    PageChip(notebook, "stm32")
{
    self = this;
}
