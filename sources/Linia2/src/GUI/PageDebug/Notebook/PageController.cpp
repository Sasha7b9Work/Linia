// 2026/08/19 11:02:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageDebug/Notebook/PageController.h"
#pragma warning(push, 0)
    #include <wx/statbox.h>
#pragma warning(pop)


PageController *PageController::self = nullptr;


PageController::PageController(wxNotebook *notebook) :
    PageChip(notebook, "stm32")
{
    self = this;

    wxStaticBox *box = new wxStaticBox(this, wxID_ANY, L("Обновление прошивки"), { 10, 10 }, { 200, 100 });

    (void)box;
}
