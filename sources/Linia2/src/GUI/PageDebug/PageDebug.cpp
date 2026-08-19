// 2025/6/3 13:44:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageDebug/PageDebug.h"
#include "MainWindow.h"
#include "GUI/PageDebug/Notebook/NotebookDebug.h"
#include "GUI/PageDebug/PanelRight.h"
#pragma warning(push, 0)
#include <wx/sizer.h>
#pragma warning(pop)


PageDebug *PageDebug::self = nullptr;


PageDebug::PageDebug(Notebook *board) : PageNotebook(board, L("Отладка"))
{
    self = this;

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(new NotebookDebug(this, TheNotebookDebug));

    sizer->Add(new PanelRight(this, ThePanelRight));

    SetSizer(sizer);

    PageNotebook::Layout();

    Unpack();
}


bool PageDebug::Show(bool show)
{
    show ? TheNotebookDebug->Init() : TheNotebookDebug->DeInit();

    return wxPanel::Show(show);
}


void PageDebug::PeriodicTask()
{
    TheNotebookDebug->PeriodicTask();

//    ThePanelRight->PeriodicTask();
}


void PageDebug::Unpack()
{
    TheNotebookDebug->Unpack();
}


void PageDebug::Pack()
{
    TheNotebookDebug->Pack();
}
