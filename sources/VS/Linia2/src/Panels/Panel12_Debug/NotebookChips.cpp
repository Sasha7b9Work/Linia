// 2025/6/3 14:06:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel12_Debug/NotebookChips.h"
#include "Panels/Panel12_Debug/Old/PageAD5697.h"
#include "Panels/Panel12_Debug/PageAD5300.h"


NotebookChips *NotebookChips::self = nullptr;


NotebookChips::NotebookChips(wxWindow *parent) :
    wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP)
{
    self = this;

    new PageAD5300(this);
    new PageAD5697(this);

    wxNotebook::AddPage(PageAD5300::self, PageAD5300::self->GetName());
    wxNotebook::AddPage(PageAD5697::self, PageAD5697::self->GetName());

    wxWindowBase::Layout();
}
