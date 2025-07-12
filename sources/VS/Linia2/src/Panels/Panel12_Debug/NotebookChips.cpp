// 2025/6/3 14:06:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel12_Debug/NotebookChips.h"
#include "Panels/Panel12_Debug/PageAD5697.h"
#include "Panels/Panel12_Debug/PageAD9952.h"
#include "Panels/Panel12_Debug/PageMCP4811.h"


NotebookChips *NotebookChips::self = nullptr;


NotebookChips::NotebookChips(wxWindow *parent) :
    wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP)
{
    self = this;

    new PageAD5697(this);
    new PageAD9952(this);
    new PageMCP4811(this);

    wxNotebook::AddPage(PageAD5697::self, PageAD5697::self->GetName());
    wxNotebook::AddPage(PageAD9952::self, PageAD9952::self->GetName());
    wxNotebook::AddPage(PageMCP4811::self, PageMCP4811::self->GetName());

    wxWindowBase::Layout();
}
